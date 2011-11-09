<?php
// $Id: webfm-example-listing.tpl.php,v 1.1 2009/08/19 15:24:35 robmilne Exp $

/**
 * @file webfm-example-listing.tpl.php
 * Display a list files as links.
 *
 * Variables:
 * - $files: array of file objects from webfm_build_dir_list class
 *
 * @see webfm_example_listing_theme()
 */
?>

<ul>
  <?php
    if(count($files) > 0) {
      foreach($files as $f) {
        $name = check_plain($f->n); print('  <li>'.l(basename($name), 'webfm_send/'.$f->id).'</li>'."\n");
      }
    }
  ?>
</ul>