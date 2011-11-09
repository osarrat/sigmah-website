<?php
/**
 * @file book-export-html.tpl.php
 * Default theme implementation for printed version of book outline.
 *
 * Available variables:
 * - $title: Top level node title.
 * - $head: Header tags.
 * - $language: Language code. e.g. "en" for english.
 * - $language_rtl: TRUE or FALSE depending on right to left language scripts.
 * - $base_url: URL to home page.
 * - $content: Nodes within the current outline rendered through
 *   book-node-export-html.tpl.php.
 *
 * @see template_preprocess_book_export_html()
 */
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" lang="<?php print $language->language; ?>" xml:lang="<?php print $language->language; ?>">
  <head>
    <?php print $head; ?>
    <title><?php print $title; ?></title>
    <base href="<?php print $base_url; ?>" />
    <link type="text/css" rel="stylesheet" href="misc/print.css" />
    <?php if ($language_rtl): ?>
      <link type="text/css" rel="stylesheet" href="misc/print-rtl.css" />
    <?php endif; ?>
      <style type="text/css">
        .new-page {
          page-break-after: left;
        }

        body {
          font-family: Arial, Helvetica, sans-serif;
          margin-top: 130px;
          margin-bottom: 70px;
          margin-left: 80px;
          margin-right: 80px;
        }

        .book-heading {
          color: #40271A;
        }

        div.section-1 h1.book-heading {
          text-align: center;
          font-size: 36px;
        }

        div.section-2 h1.book-heading {
          text-align: left;
          font-size: 28px;
          margin-left: 10px;
        }

        div.section-3 h1.book-heading {
          text-align: left;
          font-size: 22px;
          margin-left: 20px;
        }

        div.section-4 h1.book-heading {
          text-align: left;
          font-size: 18px;
          margin-left: 30px;
        }

        div.section-5 h1.book-heading {
          text-align: left;
          font-size: 16px;
          margin-left: 40px;
        }

        div.section-6 h1.book-heading {
          text-align: left;
          font-size: 14px;
          margin-left: 50px;
          font-style: italic;  
        }

        #couverture {
          text-align: center;
        }

        #couverture h1 {
          color: #40271A;
          font-size: 56px;
          padding-top: 60px;
        }
      </style>
    </head>
    <body>

      <!--Permet d'ajouter un header et un footer sur toutes les pages.-->
      <script type="text/php">
        $font = Font_Metrics::get_font("arial");
        $w = $pdf->get_width();
        $h = $pdf->get_height();

        $header = $pdf->open_object();
        $pdf->image(drupal_get_path('module', 'ideia') . '/logo-small.png', "png", 40, 50, 114, 30);
        $pdf->close_object();
        $pdf->add_object($header, "all");
        $pdf->page_text(($w / 2) - (strlen('User Manual') / 2) - 24, 55, 'User Manual', $font, 10, array(0, 0, 0));

        $pdf->page_text(($w / 2) - 5, $h - 50, '{PAGE_NUM}/{PAGE_COUNT}', $font, 10, array(0, 0, 0));
      </script>

      <!-- On rajoute la page de couverture -->
      <div id="couverture">
        <img src="<?php print drupal_get_path('module', 'ideia') . '/logo-big.png'; ?>" alt="Sigmah" />
        <h1><?php print t('User Manual'); ?></h1>
      </div>
      <div class="new-page"></div>

    <?php
      /**
       * The given node is /embedded to its absolute depth in a top level
       * section/. For example, a child node with depth 2 in the hierarchy is
       * contained in (otherwise empty) &lt;div&gt; elements corresponding to
       * depth 0 and depth 1. This is intended to support WYSIWYG output - e.g.,
       * level 3 sections always look like level 3 sections, no matter their
       * depth relative to the node selected to be exported as printer-friendly
       * HTML.
       */
      $div_close = '';
    ?>
    <?php for ($i = 1; $i < $depth; $i++) : ?>
        <div class="section-<?php print $i; ?>">
      <?php $div_close .= '</div>'; ?>
      <?php endfor; ?>

      <?php print $contents; ?>
      <?php print $div_close; ?>

  </body>
</html>

