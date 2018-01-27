cd /opt/openbaton/scripts/NFV_LTE_EPC-1.0/src/
screen -d -m -S mme sudo ./mme.out 10 | sudo tee mme_run.log
