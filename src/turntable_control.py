#!/usr/bin/env python

from numpy import angle
import rospy
from turntable import Turntable
from std_msgs.msg import Float64

class TurntableControl(object):

    def __init__(self):
        rospy.init_node('turntable_control')
        self.position_publisher  = rospy.Publisher('position', Float64, queue_size=2)
        self.turntable = Turntable()

    def run(self):
        main_rate = rospy.Rate(10)
        angle_queue = [-360,360,-360]#[a for a in range(10,360,10)]
        #self.turntable.home()
        #while not rospy.is_shutdown():
            
        while len(angle_queue) > 0:
            target_angle = angle_queue.pop(0)
            rospy.loginfo("Going to %f" %target_angle)
            self.turntable.yaw_to(target_angle)
            self.position_publisher.publish(target_angle)
            rospy.loginfo("At angle %f" %target_angle)


    
if __name__ == '__main__':
    try:
        tc = TurntableControl()
        tc.run()
    except rospy.ROSInterruptException:
        pass
