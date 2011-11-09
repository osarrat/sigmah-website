NOTE: This module is currently in an alpha state, and many aspects of it remain 
in the proof-of-concept realm. Please do not use it for any production purposes 
until such a time as a 1.0 is released.

The deployment framework is a series of modules which are designed to allow 
developers to easily stage Drupal data from one site to another. It is designed 
to have three parts that work together:

* Deployment API - This implements the concept of a deployment plan. You create 
a deployment plan and add objects to it which will be deployed (content types, 
views, etc.) When the time comes these items are pushed to a server you specify 
via XMLRPC with an API key. The data stored about each item is extremely minimal, 
relying largely on the implementers to implement object-specific knowledge. 
Currently this is deploy.module

* Deployment Implementers - Individual modules implement the deployment API to 
add the data they need to a deployment plan, and expose that ability to the front 
end. Currently I have content_copy_deploy.module and system_settings_deploy.module.

* Deployment Services - Services modules which contain the knowledge to receive 
deployed data and do what is appropriate on the destination server. Currently I 
have content_copy_service.module and system_settings_service.module. 

Installation and usage instructions can be found in INSTALL.txt

