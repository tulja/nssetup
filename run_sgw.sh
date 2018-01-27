cd /opt/openbaton/scripts/NFV_LTE_EPC-1.0/src/
screen -d -m -S sgw sudo ./sgw.out 10 10 10 | sudo tee sgw_run.log
