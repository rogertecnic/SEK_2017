%x1 = uint8(x1);
%x2 = uint8(x2);
%x3 = uint8(x3);
%x4 = uint8(x4);
%x5 = uint8(x5);
%x6 = uint8(x6);
rgb = zeros(1,numel(x1),3);
rgb = uint8(rgb);
rgb(:,:,1) = x1;
rgb(:,:,2) = x2;
rgb(:,:,3) = x3;
%hsvE = hsv2rgb(rgb);
%rgb(:,:,1) = x4;
%rgb(:,:,2) = x5;
%rgb(:,:,3) = x6;
%hsvD = hsv2rgb(rgb);
