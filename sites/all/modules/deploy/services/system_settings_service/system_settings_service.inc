<?php
/**
 * @file
 * Service for the management of system settings that used Drupals'
 * system_settings_form() API.
 *
 * @todo
 *   Would be nice to have an export service too.
 */

/**
 * Import system settings that use system_settings_form().
 *
 * Just takes the submitted form values and passes them straight
 * in to system_settings_form_submit().
 *
 * @param $form_values
 *   The $form_values array from a submitted system_settings_form().
 * @return
 *   bool
 * @todo
 *   Needs error handling
 */
function system_settings_service_import($form_values) {
  $form = array();
  $form_state = array();
  $form_state['values'] = $form_values;
  system_settings_form_submit($form, $form_state);
  watchdog("Services", "System Settings Import Service run for $form_id.");
  drupal_flush_all_caches();
  return TRUE;
}

/**
 * Check if user has access to import system settings.
 *
 * @return
 *   bool
 */
function system_settings_service_import_access() {
  return user_access('import system settings');
}