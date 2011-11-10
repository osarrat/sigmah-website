<?php
/**
 * @file
 * Theme implementation to format "Sigmah Boletín" simplenews newsletter footer
 * 
 * File named simplenews-newsletter-footer--<tid>.tpl.php with <tid> for
 * newsletter term's id.
 *
 * Available variables:
 * - $node: newsletter node object
 * - $language: language object
 * - $key: email key [node|test]
 * - $format: newsletter format [plain|html]
 * - $unsubscribe_text: unsubscribe text
 * - $test_message: test message warning message
 *
 * Available tokens:
 * - !confirm_unsubscribe_url: unsubscribe url to be used as link
 * for more tokens: see simplenews_mail_tokens()
 *
 * @see template_preprocess_simplenews_newsletter_footer()
 */

include 'simplenews-newsletter-footer--sigmah-news.inc.tpl.php';
 
?>