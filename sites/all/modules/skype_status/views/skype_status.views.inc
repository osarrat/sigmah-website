<?php
// $Id: skype_status.views.inc,v 1.1 2010/03/20 23:43:17 nicholasalipaz Exp $

function skype_status_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'skype_status') . '/views',
    ),
    'handlers' => array(
      'skype_status_handler_field_user' => array(
        'parent' => 'views_handler_field_user',
      ),
    ),
  );
}

function skype_status_views_data_alter(&$data) {
  $field = variable_get('skype_status_profile_field', '');
  $fieldtable = 'profile_values_' . $field;
  $data[$fieldtable]['value']['field']['handler'] = 'skype_status_handler_field_user';
}
