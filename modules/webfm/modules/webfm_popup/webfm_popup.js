// $Id: webfm_popup.js,v 1.2 2010/03/10 05:41:02 robmilne Exp $

function webfm_popup() {}

if (Drupal.jsEnabled) {
  $(window).load(webfm_popupGetMenusAjax);
}

// Add the send to rich text editor link to the right click menu of files.
function webfm_popupGetMenusAjax() {
  Webfm.menuHT.put('file', new Webfm.menuElement("Send to rich text editor", webfm_popup.sendtocaller, ""));
}

// Send the selected file to the rich text editor.
webfm_popup.sendtocaller = function(obj) {
  //CKEditor support
  if (get_url_param('CKEditor').length > 0) {
    var fid = $('#'+obj.row_id).find('td').eq(1).find('a').attr('title');
    window.opener.CKEDITOR.tools.callFunction( get_url_param('CKEditorFuncNum'), Drupal.settings.basePath + 'webfm_send/' + fid);
    window.opener.focus();
    window.close();
    return;
  }

  // the window this popup was called from
  var doc = $(window.opener.document);

  // put the file url in the input with the id specified in the url
  var fpath = obj.filepath;
  var url_id = '#'+get_url_param('url');
  doc.find(url_id).val(Drupal.settings.basePath + Drupal.settings.webfm_popup.fileDirectory + fpath);
  doc.find(url_id).change();

  // put the webfm file-id in the input with the id specified in the url
  var fid = $('#'+obj.row_id).find('td').eq(1).find('a').attr('title');
//  console.log(fid);
  var webfm_id = '#'+get_url_param('webfmid');
  doc.find(webfm_id).val(Drupal.settings.basePath + 'webfm_send/' + fid);
  doc.find(webfm_id).change();

  window.opener.focus();
  window.close();
}

// read GET parameter from the url
// for example on the url http://example.com/page.php?some=value&more=evenmore&get_it=yes
// calling get_url_param('get_it') would return 'yes'
function get_url_param(name) {
	name = name.replace(/[\[]/,"\\\[").replace(/[\]]/,"\\\]");
	var regexS = "[\\?&]"+name+"=([^&#]*)";
	var regex = new RegExp( regexS );
	var results = regex.exec( window.location.href );
	if( results == null ) return "";
	else return results[1];
}
