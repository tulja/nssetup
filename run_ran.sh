cd /opt/openbaton/scripts/NFV_LTE_EPC-1.0/src/
screen -d -m -S ran sudo ./ransim.out 10 100 | sudo tee ran_run.log
