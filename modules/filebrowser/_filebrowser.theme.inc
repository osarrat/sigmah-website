<?php

function theme_dir_listing_icon_view($node) {
  $files = & $node->file_listing;
  
  // Record if directory listing is empty or not.
  $directory_empty = TRUE;
  $output = "<div class='dir-listing-icon-view'>";
  // Considered each file (or directory)
  foreach ($files as $file_name => $data) {
    // Skip dot folder
    if ($file_name == ".") {
      continue;
    }
    
    // At least one file
    $directory_empty = FALSE;
    
    // File extension case
    if ($file_name != ".." && $data['kind'] == 0 && $node->folder_presentation->hide_extension) {
      $pos = strrpos($data['name'], ".");
      $data[FILEBROWSER_DATA_NAME_DISPLAY_NAME] = substr($data['name'], 0, $pos);
    }
    $thumbnail = _filebrowser_thumbnails_generate($node, $data);
    $href = $data['url'];
    $title = $data['display-name'] == '..' ? t('Go up') : $data['display-name'];
    $new = theme('mark', $data['status']);
    $description = $data['description'];
    $output .= theme('dir_listing_icon', $data, $thumbnail, $title, $description, $href, $new);
  }
  $output .= "</div>";
  return $output;
}

/**
 * Theming function for icon-view.
 *
 * @param $node the node to transform to icon  view.
 * @return A string containing icon view XHTML.
 */

function theme_dir_listing_statistics($statistics) {
  $output = "<div class='dir-listing-status'>";
  if ($statistics['empty']) {
    $output .= $statistics['empty'];
  }
  else {
    $output .= implode(" - ", $statistics);
  }
  $output .= "</div>";
  return $output;
}

/**
 * Theming function for list view.
 *
 * @param $node the node to transform to list view.
 * @return A string containing list view XHTML.
 */
function theme_dir_listing_list_view($node) {
  $files = & $node->file_listing;
  // Deal with files.
  $unsorted_rows = array();
  
  // add optional colunmns
  $node->folder_presentation->visible_columns['description'] = TRUE;
  
  // Visible columns
  $visible_columns = array();
  $column_names = _filebrowser_externals('metadata');
  
  // Record if directory listing is empty or not.
  $directory_empty = TRUE;
  
  // Considered each file (or directory)
  foreach ($files as $file_name => $data) {
    // Skip dot folder
    if ($file_name == ".") {
      continue;
    }
    
    // At least one file
    $directory_empty = FALSE;
    
    // File extension case
    if ($file_name != ".." && $data['kind'] == 0 && $node->folder_presentation->hide_extension) {
      $pos = strrpos($data[FILEBROWSER_DATA_NAME_DISPLAY_NAME], ".");
      $data[FILEBROWSER_DATA_NAME_DISPLAY_NAME] = substr($data[FILEBROWSER_DATA_NAME_DISPLAY_NAME], 0, $pos);
    }
    
    foreach ($column_names as $column_name => $column_spec) {
      if (isset($node->folder_presentation->visible_columns[$column_name]) && $node->folder_presentation->visible_columns[$column_name]) {
        switch ($column_name) {
          case FILEBROWSER_DATA_NAME_ICON :
            $visible_columns[$column_name] = TRUE;
            $unsorted_rows[$file_name][FILEBROWSER_DATA_NAME_ICON] = array(
                'data' => _filebrowser_thumbnails_generate($node, $data), 
                'style' => 'width:1%;' 
            );
            break;
          
          case FILEBROWSER_DATA_NAME_DISPLAY_NAME :
            $unsorted_rows[$file_name][FILEBROWSER_DATA_NAME_DISPLAY_NAME] = '<a href="' . $data['url'] . '">' . ($data['display-name'] == '..' ? t('Go up') : $data['display-name']) . '</a>' . theme('mark', $data['status']);
            $visible_columns[$column_name] = TRUE;
            break;
          
          case FILEBROWSER_DATA_NAME_SIZE :
            $used = ($data['size']) != 0 && ($data['kind'] == 0);
            $unsorted_rows[$file_name][FILEBROWSER_DATA_NAME_SIZE] = $used ? format_size($data['size']) : "";
            if ($used) {
              $visible_columns[$column_name] = TRUE;
            }
            break;
          
          case FILEBROWSER_DATA_NAME_CREATED :
            if ($data['created'] != 0) {
              $visible_columns[$column_name] = TRUE;
            }
            $unsorted_rows[$file_name][FILEBROWSER_DATA_NAME_CREATED] = format_date($data['created'], "small");
            break;
          
          case FILEBROWSER_DATA_NAME_MODIFIED :
            if ($data['modified'] != 0) {
              $visible_columns[$column_name] = TRUE;
            }
            $unsorted_rows[$file_name][FILEBROWSER_DATA_NAME_MODIFIED] = format_date($data['modified'], "small");
            break;
          
          case FILEBROWSER_DATA_NAME_TYPE :
            $used = $data['mime-type'] != 'folder' && $data['mime-type'] != 'folder/parent';
            if ($used) {
              $visible_columns[$column_name] = TRUE;
            }
            $unsorted_rows[$file_name][FILEBROWSER_DATA_NAME_TYPE] = $used ? t($data['mime-type']) : "";
            break;
          
          case FILEBROWSER_DATA_NAME_DESCRIPTION :
            if (!empty($data['description'])) {
              $visible_columns[$column_name] = TRUE;
            }
            $unsorted_rows[$file_name]['description'] = $data['description'];
            break;
          
          default :
            $visible_columns[$column_name] = isset($data[$column_name]);
            if ($visible_columns[$column_name]) {
              $unsorted_rows[$file_name][$column_name] = $data[$column_name];
            }
            break;
        }
      }
    }
  }
  
  if (!$directory_empty) {
    // Builder header and clean up unused columns
    $header = array();
    foreach ($column_names as $column_name => $column_spec) {
      if (isset($visible_columns[$column_name]) && $visible_columns[$column_name]) {
        if ($column_name == FILEBROWSER_DATA_NAME_ICON) {
          $header[$column_name] = array();
        }
        else {
          $header[$column_name] = array(
              'data' => $column_spec['title'], 
              'field' => $column_name 
          );
        }
      }
      else {
        // If unused data then clean up !
        foreach ($unsorted_rows as & $row) {
          unset($row[$column_name]);
        }
      }
    }
    $header[$node->folder_presentation->default_sort]['sort'] = $node->folder_presentation->default_sort_order;
    
    // Handle any and all sorting.
    $table_sort = tablesort_init($header);
    
    // Split files in two heaps to preserve folders and files
    $just_folders = array();
    $just_files = array();
    
    // TODO : take data from $unsorted_rows and not $data
    foreach ($files as $data) {
      if ($data['kind'] == 0) {
        $just_files[] = $data;
      }
      else {
        // Do not retain the '.' folder
        if ($data['display-name'] != '.') {
          $just_folders[] = $data;
        }
      }
    }
    
    // Sort files according to correct column.
    if ($table_sort['sql']) {
      usort($just_folders, "_filebrowser_sorter_" . $table_sort['sql']);
      if ($table_sort['sort'] == 'asc') {
        $just_folders = array_reverse($just_folders, TRUE);
      }
      usort($just_files, "_filebrowser_sorter_" . $table_sort['sql']);
      if ($table_sort['sort'] == 'asc') {
        $just_files = array_reverse($just_files, TRUE);
      }
    }
    // Combine folders and files array and setup correct order.
    //$rows = array_merge($just_folders, $just_files);
    $rows = array();
    foreach ($just_folders as $data) {
      $rows[] = $unsorted_rows[$data['display-name']];
    }
    foreach ($just_files as $data) {
      $rows[] = $unsorted_rows[$data['display-name']];
    }
    $output .= theme('table', $header, $rows);
    return $output;
  } // Directory is not empty


}

function theme_dir_listing_metadata_group($form) {
  $row = array();
  foreach (element_children($form) as $key) {
    array_push($row, drupal_render($form[$key]));
  }
  $rows[] = array(
      'data' => $row, 
      'class' => 'formgroup' 
  );
  $output .= theme('table', array(), $rows, array(
      'class' => 'formgroup' 
  ));
  return $output;
}
