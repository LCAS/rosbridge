var WSView = function(dst,w,h) {
  this.dst = dst;
  this.dstCtx = dst.getContext('2d');
  this.dstW = w;
  this.dstH = h;
  this.src = null;
  this.srcCtx = null;
  this.srcData = null;
  this.srcW = 0;
  this.srcH = 0;
  this.srcPixels = null;
}

WSView.prototype.display = function(img) {
  if (img.encoding != 'rgb8') return;

  //get the image data
  var imgW = img.width;
  var imgH = img.height;
  var imgPixels = window.atob(img.data);
  var imgLen = imgPixels.length;

  //create new scaling space if needed 
  if (this.src == null  || 
      this.srcW != imgW || 
      this.srcH != imgH) {
    var div = document.createElement('div');
    div.innerHTML = '<canvas width="'+imgW+'" height="'+imgH+'"></canvas>';
    this.src = div.firstChild;
    this.srcCtx = this.src.getContext('2d');
    this.srcW = imgW;
    this.srcH = imgH;
    this.srcData = this.srcCtx.getImageData(0,0,this.srcW,this.srcH);
    this.srcPixels = this.srcData.data; 
    console.log('created new canvas');
  }

  var i = 0;
  var j = 0;
  while(i < imgLen) {
    for (var k = 0; k < 3; k++) {
      this.srcPixels[j+k] = imgPixels.charCodeAt(i+k);
    }
    this.srcPixels[j+3] = 255;

    i += 3;
    j += 4;
  }

  this.srcCtx.putImageData(this.srcData,0,0);
  this.dstCtx.save();
    this.dstCtx.scale(this.dstW/this.srcW,this.dstH/this.srcH);
    this.dstCtx.drawImage(this.src,0,0);
  this.dstCtx.restore();

}
