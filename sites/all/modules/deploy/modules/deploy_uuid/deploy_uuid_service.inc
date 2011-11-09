<?php

function deploy_uuid_service_access() {
  return TRUE;
}

function deploy_uuid_service_get_key($uuid, $module){
  return deploy_uuid_get_key($uuid, $module);
}