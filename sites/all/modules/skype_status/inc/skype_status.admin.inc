<?php
// $Id: skype_status.admin.inc,v 1.4 2010/04/21 15:39:42 nicholasalipaz Exp $

function skype_status_admin_settings() {
  drupal_add_js(drupal_get_path('module', 'skype_status') . '/inc/skype_status.js');
  $form = array();
  $fields = array();
  $result = db_query("SELECT * FROM {profile_fields} WHERE type = 'textfield' ORDER BY category, weight");
  while ($field = db_fetch_object($result)) {
    $fields[$field->name] = t('%category - @title', array('%category' => $field->category, '@title' => $field->title));
  }
  $images_dirs = array('balloon', 'bigclassic', 'smallclassic', 'mediumicon', 'smallicon');
  $opts = array();
  foreach ($images_dirs as $style) {
    $opts[$style] = theme('image', drupal_get_path('module', 'skype_status') . '/images/' . $style . '/1.png', $style, NULL, array('id' => $style, 'class' => 'skype-status-img-rotate'));
  }
  
  $form['profile'] = array(
    '#type' => 'fieldset',
    '#title' => t('User profiles')
  );
  $form['profile']['skype_status_profile_field'] = array(
    '#type' => 'radios',
    '#title' => t('Profile field for Skype ID'),
    '#options' => $fields,
    '#default_value' => variable_get('skype_status_profile_field', ''),
    '#required' => TRUE,
    '#description' => t('Select the custom profile field that contains users\' Skype IDs. Note that users who have not filled out this profile field will be ignored when updating status information from Skype\'s web presence service API.')
  );
  $form['profile']['skype_status_show_title'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show the title for this field'),
    '#description' => t('Uncheck this box to hide the title for the chosen field.'),
    '#default_value' => variable_get('skype_status_show_title', 1),
  );
  $form['images'] = array(
    '#type' => 'fieldset',
    '#title' => t('Skype Images'),
    '#description' => t('Choose the imageset you would like to use for skype status.')
  );
  $form['images']['skype_status_buttonstyle'] = array(
    '#type' => 'radios',
    '#title' => t('Button style preference.'),
    '#options' => $opts,
    '#default_value' => variable_get('skype_status_buttonstyle', 'bigclassic'),
  );

  return system_settings_form($form);
}
