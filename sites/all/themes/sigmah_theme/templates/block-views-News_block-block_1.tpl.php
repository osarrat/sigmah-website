<div id="block-<?php print $block->module .'-'. $block->delta ?>" class="<?php print $block_classes . ' ' . $block_zebra; ?>">
  <div class="block-inner">

    <h3 id="actualite_title"><?php echo t('News'); ?></h3>
    <div id="actu" class="content">
      <?php print $block->content; //see Site building > Views : News_block & views-view-field-News-block-timestamp.tpl.php & views-view-field-News-block-title.tpl.php?>
    </div>
	<span  class="texte_blanc seeAll"><a href="<?php echo 'allnews';  //see Site building > Views : News ?>"><?php echo t('All News'); ?></a></span>
    <div class="suffix"><?php print $edit_links; ?></div>

  </div> <!-- /block-inner -->
</div> <!-- /block -->