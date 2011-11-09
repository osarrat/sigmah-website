<?php include "entete.php"; ?>
 	<div class="separator"></div>
    <!-- ______________________ MAIN _______________________ -->
    
      <div class="wrapper"  id="content">
		<?php if ($title || $mission || $messages || $help || $tabs): ?>
            <div id="content-header">

              <?php if ($mission): ?>
                <div id="mission"><?php print $mission; ?></div>
              <?php endif; ?>

              <?php print $messages; ?>

              <?php print $help; ?> 

              <?php if ($tabs): ?>
                <div id="admin_tabs"><?php print $tabs; ?></div>
              <?php endif; ?>
			  
			  <?php if ($title && $node->type !== 'og_forum' && $node->type == 'page'): ?>
                <h1 class="title">
				<?php 
				$title = str_replace(t('Revision of '),'',$title);
				$title = str_replace(t('Revisión de '),'',$title);
				$title = str_replace(t('Révision de '),'',$title);	
				echo $title;
				?></h1>
              <?php endif; ?>
			  
			  
            </div> <!-- /#content-header -->
          <?php endif; ?>	
          <div id="main_content">
		  <?php print $feed_icons; ?>
            <?php print $content; ?>
          </div> <!-- /#content-area -->

          

          <?php if ($content_bottom): ?>
            <div id="content-bottom">
				
              <?php print $content_bottom; ?>
            </div><!-- /#content-bottom -->
          <?php endif; ?>

		 <div id="right_col">
		 <?php if ($right): print $right; endif; ?> <!-- /right -->
		 </div>
		  
        </div> <!-- /content-inner /content -->



      </div> <!-- /main -->
      <!-- ______________________ FOOTER _______________________ -->

      <?php if(!empty($footer_message) || !empty($footer_block)): ?>
        <div id="footer" class="wrapper">
          <?php print $footer_message; ?>
          <?php print $footer_block; ?>
        </div> <!-- /footer -->
      <?php endif; ?>
	<?php include "pied.php"; ?>
    </div> <!-- /page -->
    <?php print $closure; ?>
  </body>
</html>