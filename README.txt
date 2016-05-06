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

1 ----> 2 ----> 6 ----> 7 ----> 9 ----> 
1 ----> 2 ----> 4 ----> 7 ----> 9 ----> 
1 ----> 2 ----> 4 ----> 9 ----> 
1 ----> 3 ----> 4 ----> 7 ----> 9 ----> 
1 ----> 3 ----> 4 ----> 9 ----> 


----------------------------------------------------------------------------------------------------------


                                      NODES STATISTICS - CYCLE 0                                      
----------------------------------------------------------------------------------------------------------
  NODE #  |  TOTAL PKTs SENT  |  TOTAL PKTs RECEIVED  |  TOTAL PKTs REMAINING  |  PKT DELIVERY PERCENT   |
----------------------------------------------------------------------------------------------------------
    1     |        17         |           0           |        9999999         |           NA            |
    2     |         0         |          10           |           10           |            0            |
    3     |         0         |           7           |           7            |            0            |
    4     |         0         |           0           |           0            |           NA            |
    6     |         0         |           0           |           0            |           NA            |
    7     |         0         |           0           |           0            |           NA            |
    9     |         0         |           0           |           0            |           NA            |
----------------------------------------------------------------------------------------------------------


                                      NODES STATISTICS - CYCLE 1                                      
----------------------------------------------------------------------------------------------------------
  NODE #  |  TOTAL PKTs SENT  |  TOTAL PKTs RECEIVED  |  TOTAL PKTs REMAINING  |  PKT DELIVERY PERCENT   |
----------------------------------------------------------------------------------------------------------
    1     |        17         |           0           |        9999999         |           NA            |
    2     |         7         |          10           |           3            |           70            |
    3     |         4         |           7           |           3            |         57.1429         |
    4     |         0         |           8           |           8            |            0            |
    6     |         0         |           3           |           3            |            0            |
    7     |         0         |           0           |           0            |           NA            |
    9     |         0         |           0           |           0            |           NA            |
----------------------------------------------------------------------------------------------------------


                                      NODES STATISTICS - CYCLE 2                                      
----------------------------------------------------------------------------------------------------------
  NODE #  |  TOTAL PKTs SENT  |  TOTAL PKTs RECEIVED  |  TOTAL PKTs REMAINING  |  PKT DELIVERY PERCENT   |
----------------------------------------------------------------------------------------------------------
    1     |        28         |           0           |        9999999         |           NA            |
    2     |         7         |          17           |           10           |         41.1765         |
    3     |         4         |          11           |           7            |         36.3636         |
    4     |         8         |           8           |           0            |           100           |
    6     |         3         |           3           |           0            |           100           |
    7     |         0         |           7           |           7            |            0            |
    9     |         0         |           4           |           0            |            0            |
----------------------------------------------------------------------------------------------------------


                                      NODES STATISTICS - CYCLE 3                                      
----------------------------------------------------------------------------------------------------------
  NODE #  |  TOTAL PKTs SENT  |  TOTAL PKTs RECEIVED  |  TOTAL PKTs REMAINING  |  PKT DELIVERY PERCENT   |
----------------------------------------------------------------------------------------------------------
    1     |        28         |           0           |        9999999         |           NA            |
    2     |        14         |          17           |           3            |         82.3529         |
    3     |         8         |          11           |           3            |         72.7273         |
    4     |         8         |          16           |           8            |           50            |
    6     |         3         |           6           |           3            |           50            |
    7     |         5         |           7           |           2            |         71.4286         |
    9     |         0         |           9           |           0            |            0            |
----------------------------------------------------------------------------------------------------------
