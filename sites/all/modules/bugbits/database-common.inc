<?php
// $Id: database-common.inc,v 1.1.2.3 2009/02/04 01:59:00 daften Exp $

/**
 * @file
 * Wrapper for database interface code.
 */

/**
 * Activate a database for bugbits queries. Don't forget to call _bugbits_db_set_inactive() to reactivate to 
 * default drupal database
 *
 * @return the name of the previously active database or FALSE if non was found.
 */
function _bugbits_db_set_active() {
  global $bugbits_drupal_db_name;

  $bugbits_drupal_db_name = db_set_active();
  
  $bugbits_db_type = variable_get('bugbits_db_type', '');
  $bugbits_db_handler = drupal_get_path('module', 'bugbits') . "/database.$bugbits_db_type.inc";
  
  if (is_file($bugbits_db_handler)) {
    include_once $bugbits_db_handler;
  }
  else {
    watchdog("Bugbits", "Bugbits setup is not done properly, database type unknown", array(), WATCHDOG_ERROR);
  }
}

/**
 * Activate the drupal database that was used before the bugbits hook took over.
 *
 * @return the name of the previously active database or FALSE if non was found.
 */
function _bugbits_db_set_inactive() {
  global $bugbits_drupal_db_name;
  
  db_set_active($bugbits_drupal_db_name);
}

/**
 * Append a database prefix to all tables in a query.
 *
 * Queries sent to Drupal should wrap all table names in curly brackets. This
 * function searches for this syntax and adds Drupal's table prefix to all
 * tables, allowing Drupal to coexist with other systems in the same database if
 * necessary.
 *
 * @param $sql
 *   A string containing a partial or entire SQL query.
 * @return
 *   The properly-prefixed string.
 */
function _bugbits_db_prefix_tables($sql) {
  $db_prefix = variable_get('bugbits_table_prefix', '');
  return strtr($sql, array('{' => $db_prefix, '}' => ''));
}