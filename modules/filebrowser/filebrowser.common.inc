<?php
// $Id: filebrowser.common.inc,v 1.1.2.15 2010/06/28 13:41:22 ulhume Exp $


/* This file is part of "filebrowser".
 *    Copyright 2009, arNuméral
 *    Author : Yoran Brault
 *    eMail  : yoran.brault@bad_arnumeral.fr (remove bad_ before sending an email)
 *    Site   : http://www.arnumeral.fr
 *
 * "filebrowser" is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * "filebrowser" is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with "filebrowser"; if not, write to the Free
 * Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA, or see the FSF site: http://www.fsf.org.
 */

/**
 * @file
 * 
 * Misc filebrowser common functions. 
 */

define('FILEBROWSER_CREATE_DIRECTORY_LISTING', 'create directory listings');
define('FILEBROWSER_DELETE_OWN_DIRECTORY_LISTINGS', 'delete own directory listings');
define('FILEBROWSER_DELETE_ANY_DIRECTORY_LISTINGS', 'delete any directory listings');
define('FILEBROWSER_EDIT_OWN_DIRECTORY_LISTINGS', 'edit own directory listings');
define('FILEBROWSER_EDIT_ANY_DIRECORY_LISTINGS', 'edit any directory listings');
define('FILEBROWSER_VIEW_DIRECORY_LISTINGS', 'view directory listings');
define('FILEBROWSER_UPLOAD', 'upload files');
define('FILEBROWSER_DOWNLOAD_ARCHIVE', 'download archive files');
define('FILEBROWSER_DOWNLOAD', 'download files');

/**
 * Column indentifiers definition.
 */
define('FILEBROWSER_DATA_NAME_ICON', 'icon');
define('FILEBROWSER_DATA_NAME_DISPLAY_NAME', 'display-name');
define('FILEBROWSER_DATA_NAME_SIZE', 'size');
define('FILEBROWSER_DATA_NAME_CREATED', 'created');
define('FILEBROWSER_DATA_NAME_MODIFIED', 'modified');
define('FILEBROWSER_DATA_NAME_TYPE', 'type');
define('FILEBROWSER_DATA_NAME_DESCRIPTION', 'description');

/**
 * Compare two sizes (used as usort callback).
 */
function _filebrowser_sorter_size($a, $b) {
  if ($a['size'] == $b['size']) {
    return 0;
  }
  return ($a['size'] > $b['size']) ? -1 : 1;
}

/**
 * Compare two creation dates (used as usort callback).
 */
function _filebrowser_sorter_created($a, $b) {
  if ($a['created'] == $b['created']) {
    return 0;
  }
  return ($a['created'] > $b['created']) ? -1 : 1;
}

/**
 * Compare two modification dates (used as usort callback).
 */
function _filebrowser_sorter_modified($a, $b) {
  if ($a['modified'] == $b['modified']) {
    return 0;
  }
  return ($a['modified'] > $b['modified']) ? -1 : 1;
}

/**
 * Compare two display names (used as usort callback).
 */
function _filebrowser_sorter_display_name($a, $b) {
  return -strcasecmp($a['display-name'], $b['display-name']);
}

/**
 * Compare file types (used as usort callback).
 */
function _filebrowser_sorter_type($a, $b) {
  return -strcasecmp($a['mime-type'], $b['mime-type']);
}

/**
 * Compare two creation descriptions (used as usort callback).
 */
function _filebrowser_sorter_description($a, $b) {
  return -strcasecmp($a['description'], $b['description']);
}

/**  Check the end of a string
 * @param $str source string
 * @param $sub element to search
 * @return return TRUE is a string ends with another string.
 */
function _filebrowser_ends_with($str, $sub) {
  return (substr($str, strlen($str) - strlen($sub)) == $sub);
}

/** Helper to get the full path of a dir_listing node
 * @param $node source node
 * @return the full path
 */
function _filebrowser_current_full_path($node) {
  return _filebrowser_get_node_file_path($node) . $node->relative_path;
}

/**
 * Helper function to get node folder with correct token replacements. Never
 * use $node->folder_path directly !!!
 * @param $node
 * @return folder path
 */
function _filebrowser_get_node_file_path(& $node) {
  $result = $node->folder_path;
  if (module_exists("token")) {
    $result = token_replace($result, $type = 'global', $object = NULL, $leading = '[', $trailing = ']');
  }
  return $result;
}

function _filebrowser_prepare_record(& $node, $load = TRUE) {
  //  echo "<pre>";
  //  var_dump($node);
  //  echo "</pre>";	
  // Fix trailing slashes
  $node->folder_path = rtrim($node->folder_path, '/');
  
  // Process uploads
  $node->folder_uploads = (object)$node->folder_uploads;
  _filebrowser_set_default($node->folder_uploads->enabled, FALSE);
  _filebrowser_set_default($node->folder_uploads->allow_overwrite, FALSE);
  _filebrowser_set_default($node->folder_uploads->accepted_uploaded_files, '');
  
  // Process handlers
  $node->folder_uploads = (object)$node->folder_uploads;
  foreach (module_implements("filebrowser_handler_info") as $module) {
    if (isset($node->$module)) {
      $node->file_handlers->$module = $node->$module;
      unset($node->$module);
    }
    $node->file_handlers = (object)$node->file_handlers;
    $node->file_handlers->$module = (object)($node->file_handlers->$module);
  }
  
  // Fix rights
  $node->folder_rights = (object)$node->folder_rights;
  unset($node->folder_rights->private_downloads);
  _filebrowser_set_default($node->folder_rights->explore_subdirs, FALSE);
  _filebrowser_set_default($node->folder_rights->download_archive, FALSE);
  _filebrowser_set_default($node->folder_rights->forbidden_files, "descript.ion\nfile.bbs\n.git\nCSV\n.svn\n");
  _filebrowser_set_default($node->folder_rights->filtered_files, "");
  if (!isset($node->folder_rights->download_manager) || !_filebrowser_externals('download_manager', $node->folder_rights->download_manager)) {
    $node->folder_rights->download_manager = 'public';
  }
  
  // Fix presentation
  $node->folder_presentation = (object)$node->folder_presentation;
  _filebrowser_set_default($node->folder_presentation->encoding, 'UTF8');
  if (!isset($node->folder_presentation->default_view) || !_filebrowser_externals('presentation', $node->folder_presentation->default_view)) {
    $node->folder_presentation->default_view = 'list-view';
  }
  _filebrowser_set_default($node->folder_presentation->hide_extension, FALSE);
  $columns = _filebrowser_externals('metadata');
  $node->folder_presentation->visible_columns = _filebrowser_filter_checkboxes_result($node->folder_presentation->visible_columns);
  foreach ($node->folder_presentation->visible_columns as $name => $value) {
    if (!isset($columns[$name])) {
      unset($node->folder_presentation->visible_columns[$name]);
    }
  }
  if (count($node->folder_presentation->visible_columns) == 0) {
    $node->folder_presentation->visible_columns = array(
        FILEBROWSER_DATA_NAME_ICON => 1, 
        FILEBROWSER_DATA_NAME_DISPLAY_NAME => 1 
    );
  }
  if (!isset($node->folder_presentation->default_sort)) {
    foreach ($node->folder_presentation->visible_columns as $name => $foo) {
      if (isset($columns[$name]['sortable']) && $columns[$name]['sortable']) {
        $node->folder_presentation->default_sort = $name;
        break;
      }
    }
  }
  _filebrowser_set_default($node->folder_presentation->default_sort_order, 'asc');
  
  // Create serialized properties
  $data = new stdClass();
  $data->folder_rights = $node->folder_rights;
  $data->folder_presentation = $node->folder_presentation;
  $data->folder_uploads = $node->folder_uploads;
  $data->file_handlers = $node->file_handlers;
  $node->properties = serialize($data);
  //  echo "<pre>";
//  var_dump($node);
//  echo "</pre>";
//  exit();
}

function _filebrowser_externals($kind, $name = NULL) {
  static $externals = array();
  if (!isset($externals[$kind])) {
    $externals[$kind] = module_invoke_all("filebrowser_$kind");
  }
  if ($name) {
    return isset($externals[$kind][$name]) ? $externals[$kind][$name] : NULL;
  }
  return $externals[$kind];
}

function _filebrowser_options_to_query($options, $values = array()) {
  $options = array_merge($options, $values);
  unset($options['relative_path']);
  $query = '';
  foreach ($options as $key => $value) {
    if (!empty($query)) {
      $query .= "&";
    }
    $query .= "$key=$value";
  }
  return $query;
}

function _filebrowser_query_to_options(&$node) {
  static $options = array(
      'fid', 
      'sort', 
      'order', 
      'view' 
  );
  $result = array();
  foreach ($options as $option) {
    if (isset($_GET[$option])) {
      $result[$option] = $_GET[$option];
    }
  }
  return $result;
}

/**
 * Convert a string from FileSystem encoding to UTF-8. 
 * 
 * @param $node which provide FileSystem encoding information. 
 * @param $source source string
 * 
 * @return re-encoded string
 */
function _filebrowser_encoding_from_fs(&$node, $source) {
  return strcasecmp($node->folder_presentation->encoding, 'UTF-8') == 0 ? $source : mb_convert_encoding($source, "UTF-8", $node->folder_presentation->encoding);
}

/**
 * Convert a string from UTF-8 to FileSystem encoding.  
 * 
 * @param $node which provide FileSystem encoding information. 
 * @param $source source string
 * 
 * @return re-encoded string
 */
function _filebrowser_encoding_to_fs(&$node, $source) {
  return strcasecmp($node->folder_presentation->encoding, 'UTF-8') == 0 ? $source : mb_convert_encoding($source, $node->folder_presentation->encoding, "UTF-8");
}

/**
 * remove all content bits from parent node id.  
 */
function _filebrowser_node_content_delete($node) {
  db_query("
  	DELETE 
  	FROM {node_dir_listing_content} 
  	WHERE 
  		nid = %d", $node->nid);
}

/**
 * Load a specific node content.
 * 
 * @param $fid content fid
 * @return content record
 */
function _filebrowser_node_content_load($fid) {
  $content = db_fetch_array(db_query("
  	SELECT * 
  	FROM {node_dir_listing_content} 
  	WHERE fid=%d", $fid));
  return $content;
}

/**
 * Load data from current path.
 * @param source node
 */
function _filebrowser_load_files(& $node, $fid = NULL) {
  global $user, $base_url;
  
  // retreive paths options
  $options = _filebrowser_query_to_options($node);
  
  // convert any fid parameter to relative_path
  $fid = isset($options['fid']) ? $options['fid'] : $fid;
  if ($fid) {
    $content = _filebrowser_node_content_load($fid);
    if (!$content) {
      drupal_goto("node/{$node->nid}");
    }
    $relative_path = $content['path'];
  }
  else {
    $relative_path = '/';
  }
  $is_subdir = $relative_path != '/';
  
  // If we shouldn't be in a subdirectory, redirect to root_dir.
  if ($is_subdir && !_filebrowser_can_explore_subfolders($node)) {
    drupal_set_message(t('You\'re not allowed to browse sub folders.'));
    return;
  }
  
  // More advanced check to make sure no parent directories match our files.
  // blacklist
  if (!empty($relative_path)) {
    $dirs = explode('/', $relative_path);
    foreach ($dirs as $dir) {
      if (!empty($dir)) {
        if (_filebrowser_cant_view_file($node, $dir)) {
          drupal_set_message(t("You're not allowed to view '%dir'.", array(
              '%dir' => $dir 
          )));
          return;
        }
      }
    }
  }
  // If this folder has already been processed, take it result
  static $cache = array();
  if (isset($cache[$relative_path])) {
    return $node->file_listing = $cache[$relative_path];
  }
  
  // Load database folder content 
  $db_content = array();
  $cursor = db_query("SELECT * FROM {node_dir_listing_content} where nid=%d and root = '%s'", $node->nid, $relative_path);
  while ($data = db_fetch_array($cursor)) {
    $db_content[$data['path']] = $data;
  }
  
  // Build full path
  $fs_root = realpath(_filebrowser_encoding_to_fs($node, _filebrowser_get_node_file_path($node)));
  $fs_root = realpath($fs_root . "/" . _filebrowser_encoding_to_fs($node, $relative_path)) . "/";
  //   print("Root Full: " . _filebrowser_to_utf8($node, $fs_root) . "<br>");
  

  // Load meta-data
  $file_metadata = array();
  // Iterate over files
  $files = array();
  $files_count = 0;
  $total_size = 0;
  $folder_count = 0;
  if (is_dir($fs_root) && $handler = opendir($fs_root)) {
    while (($fs_filename = readdir($handler)) !== FALSE) {
      $fs_file_full_path = $fs_root . $fs_filename;
      
      // Check FS reading rights
      if (!is_readable($fs_file_full_path)) {
        continue;
      }
      
      // Check subfolder rights
      if (is_dir($fs_file_full_path) && !_filebrowser_can_explore_subfolders($node)) {
        continue;
      }
      
      // First file filtering
      if ($fs_filename == '.' || $fs_filename == '..') {
        // We are in the root folder, so we don't need an "up" entry
        if ($fs_filename == '..' && !$is_subdir) {
          continue;
        }
      
      }
      else {
        // This file is not filtered
        if (is_file($fs_file_full_path) && !_filebrowser_can_view_file($node, $fs_filename)) {
          continue;
        }
        
        // This file is not allowed
        if (_filebrowser_cant_view_file($node, $fs_filename)) {
          continue;
        }
      }
      
      // Build file relative path
      $filename = _filebrowser_encoding_from_fs($node, $fs_filename);
      if ($filename == '.') {
        $file_relative_path = $relative_path;
      }
      elseif ($filename == '..') {
        $file_relative_path = _filebrowser_safe_dirname($relative_path);
        $content = db_fetch_array(db_query("SELECT * from {node_dir_listing_content} WHERE nid=%d and path='%s'", $node->nid, $file_relative_path));
        if ($content) {
          $db_content[$file_relative_path] = &$content;
        }
      
      }
      else {
        $file_relative_path = $relative_path . ($relative_path != '/' ? '/' : '') . $filename;
      }
      
      // Build database file record
      if (!isset($db_content[$file_relative_path])) {
        $db_content[$file_relative_path] = array(
            'exists' => TRUE, 
            'nid' => $node->nid, 
            'root' => $relative_path, 
            'path' => $file_relative_path 
        );
      }
      $db_content[$file_relative_path]['exists'] = TRUE;
      $db_content[$file_relative_path]['display-name'] = $filename;
      
      $files[$filename] = array(
          'nid' => $node->nid, 
          'display-name' => $filename, 
          'relative-path' => $file_relative_path, 
          'full-path' => rtrim($fs_root, '/') . "/" . $fs_filename, 
          'status' => MARK_READ 
      );
      $result = module_invoke_all('filebrowser_metadata', 'get', $files[$filename]);
      if ($result && is_array($result)) {
        $files[$filename] = array_merge($files[$filename], $result);
      }
      
      if ($files[$filename]['kind'] == 0) {
        $total_size += $files[$filename]['size'];
        $files_count++;
      }
      else {
        if ($fs_filename != '.') {
          $folder_count++;
        }
      }
      if ($user->uid) {
        if ($user->access < $files[$filename]['created']) {
          $files[$filename]['status'] = MARK_NEW;
        }
        elseif ($user->access < $files[$filename]['modified']) {
          $files[$filename]['status'] = MARK_UPDATED;
        }
      }
      
      if ($fs_filename == '..') {
        $files[$filename]['mime-type'] .= "/parent";
        $files[$filename]['kind'] = 2;
      }
      
      $file_path = array();
      if ($fs_filename == '..') {
        $parent_folder = dirname($relative_path);
        if ($parent_folder != "/") {
          $file_path['path'] = $parent_folder;
        }
      }
      else {
        $file_path['path'] = $relative_path . $fs_filename;
      }
    
    }
    
    // Set global folder properties
    $files['.']['size'] = $total_size;
    $files['.']['files_count'] = $files_count;
    $files['.']['folders_count'] = $folder_count;
    $files['.']['relative-path'] = $relative_path;
    closedir($handler);
  }
  
  // Synchronize what we seen on filesystem with what is stored in database
  // We also build an access URL for each file as it is why we stored this stuff
  // in DB (have a unique ID for each file and path) to get rid of nationnal character
  // mess in URLS.
  $to_delete = array();
  foreach ($db_content as $path => &$record) {
    if (!isset($record['exists'])) {
      $to_delete[] = $record['fid'];
    }
    else {
      if (!isset($record['fid'])) {
        drupal_write_record('node_dir_listing_content', $record);
      }
      
      $key = $record['display-name'];
      $files[$key]['fid'] = $record['fid'];
      if ($files[$key]['kind'] != 0) {
        $files[$key]['url'] = url('node/' . $node->nid, array(
            'query' => _filebrowser_options_to_query($options, array(
                'fid' => $record['fid'] 
            )), 
            'absolute' => TRUE 
        ));
      }
      else {
        $files[$key]['url'] = url('filebrowser/download/' . $record['fid'], array(
            'absolute' => TRUE 
        ));
      }
    }
  }
  
  // A quick way to drip obsolete records (FIXME not quite sure there is no limit in the number of 
  // items in the list
  if (count($to_delete)) {
    db_query("DELETE FROM {node_dir_listing_content} where fid in (" . implode(",", $to_delete) . ")");
  }
  
  // Cache update
  // FIXME : what of two nodes point to the same folder ? 
  $cache[$relative_path] = & $files;
  
  // Add all this data to the node
  $node->file_listing = $files;
}

function _filebrowser_read_description($file_path) {
  $base_path = _filebrowser_safe_dirname($file_path);
  $data = _filebrowser_load_description_file($base_path);
  $name = _filebrowser_safe_basename($file_path);
  if (isset($data['data'][$name])) {
    return $data['data'][$name];
  }
  else {
    return '';
  }
}

function _filebrowser_save_description_file($data) {
  $output = array();
  foreach ($data['data'] as $key => $value) {
    $value=str_replace(
        "\n", 
        '\n', 
        $value 
    );
    $output[] = "$key\t$value";
  }
  file_put_contents($data['file'], implode("\n", $output));
}

function _filebrowser_load_description_file($path, $replace=NULL) {
  static $descriptions = array();
  if ($replace) {
  	$descriptions[$path]=$replace;
  }
  if (!isset($descriptions[$path])) {
    $descriptions[$path] = array(
        'data' => array() 
    );
    $description_file = $path . '/descript.ion';
    if (!file_exists($description_file)) {
      $description_file = $path . '/file_name.bbs';
    }
    if (file_exists($description_file)) {
      $descriptions[$path]['file'] = $description_file;
      foreach (file($description_file) as $line) {
        if (trim($line) == '' || strpos(trim($line), '#') === 0) {
          continue;
        }
        $matches = array();
        preg_match('/"([^"]+)"\s+(.*)|(\S+)\s+(.*)/', $line, $matches);
        $name = !empty($matches[1]) ? $matches[1] : $matches[3];
        $description = !empty($matches[2]) ? $matches[2] : $matches[4];
        $description=str_replace('\n', "\n", $description);
        if (isset($descriptions[$path][$name])) {
          $descriptions[$path]['data'][$name] .= ' ' . trim($description);
        }
        else {
          $descriptions[$path]['data'][$name] = trim($description);
        }
      }
    }
  }
  return $descriptions[$path];
}

/**
 * Check if user can download ZIP archives. 
 */
function _filebrowser_can_download_archive(& $node) {
  return (node_access('view', $node) && $node->folder_rights->download_archive && user_access(FILEBROWSER_DOWNLOAD_ARCHIVE));
}

/**
 * Check if user can download files. 
 */
function _filebrowser_can_download_file(& $node) {
  return (node_access('view', $node) && user_access(FILEBROWSER_DOWNLOAD));
}

/**
 * Check if user can view a specific file.  
 */
function _filebrowser_can_view_file(& $node, $file) {
  return trim($node->folder_rights->filtered_files) == '' || _filebrowser_match_path($file, $node->folder_rights->filtered_files);
}

/**
 * Check if user should not view a specific file. 
 */
function _filebrowser_cant_view_file(& $node, $file) {
  return trim($node->folder_rights->forbidden_files) != '' && _filebrowser_match_path($file, $node->folder_rights->forbidden_files);
}

/**
 * Check if user can explore sub-folders.  
 */
function _filebrowser_can_explore_subfolders(& $node) {
  return $node->folder_rights->explore_subdirs;
}

/**
 * Create a thumbnail and the associated XHTML code for a specific file.
 * @param $node source node
 * @param $file source file
 * @return XHTML code
 */
function _filebrowser_thumbnails_generate(& $node, & $file) {
  static $thumbnailers = NULL;
  if (!$thumbnailers) {
    $thumbnailers = module_implements("filebrowser_thumbnailer");
  }
  if (count($thumbnailers) != 0) {
    foreach ($thumbnailers as $thumbnailer) {
      if ($node->file_handlers->$thumbnailer->enabled) {
        $thumbnail = module_invoke($thumbnailer, "filebrowser_thumbnailer", $file, $node->file_handlers->$thumbnailer);
        if ($thumbnail) {
          return $thumbnail;
        }
      }
    }
  }
  $main_type = dirname($file['mime-type']);
  $mime_type = str_replace("/", "-", $file['mime-type']);
  $module_path = drupal_get_path("module", "filebrowser") . "/icons/";
  $theme_path = drupal_get_path('theme', $GLOBALS['theme'])."/filebrowser/";
  $icons = array(
      $theme_path . $mime_type . ".png", 
      $theme_path . $main_type . ".png", 
      $module_path . $mime_type . ".png", 
      $module_path . $main_type . ".png" 
  );
  foreach ($icons as $icon) {
    if (file_exists($icon)) {
      return theme('image', $icon);
    }
  }
  return theme('image', $module_path . "unknown.png");
}