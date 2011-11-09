<?php
// $Id: webfm_views.inc,v 1.1 2009/09/02 15:10:11 robmilne Exp $



function webfm_views_handlers() {
  return array(
    'handlers' => array(
      // Fields
      'webfm_views_handler_file_name' => array(
        'parent' => 'views_handler_field',
      ),
      'webfm_views_handler_file_meta' => array(
        'parent' => 'views_handler_field',
      ),
    )
  );
}

function webfm_views_data(){
  $data = array();

  /**
  * The webfm_attach table links directly to the node table,
  * via the nid field
  */

  $data['webfm_attach']['table']['group'] = t('WebFM Attachments');
  
  $data['webfm_attach']['table']['join']['node'] = array(
    'left_field' => 'nid',
    'field' => 'nid',
  );
  $data['webfm_attach']['table']['join']['webfm_file'] = array(
    'left_field' => 'fid',
    'field' => 'fid',
  );
  
  
  $data['webfm_attach']['nid'] = array(
      'title' => t('Node ID'),
      'argument' => array(
        'handler' => 'views_handler_argument_numeric',
        'numeric' => TRUE,
        'validate type' => 'nid',
      ),
      'sort' => array('handler' => 'views_handler_sort'),
      'relationship' => array(
        'handler' => 'views_handler_relationship',
        'base' => 'node',
        'field' => 'nid',
        'label' => t('node'),
      ),
    );  
  
  $data['webfm_attach']['fid'] = array(
      'title' => t('File ID'),
      'argument' => array(
        'handler' => 'views_handler_argument_numeric',
        'numeric' => TRUE,
        'validate type' => 'fid',
      ),
      'sort' => array('handler' => 'views_handler_sort'),
    );  
    
  $data['webfm_file']['table']['group'] = t('WebFM Files');
  
  $data['webfm_file']['table']['base'] = array(
    'field' => 'fid',
    'title' => t('WebFM Files')
  );

  $data['webfm_file']['table']['join']['user'] = array(
		'left_field' => 'uid',
    'field' => 'uid',
  );

  $data['webfm_file']['fid'] = array(
      'title' => t('File ID'),
      'argument' => array(
        'handler' => 'views_handler_argument_numeric',
        'numeric' => TRUE,
        'validate type' => 'fid',
      ),
      'sort' => array('handler' => 'views_handler_sort'),
    );
		
		
		$data['webfm_file']['uid'] = array(
      'title' => t('User ID'),
      'argument' => array(
        'handler' => 'views_handler_argument_numeric',
        'numeric' => TRUE,
        'validate type' => 'uid',
      ),
      'sort' => array('handler' => 'views_handler_sort'),
      'relationship' => array(
        'handler' => 'views_handler_relationship',
        'base' => 'user',
        'field' => 'uid',
        'label' => t('user'),
      ),
    );
  
  $data['webfm_file']['file_name'] = array(
    'title' => t('File by Name/Title.'),   
    'field' => array(
      'handler' => 'webfm_views_handler_file_name',
			'option' => array(
        '#type' => 'select',
        '#options' => array(
          'link' => t('With links'),
          'nolink' => t('Without links'))),
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort'
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string'
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string'
    ),
    'help' => t('Display a the file name or title. Metadata title is used where present when \'Display metadata title\' checked in settings.'),
    
  );
  
  $data['webfm_file']['fpath'] = array(
    'title' => t('File path'),   
    'field' => array(
      'handler' => 'views_handler_field',
        'click sortable' => TRUE,
      
     ),
    'sort' => array(
      'handler' => 'views_handler_sort'
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string'
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string'
    ),
    'help' => t('Display the path of the file')  
  );
  
  $data['webfm_file']['fsize'] = array(
    'title' => t('File Size'),   
    'field' => array(
      'handler' => 'views_handler_field_file_size',
        'click sortable' => TRUE,
      
     ),
    'sort' => array(
      'handler' => 'views_handler_sort'
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string'
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string'
    ),
    'help' => t('Display the size of the file')  
  );
  
  $data['webfm_file']['fmime'] = array(
    'title' => t('Mime/Type'),   
    'field' => array(
      'handler' => 'views_handler_field',
        'click sortable' => TRUE,
      
     ),
    'sort' => array(
      'handler' => 'views_handler_sort'
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string'
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string'
    ),
    'help' => t('Display the mime/type of the file')
  );
  
  $data['webfm_file']['fcreatedate'] = array(
    'title' => t('File Creation Date'),   
    'field' => array(
      'handler' => 'views_handler_field_date',
        'click sortable' => TRUE,
      
     ),
    'sort' => array(
      'handler' => 'views_handler_sort_date'
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date'
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_date'
    ),
    'help' => t('Display the date of files that have been attached to a node via WebFM')
  );
  
  $data['webfm_file']['ftitle'] = array(
    'title' => t('Title'),   
    'field' => array(
      'handler' => 'webfm_views_handler_file_meta',
        'click sortable' => TRUE,
      
     ),
    'sort' => array(
      'handler' => 'views_handler_sort'
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string'
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string'
    ),
    'help' => t('Display the file title of files that have been attached to a node via WebFM (with or without links to the files themselves)')
  );
  
  $data['webfm_file']['fdesc'] = array(
    'title' => t('File Description'),   
    'field' => array(
      'handler' => 'webfm_views_handler_file_meta',
        'click sortable' => TRUE,
      
     ),
    'sort' => array(
      'handler' => 'views_handler_sort'
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string'
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string'
    ),
    'help' => t('Display the description of files that have been attached to a node via WebFM')
  );
  
  $data['webfm_file']['flang'] = array(
    'title' => t('File Language'),   
    'field' => array(
      'handler' => 'webfm_views_handler_file_meta',
        'click sortable' => TRUE,
      
     ),
    'sort' => array(
      'handler' => 'views_handler_sort'
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string'
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string'
    ),
    'help' => t('Display the language of files that have been attached to a node via WebFM')
  );
  
  $data['webfm_file']['fpublisher'] = array(
    'title' => t('File Publisher'),   
    'field' => array(
      'handler' => 'webfm_views_handler_file_meta',
        'click sortable' => TRUE,
      
     ),
    'sort' => array(
      'handler' => 'views_handler_sort'
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string'
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string'
    ),
    'help' => t('Display the publisher of files that have been attached to a node via WebFM')
  );
  
  $data['webfm_file']['dl_cnt'] = array(
    'title' => t('Download Count'),   
    'field' => array(
      'handler' => 'views_handler_field',
        'click sortable' => TRUE,
      
     ),
    'sort' => array(
      'handler' => 'views_handler_sort'
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string'
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string'
    ),
    'help' => t('Display the publisher of files that have been attached to a node via WebFM')
  );
  
  
  
  
  
  
 

//                           'fields' => array(
//                                   'all_files' => array(
//                                     'name' => t('WebFM: All files by name/title.'),
//                                     'notafield' => true,
//                                     'handler' => 'webfm_views_handler_file_all_files',
//                                      'option' => array(
//                                        '#type' => 'select',
//                                        '#options' => array(
//                                          'link' => t('With links'),
//                                          'nolink' => t('Without links'))),
//                                      'sortable' => false,
//                                      'help' => t('Display ALL the files that have been attached to the node via WebFM. Metadata title is used where present when \'Display metadata title\' checked in settings.')),
//                                    'fpath' => array(
//                                      'name' => t('WebFM: File name'),
//                                      'handler' => array (
//                                        'webfm_views_handler_file_filename_download' => t('With links'),
//                                        'webfm_views_handler_file_filename' => t('Without links')),
//                                      'sortable' => true,
//                                      // consider 'option' --> link inline - or link force download - extra arg for webfm send
//                                      'addlfields' => array('fid'),
//                                      'help' => t('Display the name of files that have been attached to a node via WebFM (with or without links to the files themselves)')),
//                                    'fsize' => array(
//                                      'name' => t('WebFM: File size'),
//                                      'sortable' => true,
//                                      'handler' => 'webfm_views_handler_file_size',
//                                      'help' => t('Display the file size of files that have been attached to a node via WebFM')),
//                                    'fmime' => array(
//                                      'name' => t('WebFM: File mime type'),
//                                      'sortable' => true,
//                                      'help' => t('Display the file mime type of files that have been attached to a node via WebFM')),
//                                    'ftitle' => array(
//                                      'name' => t('WebFM metadata:  File title'),
// 				                              'handler' => array (
// 					                               'webfm_views_handler_file_meta_download' => t('With links'),
// 					                               'webfm_views_handler_file_meta' => t('Without links')),
//                                      'sortable' => true,
// 				                              'addlfields' => array('fid'),
//                                      'help' => t('Display the file title of files that have been attached to a node via WebFM (with or without links to the files themselves)')),
//                                    'fdesc' => array(
//                                      'name' => t('WebFM metadata:  File description'),
//                                      'sortable' => true,
//                                      'handler' => 'webfm_views_handler_file_meta',
//                                      'help' => t('Display the description of files that have been attached to a node via WebFM')),
//                                    'flang' => array(
//                                      'name' => t('WebFM metadata:  File language'),
//                                      'sortable' => true,
//                                      'handler' => 'webfm_views_handler_file_meta',
//                                      'help' => t('Display the language of files that have been attached to a node via WebFM')),
//                                    'fpublisher' => array(
//                                      'name' => t('WebFM metadata:  File publisher'),
//                                      'sortable' => true,
//                                      'handler' => 'webfm_views_handler_file_meta',
//                                      'help' => t('Display the publisher of files that have been attached to a node via WebFM')),
//                                    'dl_cnt' => array(
//                                      'name' => t('WebFM:  Download Count'),
//                                      'sortable' => true,
//                                      'help' => t('Display the number of times a file has been downloaded via WebFM'))),
//                                  'filters' => array(
//                                    'fpath' => array(
//                                      'name' => t('WebFM: File name'),
//                                      'operator' => 'views_handler_operator_like',
//                                      'handler' => 'views_handler_filter_like',
//                                      'help' => t('Filter nodes by the names of files that have been attached via WebFM.  e.g. show only nodes with a file called "foo.txt" attached to them')),
//                                    'fsize' => array(
//                                      'name' => t('WebFM: File size'),
//                                      'operator' => 'views_handler_operator_gtlt',
//                                      'help' => t('Filter nodes by the size of files that have been attached via WebFM. e.g. show only nodes with files larger than 1M attached to them')),
//                                    'fmime' => array(
//                                      'name' => t('WebFM: File mime type'),
//                                      'operator' => 'views_handler_operator_like',
//                                      'handler' => 'views_handler_filter_like',
//                                      'help' => t('Filter nodes by the mime type of files that have been attached via WebFM.  e.g. show only nodes with a .pdf document attached to them')),
//                                    'ftitle' => array(
//                                      'name' => t('WebFM metadata: File title'),
//                                      'operator' => 'views_handler_operator_like',
//                                      'handler' => 'views_handler_filter_like',
//                                      'help' => t('Filter nodes by the title of files that have been attached via WebFM.  e.g. show only nodes with a file attached wich has a metadata title like "bar"')),
//                                    'fdesc' => array(
//                                      'name' => t('WebFM metadata: File description'),
//                                      'operator' => 'views_handler_operator_like',
//                                      'handler' => 'views_handler_filter_like',
//                                      'help' => t('Filter nodes by the description of files that have been attached via WebFM.  e.g. show only nodes with a file attached which have a metadata description which contains all the words "foo, bar and baz"')),
//                                    'flang' => array(
//                                      'name' => t('WebFM metadata: File language'),
//                                      'operator' => 'views_handler_operator_like',
//                                      'handler' => 'views_handler_filter_like',
//                                      'help' => t('Filter nodes by the language of files that have been attached via WebFM.  e.g. show only nodes with a file attached which have a metadata language equal to "fr"')),
//                                    'fpublisher' => array(
//                                      'name' => t('WebFM metadata: Sort by file publisher'),
//                                      'operator' => 'views_handler_operator_like',
//                                      'handler' => 'views_handler_filter_like',
//                                      'help' => t('Filter nodes by publisher of files that have been attached via WebFM.')),
//                                    'dl_cnt' => array(
//                                      'name' => t('WebFM:  Download Count'),
//                                      'operator' => 'views_handler_operator_like',
//                                      'handler' => 'views_handler_filter_like',
//                                      'help' => t('Filter nodes by downloaded count of files that have been attached via WebFM.'))),
//                                  'sorts' => array(
//                                    'fpath' => array(
//                                      'name' => t('WebFM: Sort by file name'),
//                                      'help' => t('Sort by file name')),
//                                    'fsize' => array(
//                                      'name' => t('WebFM: Sort by file size'),
//                                      'help' => t('Sort by file size.')),
//                                    'fmime' => array(
//                                      'name' => t('WebFM: Sort by mime type'),
//                                      'help' => t('Sort by mime type.')),
//                                    'ftitle' => array(
//                                      'name' => t('WebFM metadata: Sort by file title'),
//                                      'help' => t('Sort by file metadata title')),
//                                    'flang' => array(
//                                      'name' => t('WebFM metadata: Sort by file language'),
//                                      'help' => t('Sort by file language.')),
//                                    'fpublisher' => array(
//                                      'name' => t('WebFM metadata: Sort by file publisher'),
//                                      'help' => t('Sort by file publisher.')),
//                                    'dl_cnt' => array(
//                                      'name' => t('WebFM: Download Count'),
//                                      'help' => t('Sort by file downloaded count')))
//                                );
  return $data;
}