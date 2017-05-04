#include <sarafun_tree/sarafun_action_nodes/ContactAction.h>

namespace sarafun {
bool ContactAction::fillGoal(sarafun_msgs::ContactKeyframeGoal &goal) {
  bool prev_result;

  if (idx_list_.size() == 0)
  {
    if(!fillParameter("/sarafun/contact/idx", idx_list_))
    {
      goal.idx = 0;
      return false;
    }
  }

  goal.idx = idx_list_[curr_idx_];
  curr_idx_++;

  if (curr_idx_ >= idx_list_.size())
  {
    curr_idx_ = 0;
  }

  return true;
}

double ContactAction::getTimeoutValue() {
  double timeout = 0;
  fillParameter("/sarafun/bt_action_nodes/contact/timeout", 30.0, timeout);

  return timeout;
}
}

int main(int argc, char **argv) {
  ros::init(argc, argv, "ContactAction");
  std::string bt_client_name, action_server_name;

  if (!ros::param::get("/sarafun/bt_action_nodes/contact/action_server_name", action_server_name))
  {
    ROS_ERROR("%s missing action server name", ros::this_node::getName().c_str());
    return -1;
  }

  if (!ros::param::get("/sarafun/bt_action_nodes/contact/bt_client_name", bt_client_name))
  {
    ROS_ERROR("%s missing bt client name", ros::this_node::getName().c_str());
    return -1;
  }

  sarafun::ContactAction contact_action(
      ros::this_node::getName(), action_server_name, bt_client_name);
  ros::spin();
  return 1;
}
