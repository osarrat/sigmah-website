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
    <div id="background">

    <!-- ______________________ HEADER _______________________ -->
	<?php if($is_front) :?>
		<div id="header_home">
	<?php else :?>
		<div id="header">
	<?php endif; ?>
		<div id="header_wrapper" class="wrapper">
            <div id="top_header">
                
                <div id="log-zone">
					<?php if ($header): ?>
						<?php print $header; ?>								
					<?php endif; ?>
					<?php
						global $language;
						$lang_name = $language->language;
						
					    $languages = language_list('enabled');
					    //echo 'www '. drupal_get_normal_path($_GET['q']).'<br/>';
						$url = drupal_is_front_page() ? '' : $_GET['q'];	

						$links = array();
						foreach ($languages[1] as $selectedLanguage) {
							$links[$selectedLanguage->language] = array(
								'href'       => $url,
								'title'      => $selectedLanguage->native,
								'language'   => $selectedLanguage,
								'attributes' => array('class' => 'language-link'),
							);
						}
						drupal_alter('translation_link', $links, $path);
						// this adds the real paths, i.e. if we are on a german page,
						// the british flag will point to en/english_alias instead of
						  // en/node_with_german_content
						  translation_translation_link_alter($links, $_GET['q']);

						  // This one adds extended languages, i.e. those that are not enabled.
						  // Disable if you want only flags for enabled languages.
						  i18n_translation_link_alter($links, $_GET['q']);

						  // now add or replace text links by flags, according to your i18n settings.
						  if (function_exists('languageicons_translation_link_alter'))
							languageicons_translation_link_alter($links, $_GET['q']);
						?>
						<div id="lang_select">
							<select id="lang" onchange="location.href=this.options[this.selectedIndex].value;">
						<?php	foreach($links as $keyLanguage => $link) {
							    echo '<option id="' . $link['language']->language . '" value="'
								.url($link['href'], $link).'" ';
								if($link['language']->language == $lang_name){
									echo 'selected';
								}
								echo ' >'. t($link['title']).'</option>';
							}
						?>
							</select>
						</div>
                </div>
                <?php if (!empty($site_name)): ?>
					<h1 id="title" <?php if($lang_name=='en'):echo 'class="en"';elseif($lang_name=='es'):echo 'class="es"';endif; ?> >
						<a href="<?php print $front_page ?>" title="<?php print t('Home'); ?>" rel="home"><?php print $site_name; ?></a>
					</h1>
				<?php endif; ?>
                <h2 id="baseline-title">
				<?php //if (!empty($site_slogan)): print $site_slogan;  endif; ?>
                </h2>
				
				
            </div>
<!--
gestion des menus déroulants :
	dans #main_menu on a l'ul li des menus principaux.
    Si la li à la class 'hassub_menu', il y a un menu déroulant qui va avec, sinon, class 'hasnsub_menu'
    Quand on est dans une page, le lien a la class 'select', et c'est le seul a l'avoir.
    L'id d'un li qui a un sous menu, doit correspondre a l'id du sous menu a dérouler en y rajoutant 'sub_'
    par exemple li_pres deroule sub_li_pres
    l'id d'un lien doit correspondre a cela aussi, avec le 'li_' en moins.
-->			
		<div id="main_menu" >
          <ul>
		    <li class="hasnsub_menu"><a <?php if($is_front): echo 'class="select"';endif; ?> href="<?php echo url('home'); ?>" id="accueil"><span class="libele"><?php echo t('Home'); ?></span><span class="right_mainbtn">&nbsp;</span></a></li>
			<?php
				$submenus = '<div id="sub_menu">';
				$menus = menu_tree_all_data('menu-menu-principal');
				//kpr($menus);
				foreach($menus as $menuName => $menu) {
					$link = $menu['link'];
					//kpr($link['localized_options']['langcode']);
					if($link['localized_options']['langcode'] == $lang_name){
						if($link['has_children'] == 1){
							$select_parent = 0;						
							$submenus = $submenus.'<ul id="sub_li_'.str_replace(' ', '_', $link['title']).'" class="sub_menu">';
							$below = $menu['below'];
							foreach($below as $subMenuName => $subMenu){
								$select = 0;
								$subLink = $subMenu['link'];					
								if(url($subLink['href'], $subLink) == check_plain(request_uri())){
									$select = 1;
									$select_parent = 1;
								}
								if($select == 1){
									$submenus = $submenus.'<li class="lienItem  select"><span class="left_btn">&nbsp;</span><a href="
								'.url($subLink['href'], $subLink).'">'.$subLink['title'].'</a><span class="right_btn">&nbsp;</span></li>';
								}else{
									$submenus = $submenus.'<li class="lienItem"><span class="left_btn">&nbsp;</span><a href="
								'.url($subLink['href'], $subLink).'">'.$subLink['title'].'</a><span class="right_btn">&nbsp;</span></li>';
								}														
							}
							$submenus = $submenus.'</ul>';
							
							echo '<li class="hassub_menu" id="li_'.str_replace(' ', '_', $link['title']).'">';
							if($select_parent == 1){
								echo '<a href="'.url($link['href'], $link).'" class="select" id="'.str_replace(' ', '_', $link['title']).'"><span class="libele">';
							}else{
								echo '<a href="'.url($link['href'], $link).'" id="'.str_replace(' ', '_', $link['title']).'"><span class="libele">';
							}
							echo $link['title'];
							echo '</span><span class="right_mainbtn">&nbsp;</span></a>';
							echo '</li>';
							
						}else{
							echo '<li class="hassub_menu" id="li_'.str_replace(' ', '_', $link['title']).'">';
							echo '<a href="'.url($link['href'], $link).'" id="'.str_replace(' ', '_', $link['title']).'"><span class="libele">';
							echo $link['title'];
							echo '</span><span class="right_mainbtn">&nbsp;</span></a>';
							echo '</li>';
						}
					}
				}
				$submenus = $submenus.'</div>';
			?>
           	<li>
				<div id="rechercher" >
					<?php if ($content_top): ?>
						<div id="content-top">
							<?php print $content_top; ?>
						</div> <!-- /#content-top -->
					<?php endif; ?>
                </div>
            </li>
		 </ul>
         </div>
         <?php
			echo $submenus;
		 ?>
        </div>
    </div>