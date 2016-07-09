# MobilePartnerClick

MobilePartnerClick -- utility to automate work with MobilePartner application, which is provided with Huawei E3276 LTE modems.

MobilePartner application lacks feature to keep connection up, and does not make a connection on startup. To solve these two issues system administrator could switch dongle's mode to become AT-modem, or could use the following utility. 

MobilePartnerClick keeps an eye on MobilePartner application and moves mouse cursor and clicks on "Connect" button if connection gets lost. 

MobilePartnerClick is meant to work on dedicated system (e.g. virtual machine), where users won't interfere utility's work and utility won't interfere user's work.

