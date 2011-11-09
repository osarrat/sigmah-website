<?php
// $Id: webfm_ipfolder.admin.inc,v 1.2 2009/08/20 01:10:35 robmilne Exp $
/* vim: set expandtab tabstop=4 shiftwidth=4 softtabstop=4: */
/**
 * @author     Niels Hackius <niels.hackius@tu-harburg.de>
 * @copyright 2009, Technische Universität Hamburg-Harburg: Institut für Logistik und Unternehmensführung
 * @license    http://www.opensource.org/licenses/gpl-3.0.html  GNU General Public License version 3 (GPLv3)
 * @version   $Id: webfm_ipfolder.admin.inc,v 1.2 2009/08/20 01:10:35 robmilne Exp $
 * @since      12:01:33 15.04.2009
 *
 */

/**
 * Allows managing IP-Ranges, Single IPs and associated folders
 *
 * @return array system_settings_form
 */
function webfm_ipfolder_ips()
{
 $form['ip_list'] = array(
    '#type' => 'fieldset',
    '#title' => t('Current IP authenticated folders'),
    '#description' => "<b>". t("Instructions:") ."</b><br/> &nbsp; - ".
                      t("Click on an ip range to modify the authenticator assignment.") ."<br/> &nbsp; - ".
                      t("The title fields are sortable.") ."<br/><br/>".
                      "<b>". t("Your IP: @s", array('@s' => $_SERVER['REMOTE_ADDR'])) ."</b>".
                      '<br /><br/>'.
                      t('Please note that users with the <em>administer webfm</em> rights will be able to download the files anyway.'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#weight' => 0
  );

    //create beforehande for usage with tablesort_sql()
	$header = array();
	$header[] = array("data" => t("IP Range"), "field" => "ip1");
	$header[] = array("data" => t("Folder"), "field" => "folder","sort"=>"asc");
	$res = db_query("SELECT id,ip1,ip2,folder FROM {webfm_ipfolder} ".tablesort_sql($header));

  if (db_affected_rows() > 0) {

  	while($data = db_fetch_object($res)) {
  		//this takes the mysql command, INET_NTOA out of the query string.
      $data->ip1 = long2ip($data->ip1);
      $data->ip2 = ($data->ip2 == 0) ? "" : long2ip($data->ip2);
      $ip2_text = $data->ip2 ? " - ". $data->ip2 : "";
      $ip_title = $data->ip1 . $ip2_text;

      $row = array();
      $row['iprange'] = l($ip_title, "admin/settings/webfm_ipfolder/edit/". $data->id);
      $row['folder'] = l($data->folder,"admin/settings/webfm_ipfolder/edit/". $data->id);
      $row['del'] = l(t('Delete'),"admin/settings/webfm_ipfolder/delete/". $data->id);
      $rows[] = array("data" => $row);
  	}

  	$form['ip_list']['ipfoldertable'] = array(
  		'#type' => 'item',
  		'#value' => theme('table', $header, $rows, array ("class" => "container-inline")));
  }
  else {
    $form['ip_list']['notfound'] = array(
      '#value' => t("There are no folders or IPs to list."),
    );
  }
	$form['conf'] = array (
		'#type' => 'fieldset',
		'#title' => t('WebFile manager IP based folder access'),
		'#collapsible' => false,
		'#collapsed' => false,
		'#required' => false,
	);

	$form['conf']['ip1'] = array (
		'#type' => 'textfield',
		'#title' => t('Start of IP range').' ip1',
		'#size' => 15,
		'#description' => t('It is not possible to use wildcards.'),
	);
	$form['conf']['ip2'] = array (
		'#type' => 'textfield',
		'#title' => t('End of IP range').' ip2',
		'#size' => 15,
		'#description' => t('It is not possible to use wildcards.').' '.t('Leave empty for single IP matching of ip1'),
	);
	$form['conf']['folder'] = array (
		'#type' => 'textfield',
		'#title' => t('Name of folder'),
		'#size' => 15,
		'#description' => t('Provide folder name as it is provided through webfm.'),
		);

	$form['#validate'][] = 'webfm_ipfolder_ips_validate';
  	$form['#submit'][] = 'webfm_ipfolder_ips_submit';
	return system_settings_form($form);
}


/**
 * Deletes an IP- folder Association
 */
function webfm_ipfolder_ips_delete()
{
	$args = func_get_args();
	$id = $args[1];
	if(!is_numeric($id)) {
		drupal_set_message(t("Please select your IP-Range again"));
		drupal_goto('admin/settings/webfm_ipfolder');
		return;
	}

	db_query("DELETE FROM {webfm_ipfolder} WHERE id = '%d' ",$id);
	if(db_affected_rows() == 0) {
		drupal_set_message(mysql_error(), 'error');
	}
	drupal_goto('admin/settings/webfm_ipfolder');
}


/**
 * Allows to edit an IP- folder Association
 *
 * @return array system_settings_form
 */
function webfm_ipfolder_ips_edit()
{

	$args = func_get_args();
	$id = $args[1];
	if(!is_numeric($id)) {
		drupal_set_message(t("Please select your IP-Range again"));
		drupal_goto('admin/settings/webfm_ipfolder');
		return;
	}

	$result = db_query("SELECT * FROM {webfm_ipfolder} WHERE id = '%d' LIMIT 1",$id);
	if(!$result) drupal_goto('admin/settings/webfm_ipfolder');

	$data = db_fetch_object($result);

	$form['conf'] = array (
		'#type' => 'fieldset',
		'#title' => t('WebFile manager IP based folder access'),
		'#collapsible' => false,
		'#collapsed' => false,
		'#required' => false,
	);

	$form['conf']['ip1'] = array (
		'#type' => 'textfield',
		'#title' => t('Start of IP range'),
		'#size' => 15,
		'#description' => t('It is not possible to use wildcards.'),
		'#default_value' => long2ip($data->ip1),
	);
	$form['conf']['ip2'] = array (
		'#type' => 'textfield',
		'#title' => t('End of IP range'),
		'#size' => 15,
		'#description' => t('It is not possible to use wildcards.'),
		'#default_value' => long2ip($data->ip2),
	);
	$form['conf']['folder'] = array (
		'#type' => 'textfield',
		'#title' => t('Name of folder'),
		'#size' => 15,
		'#description' => t('Provide folder name as it is provided through webfm.'),
		'#default_value' => $data->folder,
	);
	$form['id'] = array (
		'#type' => 'value',
		'#value' => $id,
	);

	$form['#validate'][] = 'webfm_ipfolder_ips_validate';
  	$form['#submit'][] = 'webfm_ipfolder_ips_submit_edit';
  	$form['#redirect'][] = 'admin/settings/webfm_ipfolder';
	return system_settings_form($form);
}


/**
 * hook_validate for @see{webfm_ipfolder_ips}
 * Uses form_set_error if IP Adress isn't proper or folder-field is empty
 *
 * @param $form_id
 * @param $form_state
 */
function webfm_ipfolder_ips_validate($form_id, &$form_state)
{
	// IF IP1 contains a value but doesn't match the IP address format, then display an error.
	$form_state['values']['ip1'] = trim($form_state['values']['ip1'],".");
  	$form_state['values']['ip2'] = trim($form_state['values']['ip2'],".");
	if ($form_state['values']['ip1'] == "" OR !preg_match("/^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\$/x", $form_state['values']['ip1'])) {
		form_set_error('ip1', t("You must enter an ip address in the first field."));
  	}

  	if (trim($form_state['values']['ip2']) != "" AND !preg_match("/^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\$/x", $form_state['values']['ip2'])) {
  		form_set_error('ip2', t("You must enter an ip address in the second field or leave it empty."));
  	}

  	if(trim($form_state['values']['folder']) == "") {
  		form_set_error('folder', t("You must enter a folder."));
  	}
}


/**
 * hook_submit for @see{webfm_ipfolder_ips}
 * Inserts IP ranges and folder
 *
 * @param $form_id
 * @param $form_state
 * @param $op
 */
function webfm_ipfolder_ips_submit($form_id, &$form_state,$op="insert")
{
	$ip2 = sprintf('%u',ip2long(trim($form_state['values']["ip2"],".")));
	$ip1 = sprintf('%u',ip2long(trim($form_state['values']["ip1"],".")));

	if(!empty($ip2) AND $ip2 < $ip1) {
		$b = $ip1;
		$ip1 = $ip2;
		$ip2 = $b;
	}

	$folder = trim($form_state['values']["folder"],"/\\*");

	if($op == "update" AND !empty($form_state['values']["id"])) {
		db_query("UPDATE {webfm_ipfolder} SET ip1 = '%s',ip2 = '%s',folder ='%s' WHERE id='%d'",$ip1,$ip2,$folder,$form_state['values']["id"]);

	} else { //insert
		db_query("INSERT INTO {webfm_ipfolder} (ip1,ip2,folder) VALUES('%s','%s','%s')",$ip1,$ip2,$folder);

	}

	if(db_affected_rows() == 0) {
		drupal_set_message(mysql_error(), 'error');
	}

}


/**
 * hook_submit for @see{webfm_ipfolder_ips_edit}
 *
 * @param $form_id
 * @param $form_state
 */
function webfm_ipfolder_ips_submit_edit($form_id, &$form_state)
{
	webfm_ipfolder_ips_submit($form_id, $form_state,"update");
}

/**
 * Provides to adjust general settings: The default behavior
 *
 * @return array system_settings_form
 */
function webfm_ipfolder_settings()
{
	$form['conf'] = array (
		'#type' => 'fieldset',
		'#title' => t('WebFile manager IP based folder access'),
		'#collapsible' => false,
		'#collapsed' => false,
		'#required' => false,
	);
	$form['conf']['webfm_ipfolder_bydefault'] = array (
		'#type' => 'radios',
		'#title' => t('Default behavior'),
  		'#default_value' => variable_get('webfm_ipfolder_bydefault', false),
  		'#options' => array(t('Block access to all folders, but not to IP/folder-Pairs specifed.'), t('Allow access to all folders, but restrict access for the IP/folder-Pairs as specified.')),
	);

	return system_settings_form($form);
}
