<?php include "entete.php"; ?>

	<div class="separator"></div>
    <!-- ______________________ MAIN _______________________ -->
    
      <div class="wrapper"  id="content">
		<?php if ($mission || $messages || $help || $tabs): ?>
            <div id="content-header">
              <?php if ($mission): ?>
                <div id="mission"><?php print $mission; ?></div>
              <?php endif; ?>

              <?php print $messages; ?>

              <?php print $help; ?> 

              <?php if ($tabs): ?>
                <div id="admin_tabs"><?php print $tabs; ?></div>
           <?php endif; ?>

            </div> <!-- /#content-header -->
          <?php endif; ?>	
          <div id="main_content" class="main_content_wiki">
				<div class="baseline right linkWiki">
				<a href="<?php echo url('node/'.$node->nid.'/outline'); ?>"><?php echo t('Guide contents'); ?></a>
				<?php if(db_result(db_query('SELECT COUNT(vid) FROM {node_revisions} WHERE nid = %d', $node->nid)) > 1) : ?>
					<a href="<?php echo url('node/'.$node->nid.'/revisions'); ?>"><?php echo t('History'); ?></a>
				<?php endif; ?>
				<a href="<?php echo url('node/'.$node->nid.'/edit'); ?>"><?php echo t('Edit'); ?></a>
				</div>
            <?php if ($title): ?>
                <h4 class="title"><a href="<?php echo url('node/'.$node->nid); ?>"><?php print $title; ?></a></h4>
              <?php endif; ?>
			<?php print $content; ?>
			
           <!-- /#content-area -->

          <?php print $feed_icons; ?>

          <?php if ($content_bottom): ?>
            <div id="content-bottom">
              <?php print $content_bottom; ?>
            </div><!-- /#content-bottom -->
          <?php endif; ?>
		</div>
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