#!/vendor/bin/sh
if [ $# -eq 1 ]; then
  interval=$1
else
  exit 1
fi
while true; do
  logline="tz:"
  for f in /sys/class/thermal/thermal*; do
    temp=$(cat "$f"/temp)
    logline=$logline "|$temp"
  done
  logline=$logline" cdev:"
  for f in /sys/class/thermal/cooling_device*; do
    cur_state=$(cat "$f"/cur_state)
    logline=$logline"|$cur_state"
  done
  log -p w -t THERMAL_LOG "$logline"
  sleep "$interval"
done
