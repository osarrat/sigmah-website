// Id: $
function incNumber(num) {
  if (num === undefined) return 1;
  else if (num === 5) return 7;
  else if (num > 6) return 1;
  else return num + 1;
}
function switchImg() {
  $('.skype-status-img-rotate').attr('src', function() {
    return this.src.replace(/^([^\d]+)([\d])\.png$/, "$1" + currentImg + '.png');
  });
  currentImg = incNumber(currentImg);
};
$(document).ready(function() {
  $(function() {
    setInterval( "switchImg()", 2000 );
  });
});
var currentImg = 2;