<?php
/**
 * @file
 * Page handlers for deploy settings admin.
 */

/**
 * Manage deployment settings.
 *
 * @ingroup forms
 * @see system_settings_form()
 */
function deploy_settings() {
  $group_weight = -20;
  $form['general'] = array(
    '#type' => 'fieldset',
    '#weight' => $group_weight,
    '#title' => t('General settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  return system_settings_form($form);
}

