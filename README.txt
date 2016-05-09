The graph simulation has two modes to run

1. Interactive
2. Batch - Gets commands from input file

Usage Example
------------

  Usage : ./graph_simulator [OPTIONS] [FILE_NAME]

  -interactive                     Run in interactive mode

  -batch -file <FILE_NAME>         Run in batch mode (Commands from file)

  -help                            print this message

  
The following commands are available that can be executed in any one of the above modes.
  

Available Commands:
-------------------

  delete_graph                                                                              Delete the current graph

  add_node <NODE_ID>                                                                        Add a new node to the graph

  delete_node <NODE_ID>                                                                     Delete the given node from the graph

  create_graph -max_node <MAX_NODE> -max_edge <MAX_EDGE>                                    Create new graph with given max node and max edge

  add_edge -src_node <SRC_NODE_ID> -dest_node <DEST_NODE_ID> -weight <WEIGHT>               Add a new edage to the graph b/w src and dest

  delete_edge -src_node <SRC_NODE_ID> -dest_node <DEST_NODE_ID>                             Delete the given edge from the graph

  start_flow -src_node <SRC_NODE_ID> -dest_node <DEST_NODE_ID> -time <DURATION>             Start data flow b/w src and dest for given duration (in sec)

Current Limitation
------------------
1. Maximum edges constraint is not added yet. 
2. Can start the simulation only one src-dest at a time
3. The time input getting from start_flow command is used as no of cycle in flow as of now


Sample output log
----------------

List of paths the data will be transfering:
-------------------------------------------
9 ----> 0 ----> 1 ----> 2 ----> 3 ----> 4 ----> 5 ----> 6 ----> 7 ----> 8 ----> 


Cycle : 0
============

                          EDGE STATISTICS                           
----------------------------------------------------------------------------------
    Edge    |  ACTUAL CAPACITY   |   CURRENT CAPACITY   |  # PKTs TRANSFERRED  |
----------------------------------------------------------------------------------
   9--->0   |         1          |          0           |          1           |
   0--->1   |         1          |          1           |          0           |
   1--->2   |         1          |          1           |          0           |
   2--->3   |         1          |          1           |          0           |
   3--->4   |         1          |          1           |          0           |
   4--->5   |         1          |          1           |          0           |
   5--->6   |         1          |          1           |          0           |
   6--->7   |         1          |          1           |          0           |
   7--->8   |         1          |          1           |          0           |

                          NODE STATISTICS                           
----------------------------------------------------------------------------------
  NODE #  |  TOTAL PKTs RECEIVED  |  TOTAL PKTs SENT  |  TOTAL PKTs REMAINING  |
----------------------------------------------------------------------------------
    9     |           0           |         1         |       4294967295       |
    0     |           1           |         0         |           1            |
    1     |           0           |         0         |           0            |
    2     |           0           |         0         |           0            |
    3     |           0           |         0         |           0            |
    4     |           0           |         0         |           0            |
    5     |           0           |         0         |           0            |
    6     |           0           |         0         |           0            |
    7     |           0           |         0         |           0            |
    8     |           0           |         0         |           0            |

	....
	
Cycle : 19
============

                          EDGE STATISTICS                           
----------------------------------------------------------------------------------
    Edge    |  ACTUAL CAPACITY   |   CURRENT CAPACITY   |  # PKTs TRANSFERRED  |
----------------------------------------------------------------------------------
   9--->0   |         1          |          0           |          1           |
   0--->1   |         1          |          0           |          1           |
   1--->2   |         1          |          0           |          1           |
   2--->3   |         1          |          0           |          1           |
   3--->4   |         1          |          0           |          1           |
   4--->5   |         1          |          0           |          1           |
   5--->6   |         1          |          0           |          1           |
   6--->7   |         1          |          0           |          1           |
   7--->8   |         1          |          1           |          1           |

                          NODE STATISTICS                           
----------------------------------------------------------------------------------
  NODE #  |  TOTAL PKTs RECEIVED  |  TOTAL PKTs SENT  |  TOTAL PKTs REMAINING  |
----------------------------------------------------------------------------------
    9     |           0           |        20         |       4294967295       |
    0     |          20           |        19         |           1            |
    1     |          19           |        18         |           1            |
    2     |          18           |        17         |           1            |
    3     |          17           |        16         |           1            |
    4     |          16           |        15         |           1            |
    5     |          15           |        14         |           1            |
    6     |          14           |        13         |           1            |
    7     |          13           |        12         |           1            |
    8     |          12           |         0         |           0            |


Flow statistics:
================

Number of data sent from src	:	20
Number of data received at dest	:	12
Data flow rate from src to dest	:	60%	