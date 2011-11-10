<?php
/**
 * @file
 * Theme implementation for the 3 sigmah-news newsletter of simplenews body
 *  
 *
 * Available variables:
 * - node: Newsletter node object
 * - $body: Newsletter body (formatted as plain text or HTML)
 * - $title: Node title
 * - $language: Language object
 *
 * @see template_preprocess_simplenews_newsletter_body()
 */
?>
<?php //if ($format == 'html'): ?>

<style type="text/css">
<!--
h2 {
  font-family: Arial,Tahoma,Helvetica,sans-serif;
  color: #41281b;
  background-color: #f5b235;
}

p,li {
  font-family: Arial,Tahoma,Helvetica,sans-serif;
  color: #503227;
}

#newsletter-footer {
  text-align: center;
}
 
//-->
</style>

<table cellpadding="0" cellspacing="0" width="100%">
  <tr>
    <td align="center">
      <table cellpadding="0" cellspacing="0" width="658">
        <tr>
          <td>
            <?php
              $titleBannerAltArray = array (
                  'en' => "Recent progress with Sigmah",
                  'es' => "Los &uacute;ltimos progresos del Sigmah",
                  'fr' => "Sigmah, les avanc&eacute;es r&eacute;centes du projet"
                );
            ?>
            <img alt="<?php echo $titleBannerAltArray[$node->language]; ?>" src="http://www.sigmah.org/mailing-img/sigmah-news-<?php echo $node->language; ?>--titleBanner.png" style="border:0;" />
          </td>
        </tr>        
        <tr>
          <td>
          
<?php print $body; ?>
          
          </td>
        </tr>

<?php //else: ?>
<?php // print $body; ?>   
<?php //endif ?>