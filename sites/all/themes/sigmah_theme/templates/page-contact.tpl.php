<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
  "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language ?>" lang="<?php print $language->language ?>" dir="<?php print $language->dir ?>">
  <head>
    
    <title><?php print $head_title; ?></title>
    <?php print $head; ?>
    <?php print $styles; ?>
    <?php print $scripts; ?>
  </head>
<body>
	
	<div class="headerPop">
    	<!--<a href="#">Fermer</a>-->
    </div>
	<?php print $messages; ?>
    <div class="contentPop"> 
    	<h4><span><?php echo t('Step').' 2/2 '; ?></span><?php echo t('questions, comments ?'); ?></h4>
    <?php print $content; ?>
	</div>
</body>
</html>
