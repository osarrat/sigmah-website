<?php
// $Id: database.pgsql.inc,v 1.1.2.3 2009/02/04 01:59:00 daften Exp $

/**
 * @file
 * Mantis database interface code for PostgreSQL database servers.
 */

function _bugbits_db_connect($username, $password, $database_name, $host = 'localhost', $port = NULL) {
  // Check if PostgreSQL support is present in PHP
  if (!function_exists('pg_connect')) {
    watchdog("Bugbits", "Unable to use the PostgreSQL database (for Mantis) because the PostgreSQL extension for PHP is not installed. Check your <code>php.ini</code> to see how you can enable it.", array(), WATCHDOG_ERROR);
  }
  
  // Decode url-encoded information in the db connection string
  if (isset($username)) {
    $conn_string .= ' user='. $username;
  }
  if (isset($password)) {
    $conn_string .= ' password='. urldecode($password);
  }
  if (isset($host)) {
    $conn_string .= ' host='. urldecode($host);
  }
  if (isset($database_name)) {
    $conn_string .= ' dbname='. $database_name;
  }
  if (isset($url['port'])) {
    $conn_string .= ' port='. $port;
  }
  
  // pg_last_error() does not return a useful error message for database
  // connection errors. We must turn on error tracking to get at a good error
  // message, which will be stored in $php_errormsg.
  $track_errors_previous = ini_get('track_errors');
  ini_set('track_errors', 1);

  $connection = @pg_connect($conn_string);
  if (!$connection) {
    watchdog("Bugbits", $php_errormsg, array(), WATCHDOG_ERROR);
  }

  // Restore error tracking setting
  ini_set('track_errors', $track_errors_previous);

  pg_query($connection, "set client_encoding=\"UTF8\"");
  return $connection;
}

/**
 * Runs a basic query in the active database.
 *
 * @param $query
 *   A string containing an SQL query.
 *
 * @return
 *   A database query result resource, or FALSE if the query was not
 *   executed correctly.
 */
function _bugbits_db_query($query) {
  $query = _bugbits_db_prefix_tables($query);
  return pg_query($query);
}

/**
 * Fetch one result row from the previous query as an object.
 *
 * @param $result
 *   A database query result resource, as returned from _bugbits_db_query().
 * @return
 *   An object representing the next row of the result, or FALSE. The attributes
 *   of this object are the table fields selected by the query.
 */
function _bugbits_db_fetch_object($result) {
  if ($result) {    
    return pg_fetch_object($result);
  }
}

/**
 * Return an individual result field from the previous query.
 *
 * Only use this function if exactly one field is being selected; otherwise,
 * use db_fetch_object() or db_fetch_array().
 *
 * @param $result
 *   A database query result resource, as returned from db_query().
 * @return
 *   The resulting field or FALSE.
 */
function _bugbits_db_result($result) {
  if ($result && pg_num_rows($result) > 0) {
    // The mysql_fetch_row function has an optional second parameter $row
    // but that can't be used for compatibility with Oracle, DB2, etc.
    $array = pg_fetch_row($result);
    return $array[0];
  }
  return FALSE;
}