Dining Philosopher
=== 
# Table
- 2~4 philosophers can enter this table
- Using condition variable and mutex to maintain synchronization
- At most four philosopher in this table to prevent deadlock
# Fork
- Value equals to one when using this fork,0 is opposite
- Using condition variable and mutex to maintain synchronization
# Philosopher
- Continue think->pick up left fork -> eat ->pick up right fork -> put down fork
- Finally leave & enter table
- Remember at most four philosopher stay simultaneously in this table
# Dining Philosopher Demo
https://youtu.be/kLuxj88Kob4
