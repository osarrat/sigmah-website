<?php
// $Id: webfm-browser.tpl.php,v 1.1 2009/07/26 19:50:49 robmilne Exp $

/**
 * @file webfm_browser.tpl.php
 * Display the webfm browser.
 *
 * Variables:
 * - $links: debug, settings and help links according to permissions
 * - $upload: upload fieldset if the user is permitted access
 *
 * @see webfm_theme()
 */
?>
<div class="more-help-link">
  <?php print $links; ?>
</div>
<noscript><p class="err">JavaScript must be enabled in order to use webfm!</p></noscript>
<div id="webfm">
  <?php print $upload; ?>
</div>