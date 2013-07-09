#!/usr/bin/python
import roslib; roslib.load_manifest('rosbridge')
import rospy
import socket, threading, SocketServer
import json
import thread, time

def serve(source_socket, destination_socket, type_lookup):

    encoder = json.JSONEncoder()
    decoder = json.JSONDecoder()

    for topic in type_lookup.keys():
        source_socket.send('\x00{"receiver":"/rosbridge/subscribe","msg":["' + topic + '",0,"' + type_lookup[topic] + '"]}\xff')
        destination_socket.send('\x00{"receiver":"/rosbridge/publish","msg":["' + topic + '","' + type_lookup[topic] + '"]}\xff')

    while True:
        incoming = source_socket.recv(1024)
        incoming = incoming[1:len(incoming)-1]
        try:
            incoming_dict = decoder.decode(incoming)
            msg_type = type_lookup[incoming_dict['receiver']]
        except:
            continue
        outgoing_dict = dict()
        outgoing_dict['receiver'] = incoming_dict['receiver']
        outgoing_dict['msg'] = incoming_dict['msg']
        outgoing_dict['type'] = msg_type
        outgoing = encoder.encode(outgoing_dict)
        outgoing = '\x00' + outgoing + '\xff'
        destination_socket.send(outgoing)

if __name__ == "__main__":

    rospy.init_node('ros2ros')
    rospy.sleep(5)

    host1_address = rospy.get_param('/brown/ros2ros/host1_address', '127.0.0.1')
    host1_port = rospy.get_param('/brown/ros2ros/host1_port', 9090)
    host2_address = rospy.get_param('/brown/ros2ros/host2_address', '127.0.0.1')
    host2_port = rospy.get_param('/brown/ros2ros/host2_port', 9090)
    host1_topics = rospy.get_param('/brown/ros2ros/host1_topics', [])
    host2_topics = rospy.get_param('/brown/ros2ros/host2_topics', [])
    host1_topic_types = rospy.get_param('/brown/ros2ros/host1_topic_types', [])
    host2_topic_types = rospy.get_param('/brown/ros2ros/host2_topic_types', [])

    host1_type_lookup = dict(zip(host1_topics, host1_topic_types))
    host2_type_lookup = dict(zip(host2_topics, host2_topic_types))

    host1_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    host1_sock.connect((host1_address, host1_port))
    host1_sock.send('raw\r\n\r\n')

    host2_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    host2_sock.connect((host2_address, host2_port))
    host2_sock.send('raw\r\n\r\n')

    rospy.sleep(5)

    thread.start_new_thread(serve,(host2_sock, host1_sock, host2_type_lookup))
    thread.start_new_thread(serve,(host1_sock, host2_sock, host1_type_lookup))

    while not rospy.is_shutdown():
        rospy.spin()
