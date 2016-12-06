#include <ros/ros.h>
#include <ros/package.h>
#include <tree_generator/TreeFromKF.hpp>
#include <sarafun_msgs/BTGeneration.h>

tree_generator::TreeFromKF *create_json;
std::string tree_name;

/*
  Get a keyframe list and create the respective json file
*/
bool keyframeCallback(sarafun_msgs::BTGeneration::Request &req,
                      sarafun_msgs::BTGeneration::Response &res)
{
  try
  {
    std::string label;
    // For now, I am loading the indices in the parameter server, and I expect the
    // client nodes to read this parameters. TODO: If we want to pass more information
    // from the keyframes to the clients in the future, we will need to adopt a more
    // interesting way of passing values (possibly through the json)
    for (int i = 0; i < req.keyframe_sequence.size(); i++)
    {
      label = req.keyframe_sequence[i].label;
      tree_generator::replaceWithUnderscore(label);
      ros::param::set("/sarafun/" + label + "/idx", req.keyframe_sequence[i].idx);
    }

    json tree = create_json->createTree(req.keyframe_sequence);
    std::ofstream file;
    std::string path = ros::package::getPath("tree_generator") + "/data/generated/" + tree_name + ".json";


    file.open(path);
    file << tree.dump(2);
    file.close();
    return true;
  }
  catch(std::logic_error &e)
  {
    ROS_ERROR("Tried to create a tree, but got the logic error: %s", e.what());
    return false;
  }
  catch(...)
  {
    ROS_ERROR("Tried to create a tree, but got an unknown error!");
    return false;
  }
}

int main(int argc, char ** argv)
{
  ros::init(argc, argv, "tree_generator");
  ros::NodeHandle nh;
  std::string kf_service;
  std::string node_name = ros::this_node::getName();

  if (!nh.getParam(node_name + "/keyframe_service_name", kf_service))
  {
    ROS_ERROR("Tree generator started without a keyframe service name in the parameter server! Aborting (%s/keyframe_service_name)", node_name.c_str());
    ros::shutdown();
    return -1;
  }

  if (!nh.getParam(node_name + "/name", tree_name))
  {
    ROS_ERROR("Tree generator requires a name for the generated trees! (%s/name)", node_name.c_str());
    ros::shutdown();
    return -1;
  }

  create_json = new tree_generator::TreeFromKF;
  ros::ServiceServer tree_generation_service = nh.advertiseService(kf_service, keyframeCallback);
  ROS_INFO("Started the tree generator node");
  ros::spin();
  return 0;
}
