<?php

# GoogleCode - MantisBT interaction on SVN commit
# GPL 2 or later
#
# A big part of this code comes from script/checkin.php
#
# TODO:
# - make it work with Hg too,
# - enhance MD5-HASH checking.


# include MantisBT core
global $g_bypass_headers;
$g_bypass_headers = 1;
require_once( dirname ( dirname( __FILE__ ) ) . DIRECTORY_SEPARATOR . 'core.php' );


# Check that the username is set and exists
$t_username = config_get( 'source_control_account' );
if( is_blank( $t_username ) || ( user_get_id_by_name( $t_username ) === false ) ) {
  header('HTTP/1.0 500 Inexistent MantisBT account');
  echo "ERROR&nbsp;: Inexistent source control account ('$t_username').\n";
  exit( 1 );
}

# Login as source control user
if( !auth_attempt_script_login( $t_username ) ) {
  header('HTTP/1.0 500 Invalid MantisBT user');
  echo "ERROR&nbsp;: Invalid MantisBT user.";
  exit( 1 );
}

# Detect references to issues + concat all lines to have the comment log.
$t_commit_regexp = config_get( 'source_control_regexp' );
$t_commit_fixed_regexp = config_get( 'source_control_fixed_regexp' );
$t_comment_footer = '';

# Retrieve the JSON send by GoogleCode
$data_raw = file_get_contents('php://input');
if ($_SERVER['HTTP_GOOGLE_CODE_PROJECT_HOSTING_HOOK_HMAC'] != hash_hmac('md5', $data_raw, config_get( 'source_control_googlecode_secretkey' ))) {
  # TODO : the check hash-md5 should be more restrictive and discreet (a log ?)
  $t_comment_footer .= '[invalid hash : if the secret key has been configured, this message could be a fake]';
}

if ( ! empty($data_raw) and $data = json_decode($data_raw, true)) {
  # Loop over each revisions in the json data
  foreach($data['revisions'] as $rev) {
    $t_comment = $rev['message'];
    $t_issues = array();
    $t_fixed_issues = array();

    # Run the regexp
    if( preg_match_all( $t_commit_regexp, $rev['message'], $t_matches ) ) {
      $t_count = count( $t_matches[0] );
      for( $i = 0;$i < $t_count;++$i ) {
        $t_issues[] = $t_matches[1][$i];
      }
    }

    # Run the "fixed" regexp
    if( preg_match_all( $t_commit_fixed_regexp, $rev['message'], $t_matches ) ) {
      $t_count = count( $t_matches[0] );
      for( $i = 0;$i < $t_count;++$i ) {
        $t_fixed_issues[] = $t_matches[1][$i];
      }
    }

    # Add info about modifications (files added, removed, ...)
    $t_comment .= "\n\n";
    foreach (array('added', 'modified', 'removed') as $t_action) {
      if ( ! empty($rev[$t_action])) {
        foreach ($rev[$t_action] as $t_file) {
          $t_comment .= ucfirst($t_action) . ' : ' . $t_file . "\n";
        }
      }
    }

    # Add the footer (TODO: we use this as a warning for bad hash. Maybe not a good idea.)
    $t_comment .= $t_comment_footer;

    if ( count($t_issues) > 0 or count($t_fixed_issues) > 0 ) {
      # history parameters are reserved for future use.
      $t_history_old_value = '';
      $t_history_new_value = '';

      # add note to each bug only once
      $t_issues = array_unique( $t_issues );
      $t_fixed_issues = array_unique( $t_fixed_issues );

      # Call the custom function to register the checkin on each issue.

      foreach( $t_issues as $t_issue_id ) {
        if( !in_array( $t_issue_id, $t_fixed_issues ) ) {
          helper_call_custom_function( 'checkin', array( $t_issue_id, $t_comment, $t_history_old_value, $t_history_new_value, false ) );
        }
      }

      foreach( $t_fixed_issues as $t_issue_id ) {
        helper_call_custom_function( 'checkin', array( $t_issue_id, $t_comment, $t_history_old_value, $t_history_new_value, true ) );
      }
    }
  }
}

exit( 0 );

