<?php
// $Id: webfm_image.admin.inc,v 1.1 2009/02/08 06:24:05 robmilne Exp $

/**
 * Admin settings callback.
 * The only settings available are the target dimensions of an image file
 *
 * @return a system settings form
 */
function webfm_image_settings() {
  // Sanity check : make sure we've got a working toolkit
  if (!image_get_toolkit()) {
    drupal_set_message(t('No image toolkit is currently enabled and therefore this module will not be able to resize your images. You can select one from the <a href="!link">image toolkit settings page</a>.', array('!link' => url('admin/settings/image-toolkit'))), 'error');
    return false;
  }

  $sizes = _webfm_image_get_sizes();
  $form['#submit'][] = 'webfm_image_settings_submit';

  $form['webfm_image_sizes'] = array(
    '#type' => 'fieldset',
    '#title' => t('WebFM Image sizes'),
    '#tree' => TRUE,
    '#theme' => 'webfm_image_settings_form',
    '#description' => '<p>'.t('Create various pixel dimensions ("thumbnail" is required).<br />For each Label and dimension you enter a new "right-click" menu option will become available in WebFM for image files.<br />Clicking the menu item with resize the image based on these settings.').'</p>',
    '#element_validate' => array('webfm_image_settings_validate'),
  );

  // Add some empty rows for user defined sizes.
  $num_defined = count($sizes);
  for ($i = $num_defined; $i < $num_defined + 3; $i++) {
    $sizes['new'. $i] = array(
      'label' => '',
      'width' => '',
      'height' => '',
    );
  }

  foreach ($sizes as $key => $size) {
    $form['webfm_image_sizes'][$key]['label'] = array(
      '#type' => 'textfield',
      '#default_value' => $size['label'],
      '#size' => 25,
      '#maxlength' => 32,
    );

    // For required sizes, set the value and disable the field.
    if (in_array($sizes[$key]['label'], array('thumbnail'))) {
      $form['webfm_image_sizes'][$key]['label']['#disabled'] = TRUE;
      $form['webfm_image_sizes'][$key]['label']['#value'] = $size['label'];
      $form['webfm_image_sizes'][$key]['label']['#required'] = TRUE;
    }

    $form['webfm_image_sizes'][$key]['width'] = array(
      '#type' => 'textfield',
      '#default_value' => $size['width'],
      '#size' => 5,
      '#maxlength' => 5,
    );
    $form['webfm_image_sizes'][$key]['height'] = array(
      '#type' => 'textfield',
      '#default_value' => $size['height'],
      '#size' => 5,
      '#maxlength' => 5,
    );
  }

  return system_settings_form($form);
}

/**
 * Check that the sizes provided have the required amount of information.
 */
function webfm_image_settings_validate($form, &$form_state) {
  foreach (element_children($form) as $key) {
    // If there's a label they must provide at either a height or width
    //  if(!is_numeric($form['webfm_mp3_options']['buffer']['#value'])) {
    if (!empty($form[$key]['label']['#value'])) {
      if (empty($form[$key]['width']['#value'])) {
        form_set_error("webfm_image_sizes][$key][width", t('You must specify width (integer).'));
      } else {
        if(!is_numeric($form[$key]['width']['#value'])) {
          form_set_error("webfm_image_sizes][$key][width", t('Width is not an integer.'));
        }
      }
      if (empty($form[$key]['height']['#value'])) {
        form_set_error("webfm_image_sizes][$key][height", t('You must specify height (integer).'));
      } else {
        if(!is_numeric($form[$key]['height']['#value'])) {
          form_set_error("webfm_image_sizes][$key][height", t('Height is not an integer.'));
        }
      }
    }
  }
}

/**
 * Make changes to the settings before passing them off to
 * system_settings_form_submit().
 *
 * Remove deleted sizes, and use the label as indexes for new sizes.
 */
function webfm_image_settings_submit($form, &$form_state) {
  $old_sizes = _webfm_image_get_sizes();

  foreach ($form_state['values']['webfm_image_sizes'] as $key => $size) {
    // Remove sizes without labels.
    if (empty($size['label'])) {
      unset($form_state['values']['webfm_image_sizes'][$key]);
    }

    // Check if only one is set, indicating an addition or removal.
    if (isset($form_state['values']['webfm_image_sizes'][$key]) ^ isset($old_sizes[$key])) {

      // When adding a new size, we need to assign a key.
      if (isset($form_state['values']['webfm_image_sizes'][$key])) {
        unset($form_state['values']['webfm_image_sizes'][$key]);
        $new_key = drupal_strtolower(drupal_substr($size['label'], 0, 32));
        $form_state['values']['webfm_image_sizes'][$new_key] = $size;
      }
    }
  }
}

function theme_webfm_image_settings_form($form) {
  $header = array(t('Label'), t('Width'), t('Height'));
  foreach (element_children($form) as $key) {
    $row = array();
    $row[] = drupal_render($form[$key]['label']);
    $row[] = drupal_render($form[$key]['width']);
    $row[] = drupal_render($form[$key]['height']);
    $rows[] = $row;
  }
  $output = theme('table', $header, $rows);
  $output .= drupal_render($form);

  return $output;
}

