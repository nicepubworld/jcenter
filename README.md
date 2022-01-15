## jcenter

c/c++ jprintf logs with different color set by module or error level

Using the conf file below:

```
#path to log file
logpath=/tmp/jcenter.log
#debug level
#0:info 1:trace 2:warn 3:err 4:fatal
debug_level=1
#debug config
#0: close 1:open
main=1
#open log file or not
#0:close 1:open
logfile=1
```

Get log print like this:

```
[trace]main[2022-01-15 16:19:27 ms:1642235239477]hello!
```