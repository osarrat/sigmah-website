<?php
// $Id: taxonomy_access.views.inc,v 1.1.2.1 2010/09/03 21:25:55 xjm Exp $

/**
 * @file
 * Provides support for the Views module.
 */

/**
 * Implements hook_views_data().
 */
function taxonomy_access_views_data() {
  $data = array();

	$data['node']['taxonomy_access_view'] = array(
    'title' => t('Taxonomy Access: grant view'),
    'help' => t('Current user has Taxonomy Access Control view permission.'),
	  'filter' => array(
      'handler' => 'taxonomy_access_handler_filter_view',
    ),
  );

	$data['node']['taxonomy_access_update'] = array(
    'title' => t('Taxonomy Access: grant update'),
    'help' => t('Current user has Taxonomy Access Control update permission.'),
	  'filter' => array(
      'handler' => 'taxonomy_access_handler_filter_update',
    ),
  );

	$data['node']['taxonomy_access_delete'] = array(
    'title' => t('Taxonomy Access: grant delete'),
    'help' => t('Current user has Taxonomy Access Control delete permission.'),
	  'filter' => array(
      'handler' => 'taxonomy_access_handler_filter_delete',
    ),
  );

	$data['node']['taxonomy_access_create'] = array(
    'title' => t('Taxonomy Access: grant create'),
    'help' => t('Current user has Taxonomy Access Control create permission.'),
	  'filter' => array(
      'handler' => 'taxonomy_access_handler_filter_create',
    ),
  );

	$data['node']['taxonomy_access_list'] = array(
    'title' => t('Taxonomy Access: grant list'),
    'help' => t('Current user has Taxonomy Access Control list permission.'),
	  'filter' => array(
      'handler' => 'taxonomy_access_handler_filter_list',
    ),
  );

  return $data;
}

/**
 * Implements hook_views_handlers().
 */
function taxonomy_access_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'taxonomy_access') . '/includes',
    ),
    'handlers' => array(
      'taxonomy_access_handler_filter_view' => array(
        'parent' => 'views_handler_filter',
      ),
      'taxonomy_access_handler_filter_update' => array(
        'parent' => 'views_handler_filter',
      ),
      'taxonomy_access_handler_filter_delete' => array(
        'parent' => 'views_handler_filter',
      ),
      'taxonomy_access_handler_filter_create' => array(
        'parent' => 'views_handler_filter',
      ),
      'taxonomy_access_handler_filter_list' => array(
        'parent' => 'views_handler_filter',
      ),
    ),
  );
}
