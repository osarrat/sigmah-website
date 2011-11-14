<?php
/**
 * @file
 * Theme implementation for the 3 sigmah-news newsletter of simplenews footer
 * 
 * Copy this file in your theme directory to create a custom themed footer.
 * Rename it to simplenews-newsletter-footer--<tid>.tpl.php to override it for a 
 * newsletter using the newsletter term's id.
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

 
?>
<?php if ($format == 'html'): ?>
        <tr>
          <td>
          
<p class="newsletter-footer"><a href="!confirm_unsubscribe_url"><?php print $unsubscribe_text ?></a></p>
          
          </td>
        </tr>
          
      </table>
    </td>
  </tr> 
</table>

<?php else: ?>

-- <?php print $unsubscribe_text ?>: !confirm_unsubscribe_url

<?php endif ?>



<?php if ($key == 'test'): ?>
- - - <?php print $test_message ?> - - -
<?php endif ?>