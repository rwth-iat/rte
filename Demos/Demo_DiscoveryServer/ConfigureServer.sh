#!/bin/sh 

# Security
curl "http://localhost:7509/setVar?format=plain&path=/TechUnits/DiscoveryServer.Security.DBWrapperPath&newvalue=/TechUnits/DB1"
curl "http://localhost:7509/setVar?format=plain&path=/TechUnits/DiscoveryServer.Security.AddDBWrapper&newvalue=TRUE"
curl "http://localhost:7509/setVar?format=plain&path=/TechUnits/DiscoveryServer.Security.DSServicePath&newvalue=/TechUnits/DSSecurity"
curl "http://localhost:7509/setVar?format=plain&path=/TechUnits/DiscoveryServer.Security.AddDSService&newvalue=TRUE"
curl "http://localhost:7509/setVar?format=plain&path=/TechUnits/DiscoveryServer.Security.DSServicePaths&newvalue=/TechUnits/DSSecurity"
curl "http://localhost:7509/setVar?format=plain&path=/TechUnits/DiscoveryServer.Security.UseDSServices&newvalue=TRUE"
curl "http://localhost:7509/setVar?format=plain&path=/TechUnits/DiscoveryServer.Security.DBWrapperPaths&newvalue=/TechUnits/DB1"
curl "http://localhost:7509/setVar?format=plain&path=/TechUnits/DiscoveryServer.Security.DSServicePathForConfig&newvalue=/TechUnits/DSSecurity"
curl "http://localhost:7509/setVar?format=plain&path=/TechUnits/DiscoveryServer.Security.ConfigDSServices&newvalue=TRUE"

# Registration
curl "http://localhost:7509/setVar?format=plain&path=/TechUnits/DiscoveryServer.Registration.DBWrapperPath&newvalue=/TechUnits/DB1"
curl "http://localhost:7509/setVar?format=plain&path=/TechUnits/DiscoveryServer.Registration.AddDBWrapper&newvalue=TRUE"
curl "http://localhost:7509/setVar?format=plain&path=/TechUnits/DiscoveryServer.Registration.DSServicePath&newvalue=/TechUnits/DSRegistration"
curl "http://localhost:7509/setVar?format=plain&path=/TechUnits/DiscoveryServer.Registration.AddDSService&newvalue=TRUE"
curl "http://localhost:7509/setVar?format=plain&path=/TechUnits/DiscoveryServer.Registration.DSServicePath&newvalue=/TechUnits/DSUnregistration"
curl "http://localhost:7509/setVar?format=plain&path=/TechUnits/DiscoveryServer.Registration.AddDSService&newvalue=TRUE"
curl "http://localhost:7509/setVar?format=plain&path=/TechUnits/DiscoveryServer.Registration.DSServicePaths&newvalue=%7B/TechUnits/DSRegistration%7D%2520%7B/TechUnits/DSUnregistration%7D"
curl "http://localhost:7509/setVar?format=plain&path=/TechUnits/DiscoveryServer.Registration.UseDSServices&newvalue=TRUE"
curl "http://localhost:7509/setVar?format=plain&path=/TechUnits/DiscoveryServer.Registration.DBWrapperPaths&newvalue=/TechUnits/DB1"
curl "http://localhost:7509/setVar?format=plain&path=/TechUnits/DiscoveryServer.Registration.DSServicePathForConfig&newvalue=/TechUnits/DSRegistration"
curl "http://localhost:7509/setVar?format=plain&path=/TechUnits/DiscoveryServer.Registration.ConfigDSServices&newvalue=TRUE"
curl "http://localhost:7509/setVar?format=plain&path=/TechUnits/DiscoveryServer.Registration.DSServicePathForConfig&newvalue=/TechUnits/DSUnregistration"
curl "http://localhost:7509/setVar?format=plain&path=/TechUnits/DiscoveryServer.Registration.ConfigDSServices&newvalue=TRUE"

# Search
curl "http://localhost:7509/setVar?format=plain&path=/TechUnits/DiscoveryServer.Search.DBWrapperPath&newvalue=/TechUnits/DB1"
curl "http://localhost:7509/setVar?format=plain&path=/TechUnits/DiscoveryServer.Search.AddDBWrapper&newvalue=TRUE"
curl "http://localhost:7509/setVar?format=plain&path=/TechUnits/DiscoveryServer.Search.DSServicePaths&newvalue=/TechUnits/DSSearch"
curl "http://localhost:7509/setVar?format=plain&path=/TechUnits/DiscoveryServer.Search.AddDSService&newvalue=TRUE"
curl "http://localhost:7509/setVar?format=plain&path=/TechUnits/DiscoveryServer.Search.DSServicePaths&newvalue=/TechUnits/DSSearch"
curl "http://localhost:7509/setVar?format=plain&path=/TechUnits/DiscoveryServer.Search.UseDSServices&newvalue=TRUE"
curl "http://localhost:7509/setVar?format=plain&path=/TechUnits/DiscoveryServer.Search.DBWrapperPaths&newvalue=/TechUnits/DB1"
curl "http://localhost:7509/setVar?format=plain&path=/TechUnits/DiscoveryServer.Search.DSServicePathForConfig&newvalue=/TechUnits/DSSearch"
curl "http://localhost:7509/setVar?format=plain&path=/TechUnits/DiscoveryServer.Search.ConfigDSServices&newvalue=TRUE"

# DB1
curl "http://localhost:7509/setVar?format=plain&path=/TechUnits/DB1.StartConnect&newvalue=TRUE"
exit
