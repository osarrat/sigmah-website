<?php
/**
 * @file
 *  Link CCK content copy functionality to services module.
 */

/**
 * Content copy import service callback
 */
function content_copy_service_import($type_name, $export) {
  if (!content_copy_content_type_exists($type_name)) {
    $type_name = '<create>';
  }

  $values = array('type_name' => $type_name, 'macro' => $export, 'op' => 'Submit');
  $form_state['values'] = $values;

  drupal_execute('content_copy_import_form', $form_state);

  if ($errors = form_get_errors()) {
    foreach ($errors as $error) {
      $msg .= "$error ";
    }
    return services_error("Content Copy Import Error: $msg");
    watchdog('deploy', $msg);
  }
  else {
    watchdog("services", "Content Copy Import Service run.");
  }

  return TRUE;
}

/**
 * Content copy export service callback
 */
function content_copy_service_export($type_name) {
  if (!content_copy_content_type_exists($type_name)) {
    return services_error("Content type $type_name does not exist");
  }
  
  if (module_exists('fieldgroup')) {
    $groups = array_keys(fieldgroup_groups($content_type));
  }

  $fields = array_values(content_copy_fields($content_type));

  $values = array(
    'type_name' => $content_type,
    'groups' => $groups,
    'fields' => $fields,
  );

  $export = content_copy_export($values);
  
  watchdog("services", "Content Copy Export Service run for content type $type_name.");

  return $retval;
}

/**
 * Content type exists service callback
 */
function content_copy_content_type_exists($type_name) {
  if (array_key_exists($type_name, node_get_types())) {
    return TRUE;
  }  
  return FALSE;
}