<?php
// $Id: user_stats.author-pane.inc,v 1.1.2.2 2009/02/13 03:18:36 liammcdermott Exp $

/**
 * @file
 * Provides a preprocess function for the author pane used by
 * Advanced Forum and Advanced Profile Kit.
 */

/**
 * Implementation of hook_preprocess_author_pane().
 */
function user_stats_preprocess_author_pane(&$variables) {
  $account_id = $variables['account']->uid;

  if ($account_id != 0) {
    $variables['user_stats_posts'] = user_stats_get_stats('post_count', $account_id);
    $ip = user_stats_get_stats('ip_address', $account_id);
    $variables['user_stats_ip'] = $ip;
  }
}
