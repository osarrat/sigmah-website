<?php

// $Id: filebrowser.pages.inc,v 1.1.2.17 2010/06/28 13:41:22 ulhume Exp $


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
 * Callback for filebrowser_download/%node menu.
 */
function filebrowser_page_download($fid) {
  $content = _filebrowser_node_content_load($fid);
  $node = NULL;
  $target = NULL;
  if ($content) {
    $node = node_load($content['nid']);
    $target = _filebrowser_encoding_to_fs($node, _filebrowser_get_node_file_path($node) . $content['path']);
  }
  if (!$content || !$node || !_filebrowser_can_download_file($node) || !$target) {
    drupal_access_denied();
    exit();
  }
  
  if (is_dir($target)) {
    if (!function_exists("zip_open")) {
      drupal_set_message(t("No ZIP support found in PHP installation, please contact your administrator"));
      return;
    }
    $zip = new ZipArchive();
    $target = file_directory_path() . "/filebrowser/archives/" . _filebrowser_safe_basename($target) . ".zip";
    if (!file_exists(_filebrowser_safe_dirname($target))) {
      mkdir(_filebrowser_safe_dirname($target), 0777, TRUE);
    }
    if (file_exists($target)) {
      unlink($target);
    }
    _filebrowser_load_files($node, $fid);
    
    if ($zip->open($target, ZIPARCHIVE::CREATE) === TRUE) {
      foreach ($node->file_listing as $file_name => $file_data) {
        if ($file_data['kind'] === 0) {
          $fs_filename = realpath(_filebrowser_encoding_to_fs($node, _filebrowser_get_node_file_path($node) . "/" . $file_data['relative-path']));
          $zip->addFile($fs_filename, $file_name);
        }
      }
      $zip->close();
    }
    else {
      return t("Unable to create temporary zip file '@file'", array(
          file => $target 
      ));
    }
  }
  
  $decoded_file = _filebrowser_encoding_from_fs($node, $target);
  
  $manager = module_invoke_all('filebrowser_download_manager', 'download', $node->folder_rights->download_manager, $target, $decoded_file);
  drupal_access_denied();
  exit();
}

function filebrowser_page_metadata_form($form_state, $node) {
  _filebrowser_load_files($node);
  $form['#node'] = $node;
  foreach ($node->file_listing as $name => &$file) {
    if ($name != '.') {
      $form[$file['fid']] = array(
          '#type' => 'fieldset', 
          '#title' => $file['display-name'], 
          '#tree' => TRUE, 
          '#collapsible' => FALSE, 
          '#collapsed' => TRUE, 
          '#theme' => 'dir_listing_metadata_group' 
      );
      
      $form[$file['fid']]['thumbnail'] = array(
          '#type' => 'markup', 
          '#value' => _filebrowser_thumbnails_generate($node, $file) 
      );
      $form[$file['fid']]['description'] = array(
          '#type' => 'textarea', 
          '#title' => t('description'), 
          '#default_value' => $file['description'] 
      );
    }
  }
  $form['submit'] = array(
      '#type' => 'submit', 
      '#value' => t('save') 
  );
  return $form;
}

function filebrowser_page_metadata_form_submit($form, $form_state) {
  foreach ($form['#node']->file_listing as $name => $file) {
    if (isset($file['fid']) && isset($form_state['values'][$file['fid']])) {
      module_invoke_all('filebrowser_metadata', 'set', $file, $form_state['values'][$file['fid']]);
    }
  }
  drupal_goto("node/{$form['#node']->nid}");
}