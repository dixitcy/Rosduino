#include <ros/ros.h>
#include <regui/TwoInts.h>

bool add(regui::TwoInts::Request &req,
        regui::TwoInts::Response &res )
{
    res.sum = req.n1 + req.n2;
    return true;
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "add_two_ints_server");
    ros::NodeHandle n;

    ros::ServiceServer service = n.advertiseService("add_two_ints", add);

    ros::spin();

    return 0;
}
