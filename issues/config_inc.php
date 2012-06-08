<?php
	$g_hostname = '127.0.0.1';
	$g_db_type = 'mysql';
	$g_database_name = 'mantis';
	$g_db_username = 'root';
	$g_db_password = '';
	
	# --------------------
	# For anonymous login
	# --------------------
	$g_allow_anonymous_login = ON;
	$g_anonymous_account = 'anonymous';
	 
	# Optionally, if you want to use blank email addresses
	$g_allow_blank_email = ON;
	# --------------------

	/*****************************
	 * MantisBT Display Settings *
	 *****************************/

	/**
	 * browser window title
	 * @global string $g_window_title
	 */
	$g_window_title			= 'Sigmah Issue Tracker';
	
	/**
	 * Logo
	 * @global string $g_logo_image
	 */
	//Comment this line the day we want the original Mantis logo back
	$g_logo_image			= 'images/sigmah-logo-Mantis.png';
	
	
	/***************************
	 * MantisBT Email Settings *
	 ***************************/

	/**
	 * Administrator Email address
	 * @global string $g_administrator_email
	 */
	$g_administrator_email	= 'admin@sigmah.org';

	/**
	 * Webmaster email
	 * @global string $g_webmaster_email
	 */
	$g_webmaster_email		= 'webmaster [at] sigmah (dot) org';

	/**
	 * the sender email, part of 'From: ' header in emails
	 * @global string $g_from_email
	 */
 	$g_from_email			= 'noreply@sigmah.org';

	/**
	 * the sender name, part of 'From: ' header in emails
	 * @global string $g_from_name
	 */
	$g_from_name			= 'Sigmah Issue Tracker';

	/**
	 * the return address for bounced mail
	 * @global string $g_return_path_email
	 */
	$g_return_path_email	= 'admin@sigmah.org';
	
		
	# --------------------
	# For SVN integration
	# --------------------
	
	/**
	 * Regular expression used to detect issue ids within checkin comments.
	 * see preg_match_all() documentation at
	 * http://www.php.net/manual/en/function.preg-match-all.php
	 * @global string $g_source_control_regexp
	 */
	$g_source_control_regexp = "/\bissue [#]{0,1}(\d+)\b/i";
	
	/**
	 * Regular expression used to detect the fact that an issue is fixed and extracts
	 * its issue id.  If there is a match to this regular expression, then the issue
	 * will be marked as resolved and the resolution will be set to fixed.
	 * @global string $g_source_control_fixed_regexp
	 */
	$g_source_control_fixed_regexp = "/\bfixes issue [#]{0,1}(\d+)\b/i";
	
	/**
	 * If set to a status, then after a checkin with a log message that matches the regular expression in
	 * $g_source_control_fixed_regexp, the issue status is set to the specified status.  If set to OFF, the
	 * issue status is not changed.
	 * @global int $g_source_control_set_status_to
	 */
	$g_source_control_set_status_to     = RESOLVED;

	/**
	 * Whenever an issue status is set to $g_source_control_set_status_to, the issue resolution is set to
	 * the value specified for this configuration.
	 * @global int $g_source_control_set_resolution_to
	 */
	$g_source_control_set_resolution_to = FIXED;

	/**
	 * Account to be used by the source control script.  The account must be enabled
	 * and must have the appropriate access level to add notes to all issues even
	 * private ones (DEVELOPER access recommended).
	 * @global string $g_source_control_account
	 */
	$g_source_control_account           = 'svn_mantis-robot';
	
	/**
	 * For open source projects it is expected that the notes be public, however,
	 * for non-open source it will probably be VS_PRIVATE.
	 * @global int $g_source_control_notes_view_status
	 */
	$g_source_control_notes_view_status = VS_PUBLIC;
	
	
	
	// Google code secret key
	$g_source_control_googlecode_secretkey = "googlecodesecretkey";
	
	// Google code project URL
	$g_source_control_googlecode_project_url = "http://code.google.com/p/sigma-h/";
	
	# --------------------
	
		
	/******************
	 * OpenID via rpxnow.com (https://rpxnow.com/)
	 *******************/

	/**
	 * Enable/disable open id support.
	 */
	$g_openid_enabled = ON;
	
	/**
	 * The RpxNow API key for the site.  Note that each site should be registered separately
	 * and get its own api key, otherwise, user logins will be mixed up.  This is because the
	 * mapping between the open ids and MantisBT database id is stored in rpxnow.
	 */
	$g_openid_api_key = 'openidapikey0openidapikey0openidapikey01';
  

	/**
	 * The name of the site that is registered with rpxnow.
	 */
	$g_openid_site_name = 'sigmah';


	/**
	 * Used to disable the SSL verification if rpxnow ssl certificate is not valid.
	 */
        //SSL Verification removed because *.rpxnow.com site returns following error:
        // Error performing HTTP request: SSL certificate problem, verify that the CA cert is OK. Details: error:14090086:SSL routines:SSL3_GET_SERVER_CERTIFICATE:certificate verify failed	
	$g_openid_ssl_verification_disabled = TRUE;
	
?>