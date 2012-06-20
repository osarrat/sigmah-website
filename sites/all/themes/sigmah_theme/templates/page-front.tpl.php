<?php include "entete.php"; ?>
 
    <!-- ______________________ MAIN _______________________ -->
    
      <div id="home">
    	<div id="home_wrapper" class="wrapper">
        	<div id="pub">
			<?php if ($pub): ?>
				<?php print $pub; ?>
			<?php endif; ?>
             <img id="img_test" src="<?php echo $base_path . path_to_theme().'/img/tester.jpg'; ?>" alt="capture d'écran" />
            <div>
          <h2><?php echo t('Test the demo'); ?></h2>
                <h3><?php echo ' '.t('and discover the numerous solutions and features that work for you!'); //and discover the many solutions and functionalities imagined for you! ?></h3>
                <a id="tester" class="button" href="http://demo.sigmah.org"><?php echo t('Test'); ?></a>
            </div>          
            </div>
            
            <div id="actualite">
				<?php if ($actualite): ?>
					<?php print $actualite; //see Site building > Blocks & block-views-News_block-block_1.tpl.php ?>
				<?php endif; ?>             
               	<div id="newsletter" class="bloc_marron">
					<?php if ($newsletter): ?>
						<?php print $newsletter; //see Site building > Blocks & block-simplenews.tpl.php && sigmah.module ?>
					<?php endif; ?>
				</div>                
            </div>      
        </div>
    </div>
    
    <div class="wrapper" id="content">
    	<div id="left" class="col">
        	<h4><?php echo t('Discover'); ?></h4>
                <div class="dotted">
					<?php if ($left): ?>
						<?php print $left; ?>
					<?php endif; ?>
					<p class="miseenavant"><?php echo t('What\'s Sigmah?'); ?></p>
					 <a href="<?php echo t('http://www.dailymotion.com/group/sigmah-launch-conference-2011/')?>" id="video"><?php echo t('Sigmah video'); ?></a>
					 <em class="baseline"><?php echo t('Sigmah launch conference videos');  //(TODO durée 2 min)?></em><br />
					 <span class="baseline"><?php //(TODO 892 visionnages) ?></span>
				</div>
        </div>
        
        <div id="center" class="col">
        	<h4><?php echo t('Use'); ?></h4>
            <div class="dotted">
				<?php if ($center): ?>
					<?php print $center; ?>
				<?php endif; ?>
				<ul class="lien_puce" >
					<?php
						$result = db_query('Select count(uid) as nb from {users}');
						while($nbre = db_fetch_object($result)){
						  $nb_account = ($nbre->nb);	  
						}
						$result = db_query('SELECT count({users}.uid ) AS nb FROM {users}
											INNER JOIN {users_roles} ON {users}.uid = {users_roles}.uid
											INNER JOIN {role} ON {role}.rid = {users_roles}.rid
											WHERE {role}.name = "beta-tester" ');
						while($nbre = db_fetch_object($result)){
						  $nb_beta_testers = ($nbre->nb);  
						}
					?>
                    <li class="first_li"><a href="http://demo.sigmah.org"><?php echo t('Test the Sigmah demo'); ?></a>
					<?php echo '('.$nb_beta_testers.' '.t('beta-testers').')'; ?></li>
                    <li><a href="<?php echo url('organisational-use'); ?>#sigmah_central"><?php echo t('Use Sigmah Central'); ?></a>
					<?php //echo '('.$nb_account.' '.t('accounts').')'; ?></li>
                    <li class="last"><a href="<?php echo url('organisational-use'); ?>#download"><?php echo t('Download Sigmah'); ?></a><?php //(TODO 224 téléchargements)?></li>
                </ul>
                </div>
                </div>

                <div id="right" class="col">
                <h4><?php echo t('Contribute to Sigmah'); ?></h4>
                <div class="dotted">
                <div class="bloc_orange"><span class="titre_marron"><?php echo t('Questions, comments'); ?></span><br />
                <div class="formulaire_send">
                <?php $translated_text_area_content=t("Write your question here..."); ?>
                <textarea id="formulaire_send_textarea"><?php echo $translated_text_area_content; ?></textarea>

                <script>
                <!-- Background Discussion for this script and surrounding HTMl at http://www.sigmah.org/issues/view.php?id=450 -->
                $(document).ready(function(){
                    $("#formulaire_send_textarea").click(function(){
                      if($(this).html()=="<?php echo $translated_text_area_content; ?>"){
                      $(this).html('');
                      }
                      });
                    $("#formulaire_send_textarea").blur(function(){
                      if($(this).html()==''){
                      $(this).html("<?php echo $translated_text_area_content; ?>");
                      }
                      });
                    });
                </script>

                <a class="envoyer button iframe" href="<?php echo 'contact';  ?>" id="send" ><?php echo t('Send');//see Site building > contact form & page-contact & sigmah.module ?></a>
                </div>
                </div>
                <ul class="lien_puce cancel_bg">
                    <li><span class="titre_marron"><?php echo t('Question by e-mail :')?></span><a class="mail" href="mailto:support@sigmah.org">support@sigmah.org</a></li>
                    <li class="lien_2 last"><a href=""><?php echo t('Contact local technical suport')?>
					
					</a> <br /><?php 
					
						$result = db_query('Select count({users}.uid) as nb from {users}
											INNER JOIN {users_roles} ON {users}.uid = {users_roles}.uid
											INNER JOIN {role} ON {role}.rid = {users_roles}.rid
											WHERE {role}.name = "support"');
						while($nbre = db_fetch_object($result)){
						  $nb_experts = ($nbre->nb);	  
						}
						if(!isset($nb_experts)):$nb_experts=t('No');endif;
						echo t('('.$nb_experts.' '.'experts)')?></li>
                </ul>
				<?php if ($contribute): ?>
					<div id="contribute">
						<?php print $contribute; ?>
					</div> <!-- /#contribute -->
				<?php endif; ?>
             </div>
        </div>
    </div>
    <?php include "pied.php"; ?>

</div>
</body>
</html>

<script type="text/javascript" >
jQuery(document).ready(function(){
								
								


	/* Apply fancybox to multiple items */
	
	$("a.iframe").fancybox({
		'transitionIn'	:	'elastic',
		'transitionOut'	:	'elastic',
		'speedIn'		:	400, 
		'speedOut'		:	200, 
		'height'		: 	'100%',
		'width'			:    665,
		'overlayShow'	:	false
	});
	
								});
</script>
