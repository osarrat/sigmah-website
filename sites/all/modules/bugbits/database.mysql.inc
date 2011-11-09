<?php
// $Id: database.mysql.inc,v 1.1.2.3 2009/02/04 01:59:00 daften Exp $

/**
 * @file
 * Mantis database interface code for MySQL database servers.
 */

function _bugbits_db_connect($username, $password, $database_name, $host = 'localhost', $port = NULL) {
  // Check if MySQL support is present in PHP
  if (!function_exists('mysql_connect')) {
    watchdog("Bugbits", "Unable to use the MySQL database (for Mantis) because the MySQL extension for PHP is not installed. Check your <code>php.ini</code> to see how you can enable it.", array(), WATCHDOG_ERROR);
  }

  // Allow for non-standard MySQL port.
  if (isset($port)) {
    $host = $host .':'. $port;
  }

  // - TRUE makes mysql_connect() always open a new link, even if
  //   mysql_connect() was called before with the same parameters.
  //   This is important if you are using two databases on the same
  //   server.
  // - 2 means CLIENT_FOUND_ROWS: return the number of found
  //   (matched) rows, not the number of affected rows.
  $connection = @mysql_connect($host, $username, $password, TRUE, 2);
  if (!$connection || !mysql_select_db($database_name)) {
    // Show error screen otherwise
    watchdog("Bugbits", mysql_error(), array(), WATCHDOG_ERROR);
  }

  // Force UTF-8.
  mysql_query('SET NAMES "utf8"', $connection);
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
  return mysql_query($query);
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
    return mysql_fetch_object($result);
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
  if ($result && mysql_num_rows($result) > 0) {
    // The mysql_fetch_row function has an optional second parameter $row
    // but that can't be used for compatibility with Oracle, DB2, etc.
    $array = mysql_fetch_row($result);
    return $array[0];
  }
  return FALSE;
}