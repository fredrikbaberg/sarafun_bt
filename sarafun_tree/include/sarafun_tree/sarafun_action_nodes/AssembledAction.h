#ifndef __ASSEMBLED_ACTION__
#define __ASSEMBLED_ACTION__

#include <ros/ros.h>
#include <sarafun_tree/ExecuteAction.h>
#include <sarafun_msgs/AssembledKeyframeAction.h>

namespace sarafun {
class AssembledAction
    : ExecuteAction<sarafun_msgs::AssembledKeyframeAction,
                    sarafun_msgs::AssembledKeyframeGoal> {
public:
  AssembledAction(std::string node_name, std::string action_name,
                        std::string bt_name)
      : ExecuteAction<
            sarafun_msgs::AssembledKeyframeAction,
            sarafun_msgs::AssembledKeyframeGoal>::ExecuteAction(node_name,
                                                               action_name,
                                                               bt_name),
        node_name_(node_name), bt_name_(bt_name) {
    node_handle_ = ros::NodeHandle(node_name);
  }
  ~AssembledAction() {}

  bool fillGoal(sarafun_msgs::AssembledKeyframeGoal &goal);
  double getTimeoutValue();

private:
  ros::NodeHandle node_handle_;
  std::string node_name_;
  std::string bt_name_;
  std::vector<int> idx_list_;
  int curr_idx_;
};
}
#endif
