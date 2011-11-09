<?php
/**
 * @file
 * Page handlers for deploy logs admin.
 */

/**
 * Display a list of deployments that have taken place.
 */
function deploy_logs_overview() {
  $result = pager_query("SELECT * FROM {deploy_log} ORDER BY ts DESC", 20);
  while ($log = db_fetch_array($result)) {
    $row = array(
      l($log['plan'], 'admin/build/deploy/logs/details/'. $log['dlid']),
      $log['username'],
      format_date($log['ts'], 'small'),
      $log['server'],
    );
    $rows[] = $row;
  }

  if (empty($rows)) {
    $rows[] = array(array('data' => t('No deployment plans have been pushed.'), 'colspan' => '4', 'class' => 'message'));
  }

  $header = array(
    t('Plan'),
    t('Pushed By'),
    t('When'),
    t('To Server')
  );

  $output = theme('table', $header, $rows);
  $output .= theme('pager');

  return $output;
}

/**
 * Display the details of a single deployment.
 *
 * @param $dlid
 *   Unique identifier for the deployment log details we are displaying.
 */
function deploy_logs_details($dlid) {
  $rows = array();
  $styles = array(
    'Error' => 'background-color: #fcc',
    'Not Sent' => 'background-color: #ffd',
    'Success' => '',
  );

  $result = db_query("SELECT * FROM {deploy_log_details} WHERE dlid = %d", $dlid);
  while ($log_detail = db_fetch_array($result)) {
    $row = array(
      'data' => array(
        $log_detail['module'],
        $log_detail['description'],
        $log_detail['result'],
        $log_detail['message'],
      ),
      'style' => $styles[$log_detail['result']],
    );
    $rows[] = $row;
  }

  $header = array(
    t('Module'),
    t('Description'),
    t('Result'),
    t('Message')
  );

  return theme('table', $header, $rows);
}

