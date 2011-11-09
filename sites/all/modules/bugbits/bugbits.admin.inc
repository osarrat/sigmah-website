<?php

// $Id: bugbits.admin.inc,v 1.1.2.9 2009/04/24 02:20:00 daften Exp $


/**
 * @file
 * Administration page callbacks for the bugbits module
 */

/**
 * Form builder. Configure bugbits
 * 
 * @ingroup forms
 * @see system_settings_form()
 */
function bugbits_admin_settings() {
  global $base_url;

  $form['bugbits_mantis_url'] = array(
  	'#type' => 'textfield', 
  	'#size' => 50, 
  	'#title' => t('Public mantis URL'),
    '#required' => TRUE, 
  	'#default_value' => variable_get('bugbits_mantis_url', $base_url . '/' . drupal_get_path('module', 'bugbits') . '/mantis'), 
  	'#description' => t('The url where your mantis installation can be found, this should be the root path without a trailing slash and this url should be publicly accessible.')
  );
  
  $form['database'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
  	'#title' => t('Database settings for mantis'),
    '#required' => TRUE,  
  	'#description' => t('Here you can enter the database settings for mantis.')
  );
  $form['database']['bugbits_db_host'] = array(
    '#type' => 'textfield',
  	'#size' => 15,
    '#required' => TRUE,
  	'#title' => t('Database host for the mantis database'), 
  	'#default_value' => variable_get('bugbits_db_host', 'localhost'),
  	'#description' => t('Give the host for the mantis database.')
  );
  $form['database']['bugbits_db_port'] = array(
    '#type' => 'textfield',
  	'#size' => 5,
    '#required' => TRUE,
  	'#title' => t('Database port for the mantis database'), 
  	'#default_value' => variable_get('bugbits_db_port', '3306'),
  	'#description' => t('Give the port for the mantis database.')
  );
  $form['database']['bugbits_db_type'] = array(
    '#type' => 'select',
  	'#title' => t('Database type for mantis database'),
    '#required' => TRUE, 
  	'#default_value' => variable_get('bugbits_db_type', ''),
    '#options' => array(
      'mysql' => t('MySql'),
      'pgsql' => t('Postgresql'),
    ),
  	'#description' => t('Give the type of database in which the mantis tables are stored here.')
  );
  $form['database']['bugbits_table_prefix'] = array(
    '#type' => 'textfield',
  	'#size' => 15,
    '#required' => FALSE,
  	'#title' => t('Table prefix for mantis tables'), 
  	'#default_value' => variable_get('bugbits_table_prefix', 'mantis_'),
  	'#description' => t('Give the prefix for mantis tables (commonly nothing - an empty string - or "mantis_"). If there is an underscore (or any other seperating character), it should be added here.')
  );
  $form['database']['bugbits_db_username'] = array(
    '#type' => 'textfield',
  	'#size' => 15,
    '#required' => TRUE,
  	'#title' => t('Database username for mantis database'), 
  	'#default_value' => variable_get('bugbits_db_username', ''),
  	'#description' => t('Give the username for the database in which the mantis tables are stored here.')
  );
  $form['database']['bugbits_db_password'] = array(
    '#type' => 'textfield',
  	'#size' => 15,
    '#required' => TRUE,
  	'#title' => t('Database password for mantis database'), 
  	'#default_value' => variable_get('bugbits_db_password', ''),
  	'#description' => t('Give the password for the database in which the mantis tables are stored here.')
  );
  $form['database']['bugbits_db_name'] = array(
    '#type' => 'textfield',
  	'#size' => 15,
    '#required' => TRUE,
  	'#title' => t('Database name for mantis database'), 
  	'#default_value' => variable_get('bugbits_db_name', ''),
  	'#description' => t('Give the name for the database in which the mantis tables are stored here.')
  );
  
  return system_settings_form($form);
} // function bugbits_admin_settings

function bugbits_admin_test() {
  $output = '';
  
  // Test the database settings
  $output .= '<h1>' . t('Bugbits database settings') . '</h1>';
  if(!in_array(variable_get("bugbits_db_type", ''), array("mysql", "pgsql"))) {
    $output .= '<p>' . t('You\'re database type is not set correctly.') . '</p>';
  } else {
    $output .= '<p>' . t('You\'re database type: OK.') . '</p>';
    require_once 'database-common.inc';
    _bugbits_db_set_active();
    if(!_bugbits_db_connect(variable_get("bugbits_db_username", ''), variable_get("bugbits_db_password", ''), variable_get("bugbits_db_name", ''), variable_get('bugbits_db_host', ''), variable_get('bugbits_db_port', ''))) {
      $output .= '<p>' . t('You\'re database connection settings (username, password, database name, database host, database port) are not set correctly.') . '</p>';
    } else {
      $output .= '<p>' . t('You\'re database connection settings (username, password, database name, database host, database port): OK.') . '</p>';
      $num_users = _bugbits_db_result(_bugbits_db_query('SELECT COUNT(*) FROM {user_table}'));
      if(!is_numeric($num_users) || $num_users<0) {
        $output .= '<p>' . t('You\'re database prefix is not set correctly. Or you\'re using the wrong database.') . '</p>';
      } else {
        $output .= '<p>' . t('You\'re database prefix: OK.') . '</p>';
      }
    }
  }
  
  // Test the soap settings
  // TODO
  $output .= '<h1>' . t('Bugbits SOAP settings') . '</h1>';
  $output .= '<p>' . 'To do' . '</p>';
  
  return $output;
}