#include "pch.h"
#include "dl_anomaly_detection_workflow.h"


// Procedures 
// External procedures 
// Chapter: Deep Learning / Evaluation
void add_colormap_to_image (HObject ho_GrayValueImage, HObject ho_Image, HObject *ho_ColoredImage, 
    HTuple hv_HeatmapColorScheme)
{

  // Local iconic variables
  HObject  ho_RGBValueImage, ho_Channels, ho_ChannelsScaled;
  HObject  ho_Channel, ho_ChannelScaled, ho_ChannelScaledByte;
  HObject  ho_ImageByte, ho_ImageByteR, ho_ImageByteG, ho_ImageByteB;

  // Local control variables
  HTuple  hv_Type, hv_NumChannels, hv_ChannelIndex;
  HTuple  hv_ChannelMin, hv_ChannelMax, hv__;

  //
  //This procedure adds a gray-value image to a RGB image with a chosen colormap.
  //
  GetImageType(ho_GrayValueImage, &hv_Type);
  //The image LUT needs a byte image. Rescale real images.
  if (0 != (int(hv_Type==HTuple("real"))))
  {
    scale_image_range(ho_GrayValueImage, &ho_GrayValueImage, 0, 1);
    ConvertImageType(ho_GrayValueImage, &ho_GrayValueImage, "byte");
  }
  else if (0 != (int(hv_Type!=HTuple("byte"))))
  {
    throw HException(HTuple("For this transformation, a byte or real image is needed!"));
  }
  //
  //Apply the chosen color scheme on the gray value.
  apply_colorscheme_on_gray_value_image(ho_GrayValueImage, &ho_RGBValueImage, hv_HeatmapColorScheme);
  //
  //Convert input image to byte image for visualization.
  ImageToChannels(ho_Image, &ho_Channels);
  CountChannels(ho_Image, &hv_NumChannels);
  GenEmptyObj(&ho_ChannelsScaled);
  {
  HTuple end_val19 = hv_NumChannels;
  HTuple step_val19 = 1;
  for (hv_ChannelIndex=1; hv_ChannelIndex.Continue(end_val19, step_val19); hv_ChannelIndex += step_val19)
  {
    SelectObj(ho_Channels, &ho_Channel, hv_ChannelIndex);
    MinMaxGray(ho_Channel, ho_Channel, 0, &hv_ChannelMin, &hv_ChannelMax, &hv__);
    scale_image_range(ho_Channel, &ho_ChannelScaled, hv_ChannelMin, hv_ChannelMax);
    ConvertImageType(ho_ChannelScaled, &ho_ChannelScaledByte, "byte");
    ConcatObj(ho_ChannelsScaled, ho_ChannelScaledByte, &ho_ChannelsScaled);
  }
  }
  ChannelsToImage(ho_ChannelsScaled, &ho_ImageByte);
  //
  //Note that ImageByte needs to have the same number of channels as
  //RGBValueImage to display colormap image correctly.
  CountChannels(ho_ImageByte, &hv_NumChannels);
  if (0 != (int(hv_NumChannels!=3)))
  {
    //Just take the first channel and use this to generate
    //an image with 3 channels for visualization.
    AccessChannel(ho_ImageByte, &ho_ImageByteR, 1);
    CopyImage(ho_ImageByteR, &ho_ImageByteG);
    CopyImage(ho_ImageByteR, &ho_ImageByteB);
    Compose3(ho_ImageByteR, ho_ImageByteG, ho_ImageByteB, &ho_ImageByte);
  }
  //
  AddImage(ho_ImageByte, ho_RGBValueImage, &ho_RGBValueImage, 0.5, 0);
  (*ho_ColoredImage) = ho_RGBValueImage;
  //
  return;
}

// Chapter: Deep Learning / Evaluation
// Short Description: Create a lookup table and convert a grey scale image. 
void apply_colorscheme_on_gray_value_image (HObject ho_InputImage, HObject *ho_ResultImage, 
    HTuple hv_Schema)
{

  // Local iconic variables
  HObject  ho_ImageR, ho_ImageG, ho_ImageB;

  // Local control variables
  HTuple  hv_X, hv_Low, hv_High, hv_OffR, hv_OffG;
  HTuple  hv_OffB, hv_A1, hv_A0, hv_R, hv_G, hv_B, hv_A0R;
  HTuple  hv_A0G, hv_A0B;

  //
  //This procedure generates an RGB ResultImage for a grey-value InputImage.
  //In order to do so, create a color distribution as look up table
  //according to the Schema.
  //
  hv_X = HTuple::TupleGenSequence(0,255,1);
  TupleGenConst(256, 0, &hv_Low);
  TupleGenConst(256, 255, &hv_High);
  //
  if (0 != (int(hv_Schema==HTuple("jet"))))
  {
    //Scheme Jet: from blue to red
    hv_OffR = 3.0*64.0;
    hv_OffG = 2.0*64.0;
    hv_OffB = 64.0;
    hv_A1 = -4.0;
    hv_A0 = 255.0+128.0;
    hv_R = (((((hv_X-hv_OffR).TupleAbs())*hv_A1)+hv_A0).TupleMax2(hv_Low)).TupleMin2(hv_High);
    hv_G = (((((hv_X-hv_OffG).TupleAbs())*hv_A1)+hv_A0).TupleMax2(hv_Low)).TupleMin2(hv_High);
    hv_B = (((((hv_X-hv_OffB).TupleAbs())*hv_A1)+hv_A0).TupleMax2(hv_Low)).TupleMin2(hv_High);
    //
  }
  else if (0 != (int(hv_Schema==HTuple("inverse_jet"))))
  {
    //Scheme InvJet: from red to blue.
    hv_OffR = 64;
    hv_OffG = 2*64;
    hv_OffB = 3*64;
    hv_A1 = -4.0;
    hv_A0 = 255.0+128.0;
    hv_R = (((((hv_X-hv_OffR).TupleAbs())*hv_A1)+hv_A0).TupleMax2(hv_Low)).TupleMin2(hv_High);
    hv_G = (((((hv_X-hv_OffG).TupleAbs())*hv_A1)+hv_A0).TupleMax2(hv_Low)).TupleMin2(hv_High);
    hv_B = (((((hv_X-hv_OffB).TupleAbs())*hv_A1)+hv_A0).TupleMax2(hv_Low)).TupleMin2(hv_High);
    //
  }
  else if (0 != (int(hv_Schema==HTuple("hot"))))
  {
    //Scheme Hot.
    hv_A1 = 3.0;
    hv_A0R = 0.0;
    hv_A0G = ((1.0/3.0)*hv_A1)*255.0;
    hv_A0B = ((2.0/3.0)*hv_A1)*255.0;
    hv_R = (((hv_X*hv_A1)-hv_A0R).TupleMax2(hv_Low)).TupleMin2(hv_High);
    hv_G = (((hv_X*hv_A1)-hv_A0G).TupleMax2(hv_Low)).TupleMin2(hv_High);
    hv_B = (((hv_X*hv_A1)-hv_A0B).TupleMax2(hv_Low)).TupleMin2(hv_High);
    //
  }
  else if (0 != (int(hv_Schema==HTuple("inverse_hot"))))
  {
    //Scheme Inverse Hot.
    hv_A1 = -3.0;
    hv_A0R = hv_A1*255.0;
    hv_A0G = ((2.0/3.0)*hv_A1)*255.0;
    hv_A0B = ((1.0/3.0)*hv_A1)*255.0;
    hv_R = (((hv_X*hv_A1)-hv_A0R).TupleMax2(hv_Low)).TupleMin2(hv_High);
    hv_G = (((hv_X*hv_A1)-hv_A0G).TupleMax2(hv_Low)).TupleMin2(hv_High);
    hv_B = (((hv_X*hv_A1)-hv_A0B).TupleMax2(hv_Low)).TupleMin2(hv_High);
    //
  }
  else
  {
    //
    throw HException(("Unknown color schema: "+hv_Schema)+".");
    //
  }
  //
  LutTrans(ho_InputImage, &ho_ImageR, hv_R);
  LutTrans(ho_InputImage, &ho_ImageG, hv_G);
  LutTrans(ho_InputImage, &ho_ImageB, hv_B);
  Compose3(ho_ImageR, ho_ImageG, ho_ImageB, &(*ho_ResultImage));
  //
  return;
}

// Chapter: Deep Learning / Evaluation
void area_iou (HTuple hv_Sample, HTuple hv_Result, HTuple hv_InstanceType, HTuple hv_ResultSortIndices, 
    HTuple *hv_SampleArea, HTuple *hv_ResultArea, HTuple *hv_IoU)
{

  // Local iconic variables

  // Local control variables
  HTuple  hv_GtRow1, hv_GtCol1, hv_GtRow2, hv_GtCol2;
  HTuple  hv_ResRow1, hv_ResCol1, hv_ResRow2, hv_ResCol2;
  HTuple  hv_GtIdx, hv_Height, hv_Width, hv_ValidIdxs, hv_Intersection;
  HTuple  hv_Union, hv_GtRow, hv_GtCol, hv_GtLength1, hv_GtLength2;
  HTuple  hv_GtPhi, hv_ResRow, hv_ResCol, hv_ResLength1, hv_ResLength2;
  HTuple  hv_ResPhi;

  //
  //Compute the intersection over union (IoU) between
  //the ground truth and the inferred bounding box of the object instances.
  //The bounding box type is determined over the InstanceType.
  //
  if (0 != (int(hv_InstanceType==HTuple("rectangle1"))))
  {
    //Get bounding box coordinates.
    GetDictTuple(hv_Sample, "bbox_row1", &hv_GtRow1);
    GetDictTuple(hv_Sample, "bbox_col1", &hv_GtCol1);
    GetDictTuple(hv_Sample, "bbox_row2", &hv_GtRow2);
    GetDictTuple(hv_Sample, "bbox_col2", &hv_GtCol2);
    GetDictTuple(hv_Result, "bbox_row1", &hv_ResRow1);
    GetDictTuple(hv_Result, "bbox_col1", &hv_ResCol1);
    GetDictTuple(hv_Result, "bbox_row2", &hv_ResRow2);
    GetDictTuple(hv_Result, "bbox_col2", &hv_ResCol2);
    //
    //Sort the results.
    hv_ResRow1 = HTuple(hv_ResRow1[hv_ResultSortIndices]);
    hv_ResCol1 = HTuple(hv_ResCol1[hv_ResultSortIndices]);
    hv_ResRow2 = HTuple(hv_ResRow2[hv_ResultSortIndices]);
    hv_ResCol2 = HTuple(hv_ResCol2[hv_ResultSortIndices]);
    //
    //Compute areas.
    (*hv_SampleArea) = (hv_GtRow2-hv_GtRow1)*(hv_GtCol2-hv_GtCol1);
    (*hv_ResultArea) = (hv_ResRow2-hv_ResRow1)*(hv_ResCol2-hv_ResCol1);
    //
    //Compute IoUs.
    (*hv_IoU) = HTuple((hv_GtRow1.TupleLength())*(hv_ResRow1.TupleLength()),0);
    if (0 != (int(((*hv_IoU).TupleLength())>0)))
    {
      {
      HTuple end_val29 = (hv_GtRow1.TupleLength())-1;
      HTuple step_val29 = 1;
      for (hv_GtIdx=0; hv_GtIdx.Continue(end_val29, step_val29); hv_GtIdx += step_val29)
      {
        hv_Height = (HTuple(hv_GtRow2[hv_GtIdx]).TupleMin2(hv_ResRow2))-(HTuple(hv_GtRow1[hv_GtIdx]).TupleMax2(hv_ResRow1));
        hv_Width = (HTuple(hv_GtCol2[hv_GtIdx]).TupleMin2(hv_ResCol2))-(HTuple(hv_GtCol1[hv_GtIdx]).TupleMax2(hv_ResCol1));
        hv_ValidIdxs = HTuple((hv_Height.TupleGreaterElem(0)).TupleAnd(hv_Width.TupleGreaterElem(0))).TupleFind(1);
        if (0 != (int(hv_ValidIdxs>-1)))
        {
          hv_Intersection = HTuple(hv_Height[hv_ValidIdxs])*HTuple(hv_Width[hv_ValidIdxs]);
          hv_Union = (HTuple((*hv_SampleArea)[hv_GtIdx])+HTuple((*hv_ResultArea)[hv_ValidIdxs]))-hv_Intersection;
          (*hv_IoU)[(hv_GtIdx*(hv_ResRow1.TupleLength()))+hv_ValidIdxs] = (hv_Intersection.TupleReal())/hv_Union;
        }
      }
      }
    }
  }
  else if (0 != (int(hv_InstanceType==HTuple("rectangle2"))))
  {
    //Get bounding box coordinates.
    GetDictTuple(hv_Sample, "bbox_row", &hv_GtRow);
    GetDictTuple(hv_Sample, "bbox_col", &hv_GtCol);
    GetDictTuple(hv_Sample, "bbox_length1", &hv_GtLength1);
    GetDictTuple(hv_Sample, "bbox_length2", &hv_GtLength2);
    GetDictTuple(hv_Sample, "bbox_phi", &hv_GtPhi);
    GetDictTuple(hv_Result, "bbox_row", &hv_ResRow);
    GetDictTuple(hv_Result, "bbox_col", &hv_ResCol);
    GetDictTuple(hv_Result, "bbox_length1", &hv_ResLength1);
    GetDictTuple(hv_Result, "bbox_length2", &hv_ResLength2);
    GetDictTuple(hv_Result, "bbox_phi", &hv_ResPhi);
    //
    //Sort results.
    hv_ResRow = HTuple(hv_ResRow[hv_ResultSortIndices]);
    hv_ResCol = HTuple(hv_ResCol[hv_ResultSortIndices]);
    hv_ResLength1 = HTuple(hv_ResLength1[hv_ResultSortIndices]);
    hv_ResLength2 = HTuple(hv_ResLength2[hv_ResultSortIndices]);
    hv_ResPhi = HTuple(hv_ResPhi[hv_ResultSortIndices]);
    //
    //Compute Areas.
    (*hv_SampleArea) = (4.0*hv_GtLength1)*hv_GtLength2;
    (*hv_ResultArea) = (4.0*hv_ResLength1)*hv_ResLength2;
    //
    //Compute IoUs.
    (*hv_IoU) = HTuple((hv_GtRow.TupleLength())*(hv_ResRow.TupleLength()),0);
    if (0 != (int(((*hv_IoU).TupleLength())>0)))
    {
      {
      HTuple end_val67 = (hv_GtRow.TupleLength())-1;
      HTuple step_val67 = 1;
      for (hv_GtIdx=0; hv_GtIdx.Continue(end_val67, step_val67); hv_GtIdx += step_val67)
      {
        hv_ValidIdxs = HTuple((HTuple((*hv_SampleArea)[hv_GtIdx]).TupleGreaterElem(0)).TupleAnd((*hv_ResultArea).TupleGreaterElem(0))).TupleFind(1);
        if (0 != (int(hv_ValidIdxs>-1)))
        {
          AreaIntersectionRectangle2(HTuple(hv_GtRow[hv_GtIdx]), HTuple(hv_GtCol[hv_GtIdx]), 
              HTuple(hv_GtPhi[hv_GtIdx]), HTuple(hv_GtLength1[hv_GtIdx]), HTuple(hv_GtLength2[hv_GtIdx]), 
              HTuple(hv_ResRow[hv_ValidIdxs]), HTuple(hv_ResCol[hv_ValidIdxs]), HTuple(hv_ResPhi[hv_ValidIdxs]), 
              HTuple(hv_ResLength1[hv_ValidIdxs]), HTuple(hv_ResLength2[hv_ValidIdxs]), 
              &hv_Intersection);
          hv_Union = (HTuple((*hv_SampleArea)[hv_GtIdx])+HTuple((*hv_ResultArea)[hv_ValidIdxs]))-hv_Intersection;
          (*hv_IoU)[(hv_GtIdx*(hv_ResRow.TupleLength()))+hv_ValidIdxs] = (hv_Intersection.TupleReal())/hv_Union;
        }
      }
      }
    }
  }
  else
  {
    throw HException(("Instance type '"+hv_InstanceType)+"' is not supported");
  }
  return;
}

// Chapter: Deep Learning / Model
// Short Description: Distort the given samples to augment the dataset. 
void augment_dl_samples (HTuple hv_DLSampleBatch, HTuple hv_GenParam)
{

  // Local iconic variables
  HObject  ho_Image, ho_SegmentationImage, ho_WeightImage;
  HObject  ho_ImageRotate, ho_ImageScaled, ho_Filter, ho_GaussImage;
  HObject  ho_GaussFilter, ho_Gauss, ho_GaussTargetType, ho_AddImage;
  HObject  ho_ImageSpot, ho_ImagePart, ho_ImageRotated, ho_DomainRotated;

  // Local control variables
  HTuple  hv_AugmentationPercentage, hv_Rotation;
  HTuple  hv_Mirror, hv_BrightnessVariation, hv_BrightnessVariationSpot;
  HTuple  hv_CropPercentage, hv_CropPixel, hv_RotationRange;
  HTuple  hv_IgnoreDirection, hv_ClassIDsNoOrientationExist;
  HTuple  hv_ClassIDsNoOrientation, hv_AugmentationPercentageExists;
  HTuple  hv_RotationExists, hv_MirrorExists, hv_BrightnessVariationExists;
  HTuple  hv_BrightnessVariationSpotExists, hv_CropPercentageExists;
  HTuple  hv_CropPixelExists, hv_RotationRangeExists, hv_IgnoreDirectionExists;
  HTuple  hv_ClassIDsNoOrientationExists, hv_CurrentParamValue;
  HTuple  hv_Exception, hv_NumSamples, hv_NumAvailableDistortions;
  HTuple  hv_SampleIndex, hv_SelectedDistortion, hv_CurrentDistortion;
  HTuple  hv_DLSample, hv_ClassificationImageExists, hv_Rectangle1ParamExist;
  HTuple  hv_Rectangle2ParamExist, hv_SegmentationImageExists;
  HTuple  hv_WeightImageExists, hv_BBoxRow1, hv_BBoxCol1;
  HTuple  hv_BBoxRow2, hv_BBoxCol2, hv_BBoxRow, hv_BBoxCol;
  HTuple  hv_BBoxLength1, hv_BBoxLength2, hv_BBoxPhi, hv_BBoxLabelID;
  HTuple  hv_RotationStep, hv_NumPossibleRotations, hv_CurrentRotation;
  HTuple  hv_ImageWidth, hv_ImageHeight, hv_HomMat2DIdentity;
  HTuple  hv_HomMat2DTmp, hv_HomMat2DAdapted, hv_Offset, hv_HomMat2DRotate;
  HTuple  hv_RowTrans1, hv_ColTrans1, hv_RowTrans2, hv_ColTrans2;
  HTuple  hv_RowTrans, hv_ColTrans, hv_MaxAngle, hv_DiffAngle;
  HTuple  hv_IndicesLarge, hv_ObjIdx, hv_BBoxLengthTmp, hv_NumMirrorMethods;
  HTuple  hv_ProbabilityMethods, hv_StrMirror, hv_StrIdx;
  HTuple  hv_SelectedChar, hv_BBoxCol1Mirror, hv_BBoxCol2Mirror;
  HTuple  hv_BBoxRow1Mirror, hv_BBoxRow2Mirror, hv_BrightnessVariationValue;
  HTuple  hv_SpotSize, hv_SpotRow, hv_SpotColumn, hv_Direction;
  HTuple  hv_ShiftRow, hv_ShiftCol, hv_Type, hv_NChannels;
  HTuple  hv_Index1, hv_CropRate, hv_Row1, hv_Row2, hv_Column1;
  HTuple  hv_Column2, hv_Length;
  HTupleVector  hvec_AvailableDistortions(1);

  //
  //This procedure chooses random samples in the DLSampleBatch
  //and modifies them in order to augment the dataset.
  //The augmentation method has to be specified
  //using the dictionary GenParam.
  //
  //If no augmentation parameter is given we return directly and the samples stay unchanged.
  if (0 != (int((hv_GenParam.TupleLength())==0)))
  {
    return;
  }
  //
  //** Set the default values: ***
  //
  //The percentage of the images that are to be augmented.
  hv_AugmentationPercentage.Clear();
  hv_AugmentationPercentage[0] = "augmentation_percentage";
  hv_AugmentationPercentage[1] = 50;
  //Step size for possible rotations.
  hv_Rotation.Clear();
  hv_Rotation[0] = "rotate";
  hv_Rotation[1] = 0;
  //Allowed mirroring types are coded by 'r' (row), 'c' (column).
  hv_Mirror.Clear();
  hv_Mirror[0] = "mirror";
  hv_Mirror[1] = "off";
  //The absolute brightness change can vary in the range [-value, +value].
  hv_BrightnessVariation.Clear();
  hv_BrightnessVariation[0] = "brightness_variation";
  hv_BrightnessVariation[1] = 0;
  //The absolute brightness peak of a randomly positioned spot can vary in the range [-value, +value].
  hv_BrightnessVariationSpot.Clear();
  hv_BrightnessVariationSpot[0] = "brightness_variation_spot";
  hv_BrightnessVariationSpot[1] = 0;
  //
  //In case of a classification model there are more augmentation types.
  //Fraction of image length and width that remains after cropping (in %).
  hv_CropPercentage.Clear();
  hv_CropPercentage[0] = "crop_percentage";
  hv_CropPercentage[1] = "off";
  //Image length and width that remains after cropping (in pixel).
  hv_CropPixel.Clear();
  hv_CropPixel[0] = "crop_pixel";
  hv_CropPixel[1] = "off";
  //Step range for rotations with step size 1.
  hv_RotationRange.Clear();
  hv_RotationRange[0] = "rotate_range";
  hv_RotationRange[1] = 0;
  //
  //In case of a detection model of instance_type 'rectangle2': Use directions of instances within bounding boxes.
  hv_IgnoreDirection.Clear();
  hv_IgnoreDirection[0] = "ignore_direction";
  hv_IgnoreDirection[1] = 0;
  //In case of a detection model of instance_type 'rectangle2': Class IDs without orientation.
  hv_ClassIDsNoOrientationExist.Clear();
  hv_ClassIDsNoOrientationExist[0] = "class_ids_no_orientation";
  hv_ClassIDsNoOrientationExist[1] = 0;
  hv_ClassIDsNoOrientation = HTuple();
  //** Set user-defined parameters: ***
  //
  GetDictParam(hv_GenParam, "key_exists", HTuple(hv_AugmentationPercentage[0]), &hv_AugmentationPercentageExists);
  GetDictParam(hv_GenParam, "key_exists", HTuple(hv_Rotation[0]), &hv_RotationExists);
  GetDictParam(hv_GenParam, "key_exists", HTuple(hv_Mirror[0]), &hv_MirrorExists);
  GetDictParam(hv_GenParam, "key_exists", HTuple(hv_BrightnessVariation[0]), &hv_BrightnessVariationExists);
  GetDictParam(hv_GenParam, "key_exists", HTuple(hv_BrightnessVariationSpot[0]), 
      &hv_BrightnessVariationSpotExists);
  GetDictParam(hv_GenParam, "key_exists", HTuple(hv_CropPercentage[0]), &hv_CropPercentageExists);
  GetDictParam(hv_GenParam, "key_exists", HTuple(hv_CropPixel[0]), &hv_CropPixelExists);
  GetDictParam(hv_GenParam, "key_exists", HTuple(hv_RotationRange[0]), &hv_RotationRangeExists);
  GetDictParam(hv_GenParam, "key_exists", HTuple(hv_IgnoreDirection[0]), &hv_IgnoreDirectionExists);
  GetDictParam(hv_GenParam, "key_exists", HTuple(hv_ClassIDsNoOrientationExist[0]), 
      &hv_ClassIDsNoOrientationExists);
  //
  if (0 != hv_AugmentationPercentageExists)
  {
    //Set augmentation percentage.
    GetDictTuple(hv_GenParam, HTuple(hv_AugmentationPercentage[0]), &hv_CurrentParamValue);
    hv_AugmentationPercentage[1] = hv_CurrentParamValue;
    //Check if input value is in range of 0-100 %.
    hv_Exception = "The given value for augmentation_percentage has to be in the range 0-100.";
    if (0 != (hv_CurrentParamValue.TupleIsNumber()))
    {
      if (0 != (HTuple(int(hv_CurrentParamValue<0)).TupleOr(int(hv_CurrentParamValue>100))))
      {
        throw HException(hv_Exception);
      }
    }
    else
    {
      throw HException(hv_Exception);
    }
  }
  //If nothing should be augmented we return immediately.
  if (0 != (int(HTuple(hv_AugmentationPercentage[1])==0)))
  {
    return;
  }
  if (0 != hv_RotationExists)
  {
    //Set rotation.
    GetDictTuple(hv_GenParam, HTuple(hv_Rotation[0]), &hv_CurrentParamValue);
    hv_Rotation[1] = hv_CurrentParamValue;
    //Check if the input value is either 0, 90, or 180.
    hv_Exception = HTuple("The value given for rotation has to be either 0, 90, or 180.");
    if (0 != (hv_CurrentParamValue.TupleIsNumber()))
    {
      if (0 != (int((((HTuple(0).Append(90)).Append(180)).TupleFind(hv_CurrentParamValue))==-1)))
      {
        throw HException(hv_Exception);
      }
    }
    else
    {
      throw HException(hv_Exception);
    }
  }
  if (0 != hv_MirrorExists)
  {
    //Set mirroring.
    GetDictTuple(hv_GenParam, HTuple(hv_Mirror[0]), &hv_CurrentParamValue);
    hv_Mirror[1] = hv_CurrentParamValue;
    //Check if the input is a string and contains either 'off' or the mirroring code.
    if (0 != ((hv_CurrentParamValue.TupleIsNumber()).TupleOr(HTuple(HTuple(HTuple(HTuple(HTuple(int(hv_CurrentParamValue==HTuple("off"))).TupleOr(int(hv_CurrentParamValue==HTuple("c")))).TupleOr(int(hv_CurrentParamValue==HTuple("r")))).TupleOr(int(hv_CurrentParamValue==HTuple("cr")))).TupleOr(int(hv_CurrentParamValue==HTuple("rc")))).TupleNot())))
    {
      throw HException("Unknown type for mirroring.");
    }
  }
  if (0 != hv_BrightnessVariationExists)
  {
    //Set brightness variation.
    GetDictTuple(hv_GenParam, HTuple(hv_BrightnessVariation[0]), &hv_CurrentParamValue);
    hv_BrightnessVariation[1] = hv_CurrentParamValue;
    //Check if the input value is in range of 0-255.
    hv_Exception = "The given value for brightness_variation has to be in the range 0-255.";
    if (0 != (hv_CurrentParamValue.TupleIsNumber()))
    {
      if (0 != (HTuple(int(hv_CurrentParamValue<0)).TupleOr(int(hv_CurrentParamValue>255))))
      {
        throw HException(hv_Exception);
      }
    }
    else
    {
      throw HException(hv_Exception);
    }
  }
  if (0 != hv_BrightnessVariationSpotExists)
  {
    //Set brightness variation spot.
    GetDictTuple(hv_GenParam, HTuple(hv_BrightnessVariationSpot[0]), &hv_CurrentParamValue);
    hv_BrightnessVariationSpot[1] = hv_CurrentParamValue;
    //Check if the input value is in range of 0-255.
    hv_Exception = "The given value for brightness_variation_spot has to be in the range 0-255.";
    if (0 != (hv_CurrentParamValue.TupleIsNumber()))
    {
      if (0 != (HTuple(int(hv_CurrentParamValue<0)).TupleOr(int(hv_CurrentParamValue>255))))
      {
        throw HException(hv_Exception);
      }
    }
    else
    {
      throw HException(hv_Exception);
    }
  }
  if (0 != hv_CropPercentageExists)
  {
    //Set cropping with percentage.
    GetDictTuple(hv_GenParam, HTuple(hv_CropPercentage[0]), &hv_CurrentParamValue);
    hv_CropPercentage[1] = hv_CurrentParamValue;
    //Check if the input value is in range of 1-100%.
    hv_Exception = "The given value for crop_percentage has to be in the range 1-100.";
    if (0 != (hv_CurrentParamValue.TupleIsNumber()))
    {
      if (0 != (HTuple(int(hv_CurrentParamValue<1)).TupleOr(int(hv_CurrentParamValue>100))))
      {
        throw HException(hv_Exception);
      }
    }
    else
    {
      throw HException(hv_Exception);
    }
  }
  if (0 != hv_CropPixelExists)
  {
    //Set cropping with pixels.
    GetDictTuple(hv_GenParam, HTuple(hv_CropPixel[0]), &hv_CurrentParamValue);
    hv_CropPixel[1] = hv_CurrentParamValue;
    //Check if the input value is greater 0.
    hv_Exception = "The given value for crop_pixel has to be greater or equal to 1.";
    if (0 != (hv_CurrentParamValue.TupleIsNumber()))
    {
      if (0 != (int(hv_CurrentParamValue<1)))
      {
        throw HException(hv_Exception);
      }
    }
    else
    {
      throw HException(hv_Exception);
    }
  }
  if (0 != hv_RotationRangeExists)
  {
    //Set rotation range.
    GetDictTuple(hv_GenParam, HTuple(hv_RotationRange[0]), &hv_CurrentParamValue);
    hv_RotationRange[1] = hv_CurrentParamValue;
    //Check if the input value is in range of 0-180.
    hv_Exception = "The given value for rotate_range has to be in the range 0-180.";
    if (0 != (hv_CurrentParamValue.TupleIsNumber()))
    {
      if (0 != (HTuple(int(hv_CurrentParamValue<0)).TupleOr(int(hv_CurrentParamValue>180))))
      {
        throw HException(hv_Exception);
      }
    }
    else
    {
      throw HException(hv_Exception);
    }
  }
  if (0 != hv_IgnoreDirectionExists)
  {
    //Set if the direction is to be ignored or not.
    GetDictTuple(hv_GenParam, HTuple(hv_IgnoreDirection[0]), &hv_CurrentParamValue);
    hv_IgnoreDirection[1] = hv_CurrentParamValue;
    hv_Exception = HTuple("The value given for ignore_direction has to be either 'true','false', true or false.");
    if (0 != (int(((((HTuple("true").Append("false")).Append(1)).Append(0)).TupleFind(HTuple(hv_IgnoreDirection[1])))==-1)))
    {
      throw HException(hv_Exception);
    }
    if (0 != (int(hv_CurrentParamValue==HTuple("false"))))
    {
      hv_IgnoreDirection[1] = 0;
    }
    else if (0 != (int(hv_CurrentParamValue==HTuple("true"))))
    {
      hv_IgnoreDirection[1] = 1;
    }
  }
  if (0 != hv_ClassIDsNoOrientationExists)
  {
    //Set the IDs of the classes, for whose instances the orientation is not to be considered.
    GetDictTuple(hv_GenParam, HTuple(hv_ClassIDsNoOrientationExist[0]), &hv_CurrentParamValue);
    hv_ClassIDsNoOrientationExist[1] = int(hv_CurrentParamValue!=HTuple());
    if (0 != (int(hv_CurrentParamValue!=HTuple())))
    {
      hv_ClassIDsNoOrientation = hv_CurrentParamValue;
    }
  }
  //
  //Aggregate all possible distortions and parameter values into a vector.
  //
  hvec_AvailableDistortions = HTupleVector(1);
  //Rotation with a given angular step size.
  if (0 != (int(HTuple(hv_Rotation[1])>0)))
  {
    hvec_AvailableDistortions[HTuple(hvec_AvailableDistortions.Length())] = HTupleVector(hv_Rotation);
  }
  //Mirroring is allowed in row and column direction.
  if (0 != (HTuple(HTuple(hv_Mirror[1]).TupleRegexpTest("r")).TupleOr(HTuple(hv_Mirror[1]).TupleRegexpTest("c"))))
  {
    hvec_AvailableDistortions[HTuple(hvec_AvailableDistortions.Length())] = HTupleVector(hv_Mirror);
  }
  //Brightness variation.
  if (0 != (int(HTuple(hv_BrightnessVariation[1])>0)))
  {
    hvec_AvailableDistortions[HTuple(hvec_AvailableDistortions.Length())] = HTupleVector(hv_BrightnessVariation);
  }
  //Brightness variation spot.
  if (0 != (int(HTuple(hv_BrightnessVariationSpot[1])>0)))
  {
    hvec_AvailableDistortions[HTuple(hvec_AvailableDistortions.Length())] = HTupleVector(hv_BrightnessVariationSpot);
  }
  //Cropping percentage.
  if (0 != (HTuple(hv_CropPercentage[1]).TupleIsNumber()))
  {
    hvec_AvailableDistortions[HTuple(hvec_AvailableDistortions.Length())] = HTupleVector(hv_CropPercentage);
  }
  //Cropping pixels.
  if (0 != (HTuple(hv_CropPixel[1]).TupleIsNumber()))
  {
    hvec_AvailableDistortions[HTuple(hvec_AvailableDistortions.Length())] = HTupleVector(hv_CropPixel);
  }
  //Rotation within a given range (step size 1).
  if (0 != (int(HTuple(hv_RotationRange[1])>0)))
  {
    hvec_AvailableDistortions[HTuple(hvec_AvailableDistortions.Length())] = HTupleVector(hv_RotationRange);
  }
  //
  //** Choose random samples and augmentation methods: ***
  //
  //Number of samples to be augmented.
  hv_NumSamples = hv_DLSampleBatch.TupleLength();
  if (0 != (int(hv_NumSamples==0)))
  {
    throw HException("There are no DLSamples to be processed.");
  }
  //Check number of selected distortions.
  hv_NumAvailableDistortions = HTuple(hvec_AvailableDistortions.Length());
  if (0 != (int(hv_NumAvailableDistortions==0)))
  {
    return;
  }
  //
  //Select randomly a distortion type for each sample.
  //
  //Augment (distort) the samples.
  //
  {
  HTuple end_val233 = hv_NumSamples-1;
  HTuple step_val233 = 1;
  for (hv_SampleIndex=0; hv_SampleIndex.Continue(end_val233, step_val233); hv_SampleIndex += step_val233)
  {
    if (0 != (int((HTuple::TupleRand(1)*100)>HTuple(hv_AugmentationPercentage[1]))))
    {
      //Only augment the given percentage of samples.
      continue;
    }
    //Select the distortion type.
    hv_SelectedDistortion = (HTuple::TupleRand(1)*hv_NumAvailableDistortions).TupleInt();
    hv_CurrentDistortion = hvec_AvailableDistortions[hv_SelectedDistortion].T();
    //
    //Select the image to be augmented.
    hv_DLSample = HTuple(hv_DLSampleBatch[hv_SampleIndex]);
    GetDictObject(&ho_Image, hv_DLSample, "image");
    GetDictParam(hv_DLSample, "key_exists", "image_label_id", &hv_ClassificationImageExists);
    GetDictParam(hv_DLSample, "key_exists", "bbox_row1", &hv_Rectangle1ParamExist);
    GetDictParam(hv_DLSample, "key_exists", "bbox_phi", &hv_Rectangle2ParamExist);
    GetDictParam(hv_DLSample, "key_exists", "segmentation_image", &hv_SegmentationImageExists);
    GetDictParam(hv_DLSample, "key_exists", "weight_image", &hv_WeightImageExists);
    //Check which keys have to be modified as well.
    if (0 != hv_Rectangle1ParamExist)
    {
      GetDictTuple(hv_DLSample, "bbox_row1", &hv_BBoxRow1);
      GetDictTuple(hv_DLSample, "bbox_col1", &hv_BBoxCol1);
      GetDictTuple(hv_DLSample, "bbox_row2", &hv_BBoxRow2);
      GetDictTuple(hv_DLSample, "bbox_col2", &hv_BBoxCol2);
    }
    else if (0 != hv_Rectangle2ParamExist)
    {
      GetDictTuple(hv_DLSample, "bbox_row", &hv_BBoxRow);
      GetDictTuple(hv_DLSample, "bbox_col", &hv_BBoxCol);
      GetDictTuple(hv_DLSample, "bbox_length1", &hv_BBoxLength1);
      GetDictTuple(hv_DLSample, "bbox_length2", &hv_BBoxLength2);
      GetDictTuple(hv_DLSample, "bbox_phi", &hv_BBoxPhi);
      if (0 != (HTuple(hv_ClassIDsNoOrientationExist[1])))
      {
        GetDictTuple(hv_DLSample, "bbox_label_id", &hv_BBoxLabelID);
      }
    }
    if (0 != hv_SegmentationImageExists)
    {
      GetDictObject(&ho_SegmentationImage, hv_DLSample, "segmentation_image");
    }
    if (0 != hv_WeightImageExists)
    {
      GetDictObject(&ho_WeightImage, hv_DLSample, "weight_image");
    }
    //Some augmentation methods are only allowed for model type classification.
    if (0 != (hv_ClassificationImageExists.TupleNot()))
    {
      if (0 != (int(HTuple(hv_CurrentDistortion[0])==HTuple(hv_CropPercentage[0]))))
      {
        throw HException("The augmentation method crop_percentage is only allowed for model type classification.");
      }
      else if (0 != (int(HTuple(hv_CurrentDistortion[0])==HTuple(hv_CropPixel[0]))))
      {
        throw HException("The augmentation method crop_pixel is only allowed for model type classification.");
      }
      else if (0 != (int(HTuple(hv_CurrentDistortion[0])==HTuple(hv_RotationRange[0]))))
      {
        throw HException("The augmentation method rotate_range is only allowed for model type classification.");
      }
    }
    //
    if (0 != (int(HTuple(hv_CurrentDistortion[0])==HTuple(hv_Rotation[0]))))
    {
      //
      //Rotation
      //
      //Determine rotation angle for distortion type 'rotate' (angle in range (0:CurrentDistortion[1]:360)).
      hv_RotationStep = ((const HTuple&)hv_CurrentDistortion)[1];
      hv_NumPossibleRotations = (360.0/hv_RotationStep)-1;
      hv_CurrentRotation = hv_RotationStep*(((hv_NumPossibleRotations*HTuple::TupleRand(1)).TupleInt())+1);
      //
      if (0 != (int(hv_CurrentRotation!=0)))
      {
        GetImageSize(ho_Image, &hv_ImageWidth, &hv_ImageHeight);
        if (0 != (HTuple(int(hv_ImageWidth!=hv_ImageHeight)).TupleAnd(int(hv_CurrentRotation!=180.0))))
        {
          //If an image is not quadratic, a rotation by 90 or 270 degrees is ignored.
          continue;
        }
        //
        RotateImage(ho_Image, &ho_ImageRotate, hv_CurrentRotation, "constant");
        SetDictObject(ho_ImageRotate, hv_DLSample, "image");
        //
        if (0 != (hv_Rectangle1ParamExist.TupleOr(hv_Rectangle2ParamExist)))
        {
          //Create a transformation matrix for the rotation of the bounding boxes.
          GetImageSize(ho_Image, &hv_ImageWidth, &hv_ImageHeight);
          HomMat2dIdentity(&hv_HomMat2DIdentity);
          HomMat2dTranslate(hv_HomMat2DIdentity, 0.5, 0.5, &hv_HomMat2DTmp);
          HomMat2dTranslateLocal(hv_HomMat2DTmp, -0.5, -0.5, &hv_HomMat2DAdapted);
          hv_Offset = ((hv_ImageHeight-hv_ImageWidth)*0.5)*((hv_CurrentRotation.TupleRad()).TupleSin());
          HomMat2dTranslate(hv_HomMat2DAdapted, hv_Offset, hv_Offset, &hv_HomMat2DAdapted);
          HomMat2dRotate(hv_HomMat2DAdapted, hv_CurrentRotation.TupleRad(), hv_ImageHeight*0.5, 
              hv_ImageWidth*0.5, &hv_HomMat2DRotate);
        }
        if (0 != hv_Rectangle1ParamExist)
        {
          AffineTransPixel(hv_HomMat2DRotate, hv_BBoxRow1, hv_BBoxCol1, &hv_RowTrans1, 
              &hv_ColTrans1);
          AffineTransPixel(hv_HomMat2DRotate, hv_BBoxRow2, hv_BBoxCol2, &hv_RowTrans2, 
              &hv_ColTrans2);
          if (0 != (int(hv_CurrentRotation==90)))
          {
            hv_BBoxRow1 = hv_RowTrans2;
            hv_BBoxCol1 = hv_ColTrans1;
            hv_BBoxRow2 = hv_RowTrans1;
            hv_BBoxCol2 = hv_ColTrans2;
          }
          else if (0 != (int(hv_CurrentRotation==180)))
          {
            hv_BBoxRow1 = hv_RowTrans2;
            hv_BBoxCol1 = hv_ColTrans2;
            hv_BBoxRow2 = hv_RowTrans1;
            hv_BBoxCol2 = hv_ColTrans1;
          }
          else if (0 != (int(hv_CurrentRotation==270)))
          {
            hv_BBoxRow1 = hv_RowTrans1;
            hv_BBoxCol1 = hv_ColTrans2;
            hv_BBoxRow2 = hv_RowTrans2;
            hv_BBoxCol2 = hv_ColTrans1;
          }
          //
          SetDictTuple(hv_DLSample, "bbox_row1", hv_BBoxRow1);
          SetDictTuple(hv_DLSample, "bbox_col1", hv_BBoxCol1);
          SetDictTuple(hv_DLSample, "bbox_row2", hv_BBoxRow2);
          SetDictTuple(hv_DLSample, "bbox_col2", hv_BBoxCol2);
        }
        else if (0 != hv_Rectangle2ParamExist)
        {
          AffineTransPixel(hv_HomMat2DRotate, hv_BBoxRow, hv_BBoxCol, &hv_RowTrans, 
              &hv_ColTrans);
          //Write the bounding box angles phi in the expected interval:
          //-180бу < phi <= 180бу or if IgnoreDirection set to true -90бу < phi <= 90бу.
          if (0 != (HTuple(hv_IgnoreDirection[1]).TupleNot()))
          {
            hv_BBoxPhi += hv_CurrentRotation.TupleRad();
            hv_MaxAngle = 180;
            hv_DiffAngle = 360;
          }
          else
          {
            if (0 != (HTuple(int(hv_CurrentRotation==90)).TupleOr(int(hv_CurrentRotation==270))))
            {
              hv_BBoxPhi += HTuple(90).TupleRad();
            }
            hv_MaxAngle = 90;
            hv_DiffAngle = 180;
          }
          hv_IndicesLarge = (hv_BBoxPhi.TupleGreaterElem(hv_MaxAngle.TupleRad())).TupleFind(1);
          if (0 != (int(hv_IndicesLarge!=-1)))
          {
            hv_BBoxPhi[hv_IndicesLarge] = HTuple(hv_BBoxPhi[hv_IndicesLarge])-(hv_DiffAngle.TupleRad());
          }
          //Check that the angle BBoxPhi for objects without orientation is always set to 0.0.
          if (0 != (HTuple(hv_ClassIDsNoOrientationExist[1])))
          {
            {
            HTuple end_val357 = (hv_BBoxLabelID.TupleLength())-1;
            HTuple step_val357 = 1;
            for (hv_ObjIdx=0; hv_ObjIdx.Continue(end_val357, step_val357); hv_ObjIdx += step_val357)
            {
              if (0 != (int((hv_ClassIDsNoOrientation.TupleFind(HTuple(hv_BBoxLabelID[hv_ObjIdx])))!=-1)))
              {
                hv_BBoxPhi[hv_ObjIdx] = 0.0;
                //These classes require Length1 <= Length2: exchange them for 90бу or 270бу rotations.
                if (0 != (HTuple(int(hv_CurrentRotation==90)).TupleOr(int(hv_CurrentRotation==270))))
                {
                  hv_BBoxLengthTmp = HTuple(hv_BBoxLength1[hv_ObjIdx]);
                  hv_BBoxLength1[hv_ObjIdx] = HTuple(hv_BBoxLength2[hv_ObjIdx]);
                  hv_BBoxLength2[hv_ObjIdx] = hv_BBoxLengthTmp;
                }
              }
            }
            }
          }
          SetDictTuple(hv_DLSample, "bbox_row", hv_RowTrans);
          SetDictTuple(hv_DLSample, "bbox_col", hv_ColTrans);
          SetDictTuple(hv_DLSample, "bbox_phi", hv_BBoxPhi);
          SetDictTuple(hv_DLSample, "bbox_length1", hv_BBoxLength1);
          SetDictTuple(hv_DLSample, "bbox_length2", hv_BBoxLength2);
        }
        if (0 != hv_SegmentationImageExists)
        {
          RotateImage(ho_SegmentationImage, &ho_SegmentationImage, hv_CurrentRotation.TupleInt(), 
              "constant");
          SetDictObject(ho_SegmentationImage, hv_DLSample, "segmentation_image");
        }
        if (0 != hv_WeightImageExists)
        {
          RotateImage(ho_WeightImage, &ho_WeightImage, hv_CurrentRotation.TupleInt(), 
              "constant");
          SetDictObject(ho_WeightImage, hv_DLSample, "weight_image");
        }
      }
    }
    else if (0 != (int(HTuple(hv_CurrentDistortion[0])==HTuple(hv_Mirror[0]))))
    {
      //
      //Mirroring
      //
      //If more than one axis is allowed,
      //choose mirror axis/axes to be applied.
      hv_NumMirrorMethods = HTuple(hv_CurrentDistortion[1]).TupleStrlen();
      hv_ProbabilityMethods = 1.0/hv_NumMirrorMethods;
      hv_StrMirror = "";
      while (0 != (int(hv_StrMirror==HTuple(""))))
      {
        {
        HTuple end_val394 = hv_NumMirrorMethods-1;
        HTuple step_val394 = 1;
        for (hv_StrIdx=0; hv_StrIdx.Continue(end_val394, step_val394); hv_StrIdx += step_val394)
        {
          hv_SelectedChar = HTuple(hv_CurrentDistortion[1]).TupleStrBitSelect(hv_StrIdx);
          if (0 != (int(HTuple::TupleRand(1)<hv_ProbabilityMethods)))
          {
            hv_StrMirror += hv_SelectedChar;
          }
        }
        }
      }
      //Apply the chosen mirror axis/axes to the given sample data.
      GetImageSize(ho_Image, &hv_ImageWidth, &hv_ImageHeight);
      if (0 != (hv_StrMirror.TupleRegexpTest("c")))
      {
        MirrorImage(ho_Image, &ho_Image, "column");
        if (0 != hv_Rectangle1ParamExist)
        {
          hv_BBoxCol1Mirror = (hv_ImageWidth-hv_BBoxCol2)-1;
          hv_BBoxCol2Mirror = (hv_ImageWidth-hv_BBoxCol1)-1;
          hv_BBoxCol1 = hv_BBoxCol1Mirror;
          hv_BBoxCol2 = hv_BBoxCol2Mirror;
        }
        else if (0 != hv_Rectangle2ParamExist)
        {
          hv_BBoxCol = (hv_ImageWidth-hv_BBoxCol)-1;
          //Check that BBoxPhi is only mirrored for classes with orientation.
          if (0 != (HTuple(hv_ClassIDsNoOrientationExist[1])))
          {
            {
            HTuple end_val414 = (hv_BBoxLabelID.TupleLength())-1;
            HTuple step_val414 = 1;
            for (hv_ObjIdx=0; hv_ObjIdx.Continue(end_val414, step_val414); hv_ObjIdx += step_val414)
            {
              if (0 != (int((hv_ClassIDsNoOrientation.TupleFind(HTuple(hv_BBoxLabelID[hv_ObjIdx])))==-1)))
              {
                if (0 != (HTuple(hv_IgnoreDirection[1])))
                {
                  hv_BBoxPhi[hv_ObjIdx] = -HTuple(hv_BBoxPhi[hv_ObjIdx]);
                }
                else
                {
                  hv_BBoxPhi[hv_ObjIdx] = (((-(HTuple(hv_BBoxPhi[hv_ObjIdx]).TupleLessElem(0.0)))+(HTuple(hv_BBoxPhi[hv_ObjIdx]).TupleGreaterEqualElem(0.0)))*(HTuple(180).TupleRad()))-HTuple(hv_BBoxPhi[hv_ObjIdx]);
                }
              }
            }
            }
          }
          else
          {
            if (0 != (HTuple(hv_IgnoreDirection[1])))
            {
              hv_BBoxPhi = -hv_BBoxPhi;
            }
            else
            {
              hv_BBoxPhi = (((-(hv_BBoxPhi.TupleLessElem(0.0)))+(hv_BBoxPhi.TupleGreaterEqualElem(0.0)))*(HTuple(180).TupleRad()))-hv_BBoxPhi;
            }
          }
        }
        if (0 != hv_SegmentationImageExists)
        {
          MirrorImage(ho_SegmentationImage, &ho_SegmentationImage, "column");
        }
        if (0 != hv_WeightImageExists)
        {
          MirrorImage(ho_WeightImage, &ho_WeightImage, "column");
        }
      }
      //
      if (0 != (hv_StrMirror.TupleRegexpTest("r")))
      {
        MirrorImage(ho_Image, &ho_Image, "row");
        if (0 != hv_Rectangle1ParamExist)
        {
          hv_BBoxRow1Mirror = (hv_ImageHeight-hv_BBoxRow2)-1;
          hv_BBoxRow2Mirror = (hv_ImageHeight-hv_BBoxRow1)-1;
          hv_BBoxRow1 = hv_BBoxRow1Mirror;
          hv_BBoxRow2 = hv_BBoxRow2Mirror;
        }
        else if (0 != hv_Rectangle2ParamExist)
        {
          hv_BBoxRow = (hv_ImageHeight-hv_BBoxRow)-1;
          if (0 != (HTuple(hv_ClassIDsNoOrientationExist[1])))
          {
            {
            HTuple end_val449 = (hv_BBoxLabelID.TupleLength())-1;
            HTuple step_val449 = 1;
            for (hv_ObjIdx=0; hv_ObjIdx.Continue(end_val449, step_val449); hv_ObjIdx += step_val449)
            {
              if (0 != (int((hv_ClassIDsNoOrientation.TupleFind(HTuple(hv_BBoxLabelID[hv_ObjIdx])))==-1)))
              {
                hv_BBoxPhi[hv_ObjIdx] = -HTuple(hv_BBoxPhi[hv_ObjIdx]);
              }
            }
            }
          }
          else
          {
            hv_BBoxPhi = -hv_BBoxPhi;
          }
        }
        if (0 != hv_SegmentationImageExists)
        {
          MirrorImage(ho_SegmentationImage, &ho_SegmentationImage, "row");
        }
        if (0 != hv_WeightImageExists)
        {
          MirrorImage(ho_WeightImage, &ho_WeightImage, "row");
        }
      }
      //
      //Set the mirrored data to DLSample.
      SetDictObject(ho_Image, hv_DLSample, "image");
      if (0 != hv_Rectangle1ParamExist)
      {
        SetDictTuple(hv_DLSample, "bbox_col1", hv_BBoxCol1);
        SetDictTuple(hv_DLSample, "bbox_row1", hv_BBoxRow1);
        SetDictTuple(hv_DLSample, "bbox_col2", hv_BBoxCol2);
        SetDictTuple(hv_DLSample, "bbox_row2", hv_BBoxRow2);
      }
      else if (0 != hv_Rectangle2ParamExist)
      {
        SetDictTuple(hv_DLSample, "bbox_row", hv_BBoxRow);
        SetDictTuple(hv_DLSample, "bbox_col", hv_BBoxCol);
        SetDictTuple(hv_DLSample, "bbox_phi", hv_BBoxPhi);
      }
      if (0 != hv_SegmentationImageExists)
      {
        SetDictObject(ho_SegmentationImage, hv_DLSample, "segmentation_image");
      }
      if (0 != hv_WeightImageExists)
      {
        SetDictObject(ho_WeightImage, hv_DLSample, "weight_image");
      }
    }
    else if (0 != (int(HTuple(hv_CurrentDistortion[0])==HTuple(hv_BrightnessVariation[0]))))
    {
      //Brightness variation:
      //Add random brightness variation.
      hv_BrightnessVariationValue = ((HTuple::TupleRand(1)*2)-1)*HTuple(hv_CurrentDistortion[1]);
      ScaleImage(ho_Image, &ho_ImageScaled, 1.0, hv_BrightnessVariationValue);
      //
      //Set the augmented image to DLSample.
      SetDictObject(ho_ImageScaled, hv_DLSample, "image");
    }
    else if (0 != (int(HTuple(hv_CurrentDistortion[0])==HTuple(hv_BrightnessVariationSpot[0]))))
    {
      GetImageSize(ho_Image, &hv_ImageWidth, &hv_ImageHeight);
      //Determine random brightness variation.
      hv_BrightnessVariationValue = ((HTuple::TupleRand(1)*2)-1)*HTuple(hv_CurrentDistortion[1]);
      //Determine random spot size between [0.5*ImageHeight, ImageWidth]
      hv_SpotSize = hv_ImageWidth*((HTuple::TupleRand(1)/2)+0.5);
      //Determine random spot position.
      hv_SpotRow = HTuple::TupleRand(1)*hv_ImageHeight;
      hv_SpotColumn = HTuple::TupleRand(1)*hv_ImageWidth;
      //
      if (0 != (int(hv_BrightnessVariationValue<0)))
      {
        hv_Direction = 0;
        hv_BrightnessVariationValue = -hv_BrightnessVariationValue;
      }
      else
      {
        hv_Direction = 1;
      }
      //Generate Gauss filter that simulates an illumination spot of size 'SpotSize'.
      GenGaussFilter(&ho_Filter, 1, 1, 0, "none", "dc_center", hv_SpotSize, hv_SpotSize);
      //Shift the filter image to the given position.
      hv_ShiftRow = -((hv_SpotSize/2)-hv_SpotRow);
      hv_ShiftCol = -((hv_SpotSize/2)-hv_SpotColumn);
      TileImagesOffset(ho_Filter, &ho_GaussImage, hv_ShiftRow, hv_ShiftCol, -1, -1, 
          -1, -1, hv_ImageWidth, hv_ImageHeight);
      FullDomain(ho_GaussImage, &ho_GaussFilter);
      //Convert Gauss filter to target image type and apply brightness variation.
      GetImageType(ho_Image, &hv_Type);
      ScaleImage(ho_GaussFilter, &ho_Gauss, hv_BrightnessVariationValue, 0);
      ConvertImageType(ho_Gauss, &ho_GaussTargetType, hv_Type);
      //Add channels to fit input image.
      CountChannels(ho_Image, &hv_NChannels);
      CopyObj(ho_GaussTargetType, &ho_AddImage, 1, 1);
      {
      HTuple end_val522 = hv_NChannels-1;
      HTuple step_val522 = 1;
      for (hv_Index1=1; hv_Index1.Continue(end_val522, step_val522); hv_Index1 += step_val522)
      {
        AppendChannel(ho_AddImage, ho_GaussTargetType, &ho_AddImage);
      }
      }
      //Apply on image.
      if (0 != hv_Direction)
      {
        AddImage(ho_Image, ho_AddImage, &ho_ImageSpot, 1, 0);
      }
      else
      {
        SubImage(ho_Image, ho_AddImage, &ho_ImageSpot, 1, 0);
      }
      //
      //Set the augmented image to DLSample.
      SetDictObject(ho_ImageSpot, hv_DLSample, "image");
    }
    else if (0 != (int(HTuple(hv_CurrentDistortion[0])==HTuple(hv_CropPercentage[0]))))
    {
      GetImageSize(ho_Image, &hv_ImageWidth, &hv_ImageHeight);
      //Define cropping rectangle.
      hv_CropRate = HTuple(hv_CurrentDistortion[1])*0.01;
      hv_Row1 = (((1-hv_CropRate)*hv_ImageHeight)*HTuple::TupleRand(1)).TupleFloor();
      hv_Row2 = hv_Row1+(hv_CropRate*hv_ImageHeight);
      hv_Column1 = (((1-hv_CropRate)*hv_ImageWidth)*HTuple::TupleRand(1)).TupleFloor();
      hv_Column2 = hv_Column1+(hv_CropRate*hv_ImageWidth);
      //Crop the image.
      CropRectangle1(ho_Image, &ho_ImagePart, hv_Row1, hv_Column1, hv_Row2, hv_Column2);
      //Scale image to the input size and set the augmented image to DLSample.
      ZoomImageSize(ho_ImagePart, &ho_ImagePart, hv_ImageWidth, hv_ImageHeight, "constant");
      SetDictObject(ho_ImagePart, hv_DLSample, "image");
    }
    else if (0 != (int(HTuple(hv_CurrentDistortion[0])==HTuple(hv_CropPixel[0]))))
    {
      GetImageSize(ho_Image, &hv_ImageWidth, &hv_ImageHeight);
      //Define cropping rectangle.
      hv_Length = ((const HTuple&)hv_CurrentDistortion)[1];
      hv_Row1 = HTuple::TupleRand(1)*(hv_ImageHeight-hv_Length);
      hv_Row2 = (hv_Row1+hv_Length)-1;
      hv_Column1 = HTuple::TupleRand(1)*(hv_ImageWidth-hv_Length);
      hv_Column2 = (hv_Column1+hv_Length)-1;
      //Crop the image.
      CropRectangle1(ho_Image, &ho_ImagePart, hv_Row1, hv_Column1, hv_Row2, hv_Column2);
      //Scale the image to the input size and set the augmented image to DLSample.
      ZoomImageSize(ho_ImagePart, &ho_ImagePart, hv_ImageWidth, hv_ImageHeight, "constant");
      SetDictObject(ho_ImagePart, hv_DLSample, "image");
    }
    else if (0 != (int(HTuple(hv_CurrentDistortion[0])==HTuple(hv_RotationRange[0]))))
    {
      GetImageSize(ho_Image, &hv_ImageWidth, &hv_ImageHeight);
      //Determine rotation angle for method 'rotate_range' (angle in range [0:1:CurrentDistortion[1])).
      hv_RotationStep = 1;
      hv_NumPossibleRotations = ((const HTuple&)hv_CurrentDistortion)[1];
      hv_CurrentRotation = hv_RotationStep*(((hv_NumPossibleRotations*HTuple::TupleRand(1)).TupleInt())+1);
      //Select direction of rotation randomly.
      if (0 != (int((HTuple::TupleRand(1).TupleRound())>0.5)))
      {
        hv_CurrentRotation = 360.0-hv_CurrentRotation;
      }
      if (0 != (HTuple(int((hv_CurrentRotation.TupleInt())==hv_CurrentRotation)).TupleAnd(int(((hv_CurrentRotation.TupleInt())%90)==0))))
      {
        //Rotations around 90 degrees are faster with rotate_image.
        RotateImage(ho_Image, &ho_ImagePart, hv_CurrentRotation.TupleInt(), "constant");
      }
      else
      {
        //Create rotation matrix.
        HomMat2dIdentity(&hv_HomMat2DIdentity);
        HomMat2dRotate(hv_HomMat2DIdentity, hv_CurrentRotation.TupleRad(), hv_ImageHeight/2.0, 
            hv_ImageWidth/2.0, &hv_HomMat2DRotate);
        //Apply rotation.
        AffineTransImage(ho_Image, &ho_ImageRotated, hv_HomMat2DRotate, "constant", 
            "false");
        //Remove potential undefined domain.
        GetDomain(ho_ImageRotated, &ho_DomainRotated);
        InnerRectangle1(ho_DomainRotated, &hv_Row1, &hv_Column1, &hv_Row2, &hv_Column2);
        CropRectangle1(ho_ImageRotated, &ho_ImagePart, hv_Row1, hv_Column1, hv_Row2, 
            hv_Column2);
        //Scale image to the input size.
        ZoomImageSize(ho_ImagePart, &ho_ImagePart, hv_ImageWidth, hv_ImageHeight, 
            "constant");
      }
      //
      //Set the augmented image to DLSample.
      SetDictObject(ho_ImagePart, hv_DLSample, "image");
    }
  }
  }
  return;
}

// Chapter: Deep Learning / Semantic Segmentation
// Short Description: Calculate the class weights for a semantic segmentation dataset. 
void calculate_dl_segmentation_class_weights (HTuple hv_DLDataset, HTuple hv_MaxWeight, 
    HTuple hv_IgnoreClassIDs, HTuple *hv_ClassWeights)
{

  // Local iconic variables
  HObject  ho_SegmentationImage;

  // Local control variables
  HTuple  hv_KeysExists, hv_DLSamples, hv_SampleIndices;
  HTuple  hv_ClassIDs, hv_NumClasses, hv_ClassIDsToClassIdx;
  HTuple  hv_ClassAreas, hv_SampleIndex, hv_DLSample, hv_ImageType;
  HTuple  hv_AbsoluteHisto, hv_TotalArea, hv_ValidClasses;
  HTuple  hv_ClassFreq, hv_IndicesToClip;

  //
  //This procedure calculates a weight for each class that is present in the Dataset.
  //The class weights are calculated according to the inverse class frequencies
  //in the training dataset.
  //Therefore, the dataset has to be split before calling this procedure.
  //
  //Check if the input is correct.
  GetDictParam(hv_DLDataset, "key_exists", (HTuple("samples").Append("class_ids")), 
      &hv_KeysExists);
  if (0 != (HTuple(hv_KeysExists[0]).TupleNot()))
  {
    throw HException("DLDataset must contain a key-value pair for 'samples'");
  }
  if (0 != (HTuple(hv_KeysExists[1]).TupleNot()))
  {
    throw HException("DLDataset must contain a key-value pair for 'class_ids'");
  }
  if (0 != (int(hv_MaxWeight<=0)))
  {
    throw HException("MaxWeight must be greater than 0");
  }
  //
  //Get the samples of the dataset.
  GetDictTuple(hv_DLDataset, "samples", &hv_DLSamples);
  //Get the train samples.
  find_dl_samples(hv_DLSamples, "split", "train", "match", &hv_SampleIndices);
  if (0 != (int((hv_SampleIndices.TupleLength())==0)))
  {
    throw HException("The DLDataset does not contain any samples with value 'train' for key 'split'");
  }
  //
  //Get the class IDs of the dataset.
  GetDictTuple(hv_DLDataset, "class_ids", &hv_ClassIDs);
  hv_NumClasses = hv_ClassIDs.TupleLength();
  //
  //Define mapping from class ID to class index.
  hv_ClassIDsToClassIdx = HTuple((hv_ClassIDs.TupleMax())+1,-1);
  hv_ClassIDsToClassIdx[hv_ClassIDs] = HTuple::TupleGenSequence(0,(hv_ClassIDs.TupleLength())-1,1);
  //
  //We want to collect the number of pixels for each class.
  hv_ClassAreas = HTuple(hv_ClassIDs.TupleLength(),0);
  //
  //Loop over the samples.
  {
  HTuple end_val38 = (hv_SampleIndices.TupleLength())-1;
  HTuple step_val38 = 1;
  for (hv_SampleIndex=0; hv_SampleIndex.Continue(end_val38, step_val38); hv_SampleIndex += step_val38)
  {
    //
    //Read the sample.
    read_dl_samples(hv_DLDataset, HTuple(hv_SampleIndices[hv_SampleIndex]), &hv_DLSample);
    //
    //Get the segmentation image.
    GetDictObject(&ho_SegmentationImage, hv_DLSample, "segmentation_image");
    //
    //Convert the segmentation image if necessary.
    GetImageType(ho_SegmentationImage, &hv_ImageType);
    if (0 != (int(((((HTuple("int1").Append("int2")).Append("uint2")).Append("byte")).TupleFind(hv_ImageType))==-1)))
    {
      ConvertImageType(ho_SegmentationImage, &ho_SegmentationImage, "uint2");
    }
    //
    //Get the number of pixels for each class.
    GrayHistoAbs(ho_SegmentationImage, ho_SegmentationImage, 1, &hv_AbsoluteHisto);
    //
    //Accumulate the areas.
    hv_ClassAreas += HTuple(hv_AbsoluteHisto[hv_ClassIDs]);
  }
  }
  //
  //Get the total number of pixels without the area of ignore classes.
  hv_ClassAreas[HTuple(hv_ClassIDsToClassIdx[hv_IgnoreClassIDs])] = 0;
  hv_TotalArea = hv_ClassAreas.TupleSum();
  //
  //Calculate the inverse class frequencies.
  (*hv_ClassWeights) = HTuple(hv_ClassIDs.TupleLength(),0.);
  hv_ValidClasses = (hv_ClassAreas.TupleNotEqualElem(0)).TupleFind(1);
  hv_ClassFreq = hv_ClassAreas/(hv_TotalArea.TupleReal());
  (*hv_ClassWeights)[hv_ValidClasses] = 1./(HTuple(hv_ClassFreq[hv_ValidClasses])+0.0001);
  //
  //Scale the weights to obtain a final output of 1.0 for the most frequent class.
  (*hv_ClassWeights) = (*hv_ClassWeights)/(HTuple((*hv_ClassWeights)[hv_ValidClasses]).TupleMin());
  //Clip the weights.
  hv_IndicesToClip = ((*hv_ClassWeights).TupleGreaterElem(hv_MaxWeight)).TupleFind(1);
  if (0 != (int(hv_IndicesToClip!=-1)))
  {
    (*hv_ClassWeights)[hv_IndicesToClip] = hv_MaxWeight;
  }

  return;
}

// Chapter: Deep Learning / Evaluation
// Short Description: Calculate evaluation measures based on the values of RunningMeasures and the settings in EvalParams. 
void calculate_evaluation_measures (HTuple hv_RunningMeasures, HTuple hv_EvalParams, 
    HTuple *hv_EvaluationResult)
{

  // Local control variables
  HTuple  hv_EvaluationType;

  //
  //This procedure calculates the final measures depending on the evaluation type.
  //
  GetDictTuple(hv_EvalParams, "evaluation_type", &hv_EvaluationType);
  if (0 != (int(hv_EvaluationType==HTuple("anomaly_detection"))))
  {
    calculate_image_anomaly_measures(hv_RunningMeasures, hv_EvalParams, &(*hv_EvaluationResult));
  }
  else if (0 != (int(hv_EvaluationType==HTuple("classification"))))
  {
    calculate_image_classification_measures(hv_RunningMeasures, hv_EvalParams, &(*hv_EvaluationResult));
  }
  else if (0 != (int(hv_EvaluationType==HTuple("detection"))))
  {
    calculate_instance_measures(hv_RunningMeasures, hv_EvalParams, &(*hv_EvaluationResult));
  }
  else if (0 != (int(hv_EvaluationType==HTuple("segmentation"))))
  {
    calculate_pixel_measures(hv_RunningMeasures, hv_EvalParams, &(*hv_EvaluationResult));
  }
  //
  return;
}

// Chapter: Deep Learning / Anomaly Detection
// Short Description: Calculate anomaly detection measures based on RunningMeasures. 
void calculate_image_anomaly_measures (HTuple hv_RunningMeasures, HTuple hv_EvalParams, 
    HTuple *hv_EvaluationResult)
{

  // Local iconic variables

  // Local control variables
  HTuple  hv_CalcAnomalyHistogram, hv_CalcPrecision;
  HTuple  hv_CalcRecall, hv_CalcAbsoluteConfusionMatrix, hv_CalcRelativeConfusionMatrix;
  HTuple  hv_MeasuresExists, hv_Measures, hv_M, hv_AnomalyClassificationThresholdExists;
  HTuple  hv_AnomalyClassificationThreshold, hv_ImageIDs;
  HTuple  hv_AnomalyLabelIDs, hv_AnomalyScores, hv_OKIndices;
  HTuple  hv_NOKIndices, hv_HistoOKXValues, hv_HistoOKYValues;
  HTuple  hv_NumOKEvalData, hv_ImageLevelScoresOK, hv_HistoNOKXValues;
  HTuple  hv_HistoNOKYValues, hv_NumNOKEvalData, hv_ImageLevelScoresNOK;
  HTuple  hv_ScoreHistogram, hv_NumClasses, hv_ClassIDs, hv_AllPredictions;
  HTuple  hv_IndThreshold, hv_CurrentThresholdValue, hv_CurrentThresholdKey;
  HTuple  hv_Predictions, hv_AbsoluteConfustionMatrices, hv_AbsoluteConfusionMatrix;
  HTuple  hv_Rows, hv_Columns, hv_Value, hv_AbsoluteConfusionMatrixDictionary;
  HTuple  hv_RelativeConfustionMatrices, hv_RelativeConfusionMatrix;
  HTuple  hv_RelativeConfusionMatrixDictionary, hv_GlobalEvaluation;
  HTuple  hv_AllClassPrecisions, hv_AllMeanPrecisions, hv_AbsoluteConfusionMatrices;
  HTuple  hv_ClassPrecisions, hv_MatrixRowSumID, hv_MatrixColumnSumID;
  HTuple  hv_Index, hv_TruePositive, hv_SumPredictedClass;
  HTuple  hv_SumLabel, hv_ClassPrecision, hv_ValidClassPrecisions;
  HTuple  hv_MeanPrecision, hv_AllClassRecalls, hv_AllMeanRecalls;
  HTuple  hv_ClassRecalls, hv_ClassRecall, hv_ValidClassRecalls;
  HTuple  hv_MeanRecall;

  //
  //This procedure calculates the final summarizing image anomaly detection measures based on the running measures.
  //
  hv_CalcAnomalyHistogram = 1;
  hv_CalcPrecision = 0;
  hv_CalcRecall = 0;
  hv_CalcAbsoluteConfusionMatrix = 0;
  hv_CalcRelativeConfusionMatrix = 0;
  GetDictParam(hv_EvalParams, "key_exists", "measures", &hv_MeasuresExists);
  if (0 != hv_MeasuresExists)
  {
    GetDictTuple(hv_EvalParams, "measures", &hv_Measures);
    {
    HTuple end_val11 = (hv_Measures.TupleLength())-1;
    HTuple step_val11 = 1;
    for (hv_M=0; hv_M.Continue(end_val11, step_val11); hv_M += step_val11)
    {
      if (0 != (int(HTuple(hv_Measures[hv_M])==HTuple("anomaly_score_histogram"))))
      {
        //The default, just here for consistency.
        hv_CalcAnomalyHistogram = 1;
      }
      else if (0 != (int(HTuple(hv_Measures[hv_M])==HTuple("precision"))))
      {
        hv_CalcPrecision = 1;
        hv_CalcAbsoluteConfusionMatrix = 1;
      }
      else if (0 != (int(HTuple(hv_Measures[hv_M])==HTuple("recall"))))
      {
        hv_CalcRecall = 1;
        hv_CalcAbsoluteConfusionMatrix = 1;
      }
      else if (0 != (int(HTuple(hv_Measures[hv_M])==HTuple("absolute_confusion_matrix"))))
      {
        hv_CalcAbsoluteConfusionMatrix = 1;
      }
      else if (0 != (int(HTuple(hv_Measures[hv_M])==HTuple("relative_confusion_matrix"))))
      {
        hv_CalcRelativeConfusionMatrix = 1;
      }
      else if (0 != (int(HTuple(hv_Measures[hv_M])==HTuple("all"))))
      {
        hv_CalcPrecision = 1;
        hv_CalcRecall = 1;
        hv_CalcAbsoluteConfusionMatrix = 1;
        hv_CalcRelativeConfusionMatrix = 1;
      }
      else
      {
        throw HException(("Unknown Measure: "+HTuple(hv_Measures[hv_M]))+".");
      }
    }
    }
  }
  //
  GetDictParam(hv_EvalParams, "key_exists", "anomaly_classification_thresholds", 
      &hv_AnomalyClassificationThresholdExists);
  if (0 != hv_AnomalyClassificationThresholdExists)
  {
    GetDictTuple(hv_EvalParams, "anomaly_classification_thresholds", &hv_AnomalyClassificationThreshold);
  }
  else if (0 != (hv_CalcRelativeConfusionMatrix.TupleOr(hv_CalcAbsoluteConfusionMatrix)))
  {
    throw HException("A threshold value is needed to calculate a confusion matrix.");
  }
  //
  //Get and check values in RunningMeasures.
  //
  //Get image ids.
  GetDictTuple(hv_RunningMeasures, "image_ids", &hv_ImageIDs);
  //Get anomaly ids.
  GetDictTuple(hv_RunningMeasures, "anomaly_label_ids", &hv_AnomalyLabelIDs);
  //Get image scores.
  GetDictTuple(hv_RunningMeasures, "anomaly_scores", &hv_AnomalyScores);
  //
  //Calculate histograms.
  //
  //Find scores of 'ok' and 'nok' images.
  hv_OKIndices = hv_AnomalyLabelIDs.TupleFind(0);
  hv_NOKIndices = hv_AnomalyLabelIDs.TupleFind(1);
  if (0 != (HTuple(int(hv_OKIndices==-1)).TupleAnd(int(hv_NOKIndices==-1))))
  {
    throw HException("No data available for evaluation");
  }
  //
  //Calculate histogram for 'ok' images.
  hv_HistoOKXValues = HTuple();
  hv_HistoOKYValues = HTuple();
  hv_NumOKEvalData = 0;
  if (0 != (int(hv_OKIndices!=-1)))
  {
    hv_NumOKEvalData = hv_OKIndices.TupleLength();
    hv_ImageLevelScoresOK = HTuple(hv_AnomalyScores[hv_OKIndices]);
    hv_HistoOKXValues = hv_ImageLevelScoresOK.TupleSort();
    hv_HistoOKYValues = (HTuple::TupleGenSequence(hv_ImageLevelScoresOK.TupleLength(),1,-1).TupleReal())/(hv_ImageLevelScoresOK.TupleLength());
  }
  //
  //Calculate histogram for 'nok' images.
  hv_HistoNOKXValues = HTuple();
  hv_HistoNOKYValues = HTuple();
  hv_NumNOKEvalData = 0;
  if (0 != (int(hv_NOKIndices!=-1)))
  {
    hv_NumNOKEvalData = hv_NOKIndices.TupleLength();
    hv_ImageLevelScoresNOK = HTuple(hv_AnomalyScores[hv_NOKIndices]);
    hv_HistoNOKXValues = hv_ImageLevelScoresNOK.TupleSort();
    hv_HistoNOKYValues = (HTuple::TupleGenSequence(1,hv_ImageLevelScoresNOK.TupleLength(),1).TupleReal())/(hv_ImageLevelScoresNOK.TupleLength());
  }
  //
  //Create dictionary for the score histogram.
  CreateDict(&hv_ScoreHistogram);
  SetDictTuple(hv_ScoreHistogram, "ok_x", hv_HistoOKXValues);
  SetDictTuple(hv_ScoreHistogram, "ok_y", hv_HistoOKYValues);
  SetDictTuple(hv_ScoreHistogram, "nok_x", hv_HistoNOKXValues);
  SetDictTuple(hv_ScoreHistogram, "nok_y", hv_HistoNOKYValues);
  //
  //Set the score histogram in the results dictionary.
  CreateDict(&(*hv_EvaluationResult));
  SetDictTuple((*hv_EvaluationResult), "anomaly_score_histogram", hv_ScoreHistogram);
  //
  //Get Predictions according to given Threshold value(s).
  //Remember, precision and recall base on the absolute confusion matrix.
  if (0 != (hv_CalcAbsoluteConfusionMatrix.TupleOr(hv_CalcRelativeConfusionMatrix)))
  {
    GetDictTuple(hv_EvalParams, "num_classes", &hv_NumClasses);
    GetDictTuple(hv_EvalParams, "class_ids", &hv_ClassIDs);
    CreateDict(&hv_AllPredictions);
    {
    HTuple end_val100 = (hv_AnomalyClassificationThreshold.TupleLength())-1;
    HTuple step_val100 = 1;
    for (hv_IndThreshold=0; hv_IndThreshold.Continue(end_val100, step_val100); hv_IndThreshold += step_val100)
    {
      hv_CurrentThresholdValue = HTuple(hv_AnomalyClassificationThreshold[hv_IndThreshold]);
      hv_CurrentThresholdKey = hv_IndThreshold.TupleString(".3d");
      hv_Predictions = hv_AnomalyScores.TupleGreaterEqualElem(hv_CurrentThresholdValue);
      SetDictTuple(hv_AllPredictions, hv_CurrentThresholdKey, hv_Predictions);
    }
    }
  }
  //
  //Calculate absolute confusion matrix.
  if (0 != hv_CalcAbsoluteConfusionMatrix)
  {
    hv_AbsoluteConfustionMatrices = HTuple();
    {
    HTuple end_val111 = (hv_AnomalyClassificationThreshold.TupleLength())-1;
    HTuple step_val111 = 1;
    for (hv_IndThreshold=0; hv_IndThreshold.Continue(end_val111, step_val111); hv_IndThreshold += step_val111)
    {
      hv_CurrentThresholdValue = HTuple(hv_AnomalyClassificationThreshold[hv_IndThreshold]);
      hv_CurrentThresholdKey = hv_IndThreshold.TupleString(".3d");
      GetDictTuple(hv_AllPredictions, hv_CurrentThresholdKey, &hv_Predictions);
      gen_confusion_matrix(hv_AnomalyLabelIDs, hv_Predictions, (HTuple("display_matrix").Append("return_matrix")), 
          (HTuple("none").Append("absolute")), HTuple(), &hv_AbsoluteConfusionMatrix);
      GetSizeMatrix(hv_AbsoluteConfusionMatrix, &hv_Rows, &hv_Columns);
      if (0 != (HTuple(HTuple(int(hv_NumOKEvalData<=0)).TupleOr(int(hv_NumNOKEvalData<=0))).TupleAnd(HTuple(int(hv_Rows<2)).TupleOr(int(hv_Columns<2)))))
      {
        //Patch matrix to 2x2 in case only 'ok' or only 'nok'
        //data is used for evaluation.
        GetValueMatrix(hv_AbsoluteConfusionMatrix, 0, 0, &hv_Value);
        CreateMatrix(2, 2, 0, &hv_AbsoluteConfusionMatrix);
        if (0 != (int(hv_NumOKEvalData<=0)))
        {
          SetValueMatrix(hv_AbsoluteConfusionMatrix, 0, 1, (hv_Predictions.TupleLength())-hv_Value);
          SetValueMatrix(hv_AbsoluteConfusionMatrix, 1, 1, hv_Value);
        }
        if (0 != (int(hv_NumNOKEvalData<=0)))
        {
          SetValueMatrix(hv_AbsoluteConfusionMatrix, 0, 0, hv_Value);
          SetValueMatrix(hv_AbsoluteConfusionMatrix, 1, 0, (hv_Predictions.TupleLength())-hv_Value);
        }
      }
      CreateDict(&hv_AbsoluteConfusionMatrixDictionary);
      SetDictTuple(hv_AbsoluteConfusionMatrixDictionary, "confusion_matrix", hv_AbsoluteConfusionMatrix);
      SetDictTuple(hv_AbsoluteConfusionMatrixDictionary, "threshold", hv_CurrentThresholdValue);
      hv_AbsoluteConfustionMatrices[hv_IndThreshold] = hv_AbsoluteConfusionMatrixDictionary;
    }
    }
    SetDictTuple((*hv_EvaluationResult), "absolute_confusion_matrix", hv_AbsoluteConfustionMatrices);
  }
  //
  //Calculate relative confusion matrix.
  if (0 != hv_CalcRelativeConfusionMatrix)
  {
    hv_RelativeConfustionMatrices = HTuple();
    {
    HTuple end_val142 = (hv_AnomalyClassificationThreshold.TupleLength())-1;
    HTuple step_val142 = 1;
    for (hv_IndThreshold=0; hv_IndThreshold.Continue(end_val142, step_val142); hv_IndThreshold += step_val142)
    {
      hv_CurrentThresholdValue = HTuple(hv_AnomalyClassificationThreshold[hv_IndThreshold]);
      hv_CurrentThresholdKey = hv_IndThreshold.TupleString(".3d");
      GetDictTuple(hv_AllPredictions, hv_CurrentThresholdKey, &hv_Predictions);
      gen_confusion_matrix(hv_AnomalyLabelIDs, hv_Predictions, (HTuple("display_matrix").Append("return_matrix")), 
          (HTuple("none").Append("relative")), HTuple(), &hv_RelativeConfusionMatrix);
      GetSizeMatrix(hv_RelativeConfusionMatrix, &hv_Rows, &hv_Columns);
      if (0 != (HTuple(HTuple(int(hv_NumOKEvalData<=0)).TupleOr(int(hv_NumNOKEvalData<=0))).TupleAnd(HTuple(int(hv_Rows<2)).TupleOr(int(hv_Columns<2)))))
      {
        //Patch matrix to 2x2 in case only 'ok' or only 'nok'
        //data is used for evaluation.
        GetValueMatrix(hv_RelativeConfusionMatrix, 0, 0, &hv_Value);
        CreateMatrix(2, 2, 0, &hv_RelativeConfusionMatrix);
        if (0 != (int(hv_NumOKEvalData<=0)))
        {
          SetValueMatrix(hv_RelativeConfusionMatrix, 0, 1, 1.0-hv_Value);
          SetValueMatrix(hv_RelativeConfusionMatrix, 1, 1, hv_Value);
        }
        if (0 != (int(hv_NumNOKEvalData<=0)))
        {
          SetValueMatrix(hv_RelativeConfusionMatrix, 0, 0, hv_Value);
          SetValueMatrix(hv_RelativeConfusionMatrix, 1, 0, 1.0-hv_Value);
        }
      }
      CreateDict(&hv_RelativeConfusionMatrixDictionary);
      SetDictTuple(hv_RelativeConfusionMatrixDictionary, "confusion_matrix", hv_RelativeConfusionMatrix);
      SetDictTuple(hv_RelativeConfusionMatrixDictionary, "threshold", hv_CurrentThresholdValue);
      hv_RelativeConfustionMatrices[hv_IndThreshold] = hv_RelativeConfusionMatrixDictionary;
    }
    }
    SetDictTuple((*hv_EvaluationResult), "relative_confusion_matrix", hv_RelativeConfustionMatrices);
  }
  //
  if (0 != (hv_CalcPrecision.TupleOr(hv_CalcRecall)))
  {
    CreateDict(&hv_GlobalEvaluation);
  }
  //Calculate precision.
  if (0 != hv_CalcPrecision)
  {
    hv_AllClassPrecisions = HTuple();
    hv_AllMeanPrecisions = HTuple();
    GetDictTuple((*hv_EvaluationResult), "absolute_confusion_matrix", &hv_AbsoluteConfusionMatrices);
    {
    HTuple end_val178 = (hv_AbsoluteConfusionMatrices.TupleLength())-1;
    HTuple step_val178 = 1;
    for (hv_IndThreshold=0; hv_IndThreshold.Continue(end_val178, step_val178); hv_IndThreshold += step_val178)
    {
      hv_AbsoluteConfusionMatrixDictionary = HTuple(hv_AbsoluteConfusionMatrices[hv_IndThreshold]);
      GetDictTuple(hv_AbsoluteConfusionMatrixDictionary, "confusion_matrix", &hv_AbsoluteConfusionMatrix);
      hv_ClassPrecisions = HTuple();
      SumMatrix(hv_AbsoluteConfusionMatrix, "rows", &hv_MatrixRowSumID);
      SumMatrix(hv_AbsoluteConfusionMatrix, "columns", &hv_MatrixColumnSumID);
      {
      HTuple end_val184 = hv_NumClasses-1;
      HTuple step_val184 = 1;
      for (hv_Index=0; hv_Index.Continue(end_val184, step_val184); hv_Index += step_val184)
      {
        //Compute the precision for every selected class.
        GetValueMatrix(hv_AbsoluteConfusionMatrix, HTuple(hv_ClassIDs[hv_Index]), 
            HTuple(hv_ClassIDs[hv_Index]), &hv_TruePositive);
        GetValueMatrix(hv_MatrixRowSumID, HTuple(hv_ClassIDs[hv_Index]), 0, &hv_SumPredictedClass);
        GetValueMatrix(hv_MatrixColumnSumID, 0, HTuple(hv_ClassIDs[hv_Index]), &hv_SumLabel);
        if (0 != (int(hv_SumLabel<=0)))
        {
          //Invalid per-class precision.
          hv_ClassPrecision = -1.0;
        }
        else if (0 != (int(hv_SumPredictedClass==0)))
        {
          hv_ClassPrecision = 0.0;
        }
        else
        {
          hv_ClassPrecision = hv_TruePositive/hv_SumPredictedClass;
        }
        hv_ClassPrecisions = hv_ClassPrecisions.TupleConcat(hv_ClassPrecision);
      }
      }
      TupleSelectMask(hv_ClassPrecisions, hv_ClassPrecisions.TupleGreaterEqualElem(0.0), 
          &hv_ValidClassPrecisions);
      hv_MeanPrecision = hv_ValidClassPrecisions.TupleMean();
      hv_AllClassPrecisions = hv_AllClassPrecisions.TupleConcat(hv_ClassPrecisions);
      hv_AllMeanPrecisions = hv_AllMeanPrecisions.TupleConcat(hv_MeanPrecision);
      ClearMatrix(hv_MatrixRowSumID);
    }
    }
    SetDictTuple(hv_GlobalEvaluation, "precision_per_class", hv_AllClassPrecisions);
    SetDictTuple(hv_GlobalEvaluation, "mean_precision", hv_AllMeanPrecisions);
  }
  //
  //Calculate recall.
  if (0 != hv_CalcRecall)
  {
    hv_AllClassRecalls = HTuple();
    hv_AllMeanRecalls = HTuple();
    GetDictTuple((*hv_EvaluationResult), "absolute_confusion_matrix", &hv_AbsoluteConfustionMatrices);
    {
    HTuple end_val214 = (hv_AbsoluteConfustionMatrices.TupleLength())-1;
    HTuple step_val214 = 1;
    for (hv_IndThreshold=0; hv_IndThreshold.Continue(end_val214, step_val214); hv_IndThreshold += step_val214)
    {
      hv_AbsoluteConfusionMatrixDictionary = HTuple(hv_AbsoluteConfustionMatrices[hv_IndThreshold]);
      GetDictTuple(hv_AbsoluteConfusionMatrixDictionary, "confusion_matrix", &hv_AbsoluteConfusionMatrix);
      hv_ClassRecalls = HTuple();
      SumMatrix(hv_AbsoluteConfusionMatrix, "columns", &hv_MatrixColumnSumID);
      {
      HTuple end_val219 = hv_NumClasses-1;
      HTuple step_val219 = 1;
      for (hv_Index=0; hv_Index.Continue(end_val219, step_val219); hv_Index += step_val219)
      {
        //Compute the recall for every selected class.
        GetValueMatrix(hv_AbsoluteConfusionMatrix, HTuple(hv_ClassIDs[hv_Index]), 
            HTuple(hv_ClassIDs[hv_Index]), &hv_TruePositive);
        GetValueMatrix(hv_MatrixColumnSumID, 0, HTuple(hv_ClassIDs[hv_Index]), &hv_SumLabel);
        if (0 != (int(hv_SumLabel==0)))
        {
          //Invalid per-class recall.
          hv_ClassRecall = -1.0;
        }
        else
        {
          hv_ClassRecall = hv_TruePositive/hv_SumLabel;
        }
        hv_ClassRecalls = hv_ClassRecalls.TupleConcat(hv_ClassRecall);
      }
      }
      TupleSelectMask(hv_ClassRecalls, hv_ClassRecalls.TupleGreaterEqualElem(0.0), 
          &hv_ValidClassRecalls);
      hv_MeanRecall = hv_ValidClassRecalls.TupleMean();
      hv_AllClassRecalls = hv_AllClassRecalls.TupleConcat(hv_ClassRecalls);
      hv_AllMeanRecalls = hv_AllMeanRecalls.TupleConcat(hv_MeanRecall);
      ClearMatrix(hv_MatrixColumnSumID);
      //
    }
    }
    SetDictTuple(hv_GlobalEvaluation, "recall_per_class", hv_AllClassRecalls);
    SetDictTuple(hv_GlobalEvaluation, "mean_recall", hv_AllMeanRecalls);
  }
  //
  if (0 != (hv_CalcPrecision.TupleOr(hv_CalcRecall)))
  {
    SetDictTuple((*hv_EvaluationResult), "global_evaluation", hv_GlobalEvaluation);
  }
  //
  return;
}

// Chapter: Deep Learning / Evaluation
// Short Description: Calculate image classification measures based on RunningMeasures. 
void calculate_image_classification_measures (HTuple hv_RunningMeasures, HTuple hv_EvalParams, 
    HTuple *hv_EvaluationResult)
{

  // Local iconic variables

  // Local control variables
  HTuple  hv_TopKErrorKs, hv_CalcPrecision, hv_CalcRecall;
  HTuple  hv_CalcFScore, hv_CalcAbsoluteConfusionMatrix, hv_CalcRelativeConfusionMatrix;
  HTuple  hv_Measures, hv_RegExpTopKError, hv_M, hv_ComputeTopKError;
  HTuple  hv_K, hv_ClassIDs, hv_KeyExists, hv_ClassesToEvaluate;
  HTuple  hv_ClassIDsToEvaluate, hv_ClassNames, hv_Index;
  HTuple  hv_Position, hv_ImageIDs, hv_ImageLabelIDs, hv_Predictions;
  HTuple  hv_TopKPredictions, hv_EvalIndex, hv_CurrentEvalClass;
  HTuple  hv_IndexClass, hv_EvaluatedSamples, hv_ConfusionMatrix;
  HTuple  hv_RelativeConfusionMatrix, hv_EvalClassID, hv_KIndex;
  HTuple  hv_Indices, hv_TopKError, hv_NumClasses, hv_ClassPrecisions;
  HTuple  hv_MatrixRowSumID, hv_TruePositive, hv_SumPredictedClass;
  HTuple  hv_ClassPrecision, hv_Precision, hv_ClassRecalls;
  HTuple  hv_MatrixColumnSumID, hv_SumLabel, hv_ClassRecall;
  HTuple  hv_Recall, hv_ClassFScores, hv_SumPrecisionRecall;
  HTuple  hv_PositiveIndices, hv_FScore, hv_KeyName;

  //
  //This procedure calculates the final summarizing image classification measures based on the running measures.
  //
  //Set default values.
  hv_TopKErrorKs = HTuple();
  hv_CalcPrecision = 0;
  hv_CalcRecall = 0;
  hv_CalcFScore = 0;
  hv_CalcAbsoluteConfusionMatrix = 0;
  hv_CalcRelativeConfusionMatrix = 0;
  //
  //Check which measures are to be calculated.
  GetDictTuple(hv_EvalParams, "measures", &hv_Measures);
  hv_RegExpTopKError = "top([0-9]+)_error";
  {
  HTuple end_val14 = (hv_Measures.TupleLength())-1;
  HTuple step_val14 = 1;
  for (hv_M=0; hv_M.Continue(end_val14, step_val14); hv_M += step_val14)
  {
    hv_ComputeTopKError = HTuple(hv_Measures[hv_M]).TupleRegexpTest("top([0-9]+)_error");
    if (0 != hv_ComputeTopKError)
    {
      hv_K = (HTuple(hv_Measures[hv_M]).TupleRegexpMatch(hv_RegExpTopKError)).TupleNumber();
      hv_TopKErrorKs = (hv_TopKErrorKs.TupleConcat(hv_K)).TupleSort();
    }
    else if (0 != (int(HTuple(hv_Measures[hv_M])==HTuple("precision"))))
    {
      hv_CalcPrecision = 1;
    }
    else if (0 != (int(HTuple(hv_Measures[hv_M])==HTuple("recall"))))
    {
      hv_CalcRecall = 1;
    }
    else if (0 != (int(HTuple(hv_Measures[hv_M])==HTuple("f_score"))))
    {
      hv_CalcFScore = 1;
    }
    else if (0 != (int(HTuple(hv_Measures[hv_M])==HTuple("absolute_confusion_matrix"))))
    {
      hv_CalcAbsoluteConfusionMatrix = 1;
    }
    else if (0 != (int(HTuple(hv_Measures[hv_M])==HTuple("relative_confusion_matrix"))))
    {
      hv_CalcRelativeConfusionMatrix = 1;
    }
    else if (0 != (int(HTuple(hv_Measures[hv_M])==HTuple("all"))))
    {
      hv_TopKErrorKs = (hv_TopKErrorKs.TupleConcat(1)).TupleSort();
      hv_CalcPrecision = 1;
      hv_CalcRecall = 1;
      hv_CalcFScore = 1;
      hv_CalcAbsoluteConfusionMatrix = 1;
      hv_CalcRelativeConfusionMatrix = 1;
    }
    else
    {
      throw HException("Unknown image classification measure: "+HTuple(hv_Measures[hv_M]));
    }
  }
  }
  //
  //Initialize output dictionary and get necessary evaluation parameters.
  CreateDict(&(*hv_EvaluationResult));
  GetDictTuple(hv_EvalParams, "class_ids", &hv_ClassIDs);
  GetDictParam(hv_EvalParams, "key_exists", "class_names_to_evaluate", &hv_KeyExists);
  if (0 != hv_KeyExists)
  {
    GetDictTuple(hv_EvalParams, "class_names_to_evaluate", &hv_ClassesToEvaluate);
    hv_ClassIDsToEvaluate = HTuple();
    GetDictTuple(hv_EvalParams, "class_names", &hv_ClassNames);
    {
    HTuple end_val49 = (hv_ClassesToEvaluate.TupleLength())-1;
    HTuple step_val49 = 1;
    for (hv_Index=0; hv_Index.Continue(end_val49, step_val49); hv_Index += step_val49)
    {
      hv_Position = ((HTuple("global").TupleConcat(hv_ClassNames)).TupleEqualElem(HTuple(hv_ClassesToEvaluate[hv_Index]))).TupleFind(1);
      if (0 != (HTuple(int(hv_Position==-1)).TupleOr(int(hv_Position==HTuple()))))
      {
        throw HException("Invalid entry in  'class_names_to_evaluate': "+HTuple((HTuple("global").TupleConcat(hv_ClassesToEvaluate))[hv_Index]));
      }
      hv_ClassIDsToEvaluate = hv_ClassIDsToEvaluate.TupleConcat(HTuple((HTuple("global").TupleConcat(hv_ClassIDs))[hv_Position]));
    }
    }
    SetDictTuple(hv_EvalParams, "class_ids_to_evaluate", hv_ClassIDsToEvaluate);
  }
  GetDictTuple(hv_EvalParams, "class_ids_to_evaluate", &hv_ClassIDsToEvaluate);
  //
  //Get and check values in RunningMeasures.
  GetDictTuple(hv_RunningMeasures, "image_ids", &hv_ImageIDs);
  GetDictTuple(hv_RunningMeasures, "image_label_ids", &hv_ImageLabelIDs);
  GetDictTuple(hv_RunningMeasures, "top1_predictions", &hv_Predictions);
  GetDictTuple(hv_RunningMeasures, "topk_predictions", &hv_TopKPredictions);
  //
  //Check if needed classes appear in image label IDs.
  //For the confusion matrices, all classes need to be represented.
  hv_CalcAbsoluteConfusionMatrix = HTuple(HTuple(hv_CalcPrecision.TupleOr(hv_CalcRecall)).TupleOr(hv_CalcFScore)).TupleOr(hv_CalcAbsoluteConfusionMatrix);
  hv_CalcRelativeConfusionMatrix = HTuple(HTuple(hv_CalcPrecision.TupleOr(hv_CalcRecall)).TupleOr(hv_CalcFScore)).TupleOr(hv_CalcRelativeConfusionMatrix);
  if (0 != (hv_CalcAbsoluteConfusionMatrix.TupleOr(hv_CalcRelativeConfusionMatrix)))
  {
    if (0 != (int(((hv_ImageLabelIDs.TupleSort()).TupleUniq())!=(hv_ClassIDs.TupleSort()))))
    {
      throw HException("Not all classes are represented in the ground truth labels. \nPlease check your data split.");
    }
  }
  //For top-K errors, the evaluated classes need to be represented.
  if (0 != (int(hv_TopKErrorKs!=HTuple())))
  {
    {
    HTuple end_val77 = (hv_ClassIDsToEvaluate.TupleLength())-1;
    HTuple step_val77 = 1;
    for (hv_EvalIndex=0; hv_EvalIndex.Continue(end_val77, step_val77); hv_EvalIndex += step_val77)
    {
      hv_CurrentEvalClass = HTuple(hv_ClassIDsToEvaluate[hv_EvalIndex]);
      if (0 != (int(hv_CurrentEvalClass!=HTuple("global"))))
      {
        hv_IndexClass = hv_ImageLabelIDs.TupleFind(hv_CurrentEvalClass);
        if (0 != (HTuple(int(hv_IndexClass==-1)).TupleOr(int(hv_IndexClass==HTuple()))))
        {
          throw HException(("The evaluated class ID "+hv_CurrentEvalClass)+" is not represented in the ground truth labels.");
        }
      }
    }
    }
  }
  //
  //Set image IDs, image label IDs, and top1-predictions to of evaluated samples EvaluationResult.
  CreateDict(&hv_EvaluatedSamples);
  SetDictTuple(hv_EvaluatedSamples, "image_ids", hv_ImageIDs);
  SetDictTuple(hv_EvaluatedSamples, "image_label_ids", hv_ImageLabelIDs);
  SetDictTuple(hv_EvaluatedSamples, "top1_predictions", hv_Predictions);
  SetDictTuple((*hv_EvaluationResult), "evaluated_samples", hv_EvaluatedSamples);
  //
  //Calculate absolute confusion matrix if needed and set it to EvaluationResult.
  if (0 != hv_CalcAbsoluteConfusionMatrix)
  {
    gen_confusion_matrix(hv_ImageLabelIDs, hv_Predictions, "display_matrix", "none", 
        HTuple(), &hv_ConfusionMatrix);
    SetDictTuple((*hv_EvaluationResult), "absolute_confusion_matrix", hv_ConfusionMatrix);
  }
  //
  //Calculate relative confusion matrix.
  if (0 != hv_CalcRelativeConfusionMatrix)
  {
    gen_confusion_matrix(hv_ImageLabelIDs, hv_Predictions, (HTuple("display_matrix").Append("return_matrix")), 
        (HTuple("none").Append("relative")), HTuple(), &hv_RelativeConfusionMatrix);
    SetDictTuple((*hv_EvaluationResult), "relative_confusion_matrix", hv_RelativeConfusionMatrix);
  }
  //
  // Calcuate measures for every class to be evaluated.
  {
  HTuple end_val108 = (hv_ClassIDsToEvaluate.TupleLength())-1;
  HTuple step_val108 = 1;
  for (hv_EvalIndex=0; hv_EvalIndex.Continue(end_val108, step_val108); hv_EvalIndex += step_val108)
  {
    hv_CurrentEvalClass = HTuple(hv_ClassIDsToEvaluate[hv_EvalIndex]);
    CreateDict(&hv_EvalClassID);
    //
    //Calculate top-K errors.
    {
    HTuple end_val113 = (hv_TopKErrorKs.TupleLength())-1;
    HTuple step_val113 = 1;
    for (hv_KIndex=0; hv_KIndex.Continue(end_val113, step_val113); hv_KIndex += step_val113)
    {
      hv_K = HTuple(hv_TopKErrorKs[hv_KIndex]);
      if (0 != (int(hv_CurrentEvalClass==HTuple("global"))))
      {
        hv_Indices = HTuple::TupleGenSequence(0,(hv_ImageLabelIDs.TupleLength())-1,1);
      }
      else
      {
        hv_Indices = hv_ImageLabelIDs.TupleFind(hv_CurrentEvalClass);
      }
      compute_top_k_error(HTuple(hv_ImageLabelIDs[hv_Indices]), HTuple(hv_TopKPredictions[hv_Indices]), 
          hv_K, &hv_TopKError);
      SetDictTuple(hv_EvalClassID, ("top"+hv_K)+"_error", hv_TopKError);
    }
    }
    //
    if (0 != (int(hv_CurrentEvalClass==HTuple("global"))))
    {
      //Compute the mean of the measures for all classes.
      hv_NumClasses = hv_ClassIDs.TupleLength();
      hv_IndexClass = hv_ClassIDs;
    }
    else
    {
      //Compute the measures for a certain class.
      hv_NumClasses = 1;
      hv_IndexClass = hv_ClassIDs.TupleFind(hv_CurrentEvalClass);
    }
    //
    //Calculate prediction.
    if (0 != (hv_CalcPrecision.TupleOr(hv_CalcFScore)))
    {
      hv_ClassPrecisions = HTuple();
      SumMatrix(hv_ConfusionMatrix, "rows", &hv_MatrixRowSumID);
      {
      HTuple end_val138 = hv_NumClasses-1;
      HTuple step_val138 = 1;
      for (hv_Index=0; hv_Index.Continue(end_val138, step_val138); hv_Index += step_val138)
      {
        //Compute the precision for every selected class.
        GetValueMatrix(hv_ConfusionMatrix, HTuple(hv_IndexClass[hv_Index]), HTuple(hv_IndexClass[hv_Index]), 
            &hv_TruePositive);
        GetValueMatrix(hv_MatrixRowSumID, HTuple(hv_IndexClass[hv_Index]), 0, &hv_SumPredictedClass);
        if (0 != (int(hv_SumPredictedClass==0)))
        {
          hv_ClassPrecision = 0;
        }
        else
        {
          hv_ClassPrecision = hv_TruePositive/hv_SumPredictedClass;
        }
        hv_ClassPrecisions = hv_ClassPrecisions.TupleConcat(hv_ClassPrecision);
      }
      }
      hv_Precision = hv_ClassPrecisions.TupleMean();
      ClearMatrix(hv_MatrixRowSumID);
      if (0 != (int(hv_NumClasses==1)))
      {
        SetDictTuple(hv_EvalClassID, "precision", hv_Precision);
      }
      else
      {
        SetDictTuple(hv_EvalClassID, "mean_precision", hv_Precision);
        SetDictTuple(hv_EvalClassID, "precision_per_class", hv_ClassPrecisions);
      }
    }
    //
    //Calculate recall.
    if (0 != (hv_CalcRecall.TupleOr(hv_CalcFScore)))
    {
      hv_ClassRecalls = HTuple();
      SumMatrix(hv_ConfusionMatrix, "columns", &hv_MatrixColumnSumID);
      {
      HTuple end_val163 = hv_NumClasses-1;
      HTuple step_val163 = 1;
      for (hv_Index=0; hv_Index.Continue(end_val163, step_val163); hv_Index += step_val163)
      {
        //Compute the recall for every class.
        GetValueMatrix(hv_ConfusionMatrix, HTuple(hv_IndexClass[hv_Index]), HTuple(hv_IndexClass[hv_Index]), 
            &hv_TruePositive);
        GetValueMatrix(hv_MatrixColumnSumID, 0, HTuple(hv_IndexClass[hv_Index]), 
            &hv_SumLabel);
        hv_ClassRecall = hv_TruePositive/hv_SumLabel;
        hv_ClassRecalls = hv_ClassRecalls.TupleConcat(hv_ClassRecall);
      }
      }
      hv_Recall = hv_ClassRecalls.TupleMean();
      ClearMatrix(hv_MatrixColumnSumID);
      if (0 != (int(hv_NumClasses==1)))
      {
        SetDictTuple(hv_EvalClassID, "recall", hv_Recall);
      }
      else
      {
        SetDictTuple(hv_EvalClassID, "mean_recall", hv_Recall);
        SetDictTuple(hv_EvalClassID, "recall_per_class", hv_ClassRecalls);
      }
    }
    //
    //Calculate F-score.
    if (0 != hv_CalcFScore)
    {
      TupleGenConst(hv_ClassPrecisions.TupleLength(), 0.0, &hv_ClassFScores);
      hv_SumPrecisionRecall = hv_ClassPrecisions+hv_ClassRecalls;
      hv_PositiveIndices = (hv_SumPrecisionRecall.TupleNotEqualElem(0.0)).TupleFind(1);
      if (0 != (HTuple(int(hv_PositiveIndices!=-1)).TupleAnd(int(hv_PositiveIndices!=HTuple()))))
      {
        hv_ClassFScores[hv_PositiveIndices] = ((2*HTuple(hv_ClassPrecisions[hv_PositiveIndices]))*HTuple(hv_ClassRecalls[hv_PositiveIndices]))/HTuple(hv_SumPrecisionRecall[hv_PositiveIndices]);
      }
      hv_FScore = hv_ClassFScores.TupleMean();
      if (0 != (int(hv_NumClasses==1)))
      {
        SetDictTuple(hv_EvalClassID, "f_score", hv_FScore);
      }
      else
      {
        SetDictTuple(hv_EvalClassID, "mean_f_score", hv_FScore);
        SetDictTuple(hv_EvalClassID, "f_score_per_class", hv_ClassFScores);
      }
    }
    //
    //Set evaluation results for current class ID.
    hv_KeyName = HTuple(hv_ClassIDsToEvaluate[hv_EvalIndex]);
    if (0 != (int(HTuple(hv_ClassIDsToEvaluate[hv_EvalIndex])!=HTuple("global"))))
    {
      hv_KeyName = "class_id_"+hv_KeyName;
    }
    SetDictTuple((*hv_EvaluationResult), hv_KeyName, hv_EvalClassID);
  }
  }
  //
  return;
}

// Chapter: Deep Learning / Evaluation
// Short Description: Calculate instance measures based on RunningMeasures. 
void calculate_instance_measures (HTuple hv_RunningMeasures, HTuple hv_EvalParams, 
    HTuple *hv_EvaluationResult)
{

  // Local iconic variables

  // Local control variables
  HTuple  hv_CalcClassAP, hv_CalcMeanAP, hv_CalcSoAP;
  HTuple  hv_InstanceType, hv_Measures, hv_M, hv_ClassIDs;
  HTuple  hv_NumClasses, hv_MaxNumDetections, hv_AreaRanges;
  HTuple  hv_IoUThresholds, hv_DetailedEvaluation, hv_InterpolatePRCurves;
  HTuple  hv_KeyExists, hv_AreaNames, hv_MinAreas, hv_MaxAreas;
  HTuple  hv_RecThreshs, hv_MDIdx, hv_MaxNum, hv_MaxNumStr;
  HTuple  hv_CurrentRunningMeasures, hv_PerMaxNumEvaluationResult;
  HTuple  hv_AreaIdx, hv_MinArea, hv_MaxArea, hv_AreaName;
  HTuple  hv_AreaRunningMeasures, hv_PerClassNumGt, hv_PerClassNumPred;
  HTuple  hv_PerClassConfidence, hv_PerClassNumGtIgnore, hv_CurrentEvaluationResult;
  HTuple  hv_ITIdx, hv_PerIoUAP, hv_PerIoUSoAP, hv_PerIoUDetailedEvaluation;
  HTuple  hv_ClsIdx, hv_PerClassDetailedEvaluation, hv_DetectionConfusionMatrix;
  HTuple  hv_PerIoUMeasure, hv_NumImgIDsWithFN, hv_NumImgIDsWithFP;
  HTuple  hv_ImgIDsWithFN, hv_ImgIDsWithFP, hv_ClassMAPDict;
  HTuple  hv_ClassMSoAPDict, hv_ClassesWithGt, hv_PerClassMAP;
  HTuple  hv_PerIoUMAP, hv_PerClassMSoAP, hv_PerIoUMSoAP;
  HTuple  hv_PerIoUNumClassesWithTP, hv_PerIoUTP, hv_PerIoUFN;
  HTuple  hv_PerIoUFP, hv_PerIoUFPClass, hv_PerIoUFPBackground;
  HTuple  hv_PerIoUFPLocalization, hv_PerIoUFPDuplicate, hv_PerIoUFPMultiple;
  HTuple  hv_PerIoUSoAPClass, hv_PerIoUSoAPLocalization, hv_PerIoUSoAPDuplicate;
  HTuple  hv_PerIoUSoAPMultiple, hv_PerIoUNumClassesWithFPClass;
  HTuple  hv_PerIoUNumClassesWithFPLocalization, hv_PerIoUNumClassesWithFPDuplicate;
  HTuple  hv_PerIoUNumClassesWithFPMultiple, hv_ClassAPPerIoU;
  HTuple  hv_ClassSoAPPerIoU, hv_NumGt, hv_NumGtIgnore, hv_NumPred;
  HTuple  hv_Confidences, hv_SortIdxs, hv_CurrentClassMeasures;
  HTuple  hv_IsTP, hv_Ignore, hv_NoIgnoreIdxs, hv_IsFP, hv_AccumulatedIsTP;
  HTuple  hv_AccumulatedIsFP, hv_Recall, hv_Precision, hv_InterpolatedPrecision;
  HTuple  hv_PIdx, hv_PrecisionAtRecThreshs, hv_RTIdx, hv_RecQuantile;
  HTuple  hv_AOD, hv_IdxsTP, hv_IsFPClass, hv_IsFPBackground;
  HTuple  hv_IsFPLocalization, hv_IsFPDuplicate, hv_IsFPMultiple;
  HTuple  hv_NumTP, hv_NumFP, hv_NumFN, hv_NumFPClass, hv_NumFPBackground;
  HTuple  hv_NumFPLocalization, hv_NumFPDuplicate, hv_NumFPMultiple;
  HTuple  hv_IndicesWithClassConfusion, hv_IsFPClassIdxs;
  HTuple  hv_ClassIdxsConfused, hv_Idx, hv_NumConfusedThisIdx;
  HTuple  hv_AODClass, hv_IdxsClass, hv_ResSoAPClass, hv_AODLocalization;
  HTuple  hv_IdxsLocalization, hv_ResSoAPLocalization, hv_AODDuplicate;
  HTuple  hv_IdxsDuplicate, hv_ResSoAPDuplicate, hv_AODMultiple;
  HTuple  hv_IdxsMultiple, hv_ResSoAPMultiple, hv_SoAPIoUIdxsPositive;
  HTuple  hv_MeanClassAP, hv_MeanClassSoAP, hv_MAP, hv_MSoAP;
  HTuple  hv_IoUsWithTP, hv_MSoAPAll, hv_NumSoAPAll, hv_IoUsWithFPClass;
  HTuple  hv_IoUsWithFPLocalization, hv_IoUsWithFPDuplicate;
  HTuple  hv_IoUsWithFPMultiple;

  //
  //This procedure calculates the final summarizing instance measures based on the running measures.
  //
  //Set default values.
  hv_CalcClassAP = 0;
  hv_CalcMeanAP = 0;
  hv_CalcSoAP = 0;
  //Check which measures are to be calculated.
  GetDictTuple(hv_EvalParams, "instance_type", &hv_InstanceType);
  GetDictTuple(hv_EvalParams, "measures", &hv_Measures);
  {
  HTuple end_val10 = (hv_Measures.TupleLength())-1;
  HTuple step_val10 = 1;
  for (hv_M=0; hv_M.Continue(end_val10, step_val10); hv_M += step_val10)
  {
    if (0 != (int(HTuple(hv_Measures[hv_M])==HTuple("mean_ap"))))
    {
      hv_CalcMeanAP = 1;
      //As we need to calculate the class APs anyway, we also write them out.
      hv_CalcClassAP = 1;
    }
    else if (0 != (HTuple(int(HTuple(hv_Measures[hv_M])==HTuple("soap"))).TupleAnd(int(hv_InstanceType==HTuple("rectangle2")))))
    {
      hv_CalcSoAP = 1;
    }
    else if (0 != (int(HTuple(hv_Measures[hv_M])==HTuple("all"))))
    {
      hv_CalcClassAP = 1;
      hv_CalcMeanAP = 1;
      if (0 != (int(hv_InstanceType==HTuple("rectangle2"))))
      {
        hv_CalcSoAP = 1;
      }
    }
    else
    {
      throw HException("Unknown Instance Measure: "+HTuple(hv_Measures[hv_M]));
    }
  }
  }
  //*
  //Dependencies of measures:
  //
  //Recall (per-class)       *                   --> AP per class --> mAP
  //Precision (per-class)  /
  //
  //*
  //Initialize output dictionary and get necessary evaluation parameters.
  CreateDict(&(*hv_EvaluationResult));
  GetDictTuple(hv_EvalParams, "class_ids", &hv_ClassIDs);
  GetDictTuple(hv_EvalParams, "num_classes", &hv_NumClasses);
  GetDictTuple(hv_EvalParams, "max_num_detections", &hv_MaxNumDetections);
  GetDictTuple(hv_EvalParams, "area_ranges", &hv_AreaRanges);
  GetDictTuple(hv_EvalParams, "iou_threshold", &hv_IoUThresholds);
  //Check if a detailed evaluation should be done and if PR-curves should be interpolated.
  hv_DetailedEvaluation = 0;
  hv_InterpolatePRCurves = 0;
  GetDictParam(hv_EvalParams, "key_exists", (HTuple("detailed_evaluation").Append("interpolate_pr_curves")), 
      &hv_KeyExists);
  if (0 != (HTuple(hv_KeyExists[0])))
  {
    GetDictTuple(hv_EvalParams, "detailed_evaluation", &hv_DetailedEvaluation);
  }
  if (0 != (HTuple(hv_KeyExists[1])))
  {
    GetDictTuple(hv_EvalParams, "interpolate_pr_curves", &hv_InterpolatePRCurves);
  }
  //
  //Get information about area ranges.
  GetDictTuple(hv_AreaRanges, "name", &hv_AreaNames);
  GetDictTuple(hv_AreaRanges, "min", &hv_MinAreas);
  GetDictTuple(hv_AreaRanges, "max", &hv_MaxAreas);
  //
  //Equidistant thresholds used to approximate the area under the Precision-Recall curve.
  hv_RecThreshs = HTuple::TupleGenSequence(0.0,1.0,0.01);
  //Start with calculation.
  if (0 != (HTuple(hv_CalcClassAP.TupleOr(hv_CalcMeanAP)).TupleOr(hv_CalcSoAP)))
  {
    //Loop over maximal number of detections.
    {
    HTuple end_val62 = (hv_MaxNumDetections.TupleLength())-1;
    HTuple step_val62 = 1;
    for (hv_MDIdx=0; hv_MDIdx.Continue(end_val62, step_val62); hv_MDIdx += step_val62)
    {
      //
      //Get corresponding running measures.
      hv_MaxNum = HTuple(hv_MaxNumDetections[hv_MDIdx]);
      hv_MaxNumStr = ""+hv_MaxNum;
      if (0 != (int(hv_MaxNum==-1)))
      {
        hv_MaxNumStr = "all";
      }
      GetDictTuple(hv_RunningMeasures, "max_num_detections_"+hv_MaxNumStr, &hv_CurrentRunningMeasures);
      //
      //Initialize output dictionary.
      CreateDict(&hv_PerMaxNumEvaluationResult);
      //
      //Loop over area ranges.
      {
      HTuple end_val76 = (hv_AreaNames.TupleLength())-1;
      HTuple step_val76 = 1;
      for (hv_AreaIdx=0; hv_AreaIdx.Continue(end_val76, step_val76); hv_AreaIdx += step_val76)
      {
        //Get area thresholds.
        hv_MinArea = HTuple(hv_MinAreas[hv_AreaIdx]);
        hv_MaxArea = HTuple(hv_MaxAreas[hv_AreaIdx]);
        hv_AreaName = HTuple(hv_AreaNames[hv_AreaIdx]);
        //
        GetDictTuple(hv_CurrentRunningMeasures, "area_"+hv_AreaName, &hv_AreaRunningMeasures);
        //
        GetDictTuple(hv_AreaRunningMeasures, "num_gt", &hv_PerClassNumGt);
        GetDictTuple(hv_AreaRunningMeasures, "num_pred", &hv_PerClassNumPred);
        GetDictTuple(hv_AreaRunningMeasures, "confidence", &hv_PerClassConfidence);
        GetDictTuple(hv_AreaRunningMeasures, "num_gt_ignore", &hv_PerClassNumGtIgnore);
        //
        //Initialize output dictionary.
        CreateDict(&hv_CurrentEvaluationResult);
        {
        HTuple end_val91 = (hv_IoUThresholds.TupleLength())-1;
        HTuple step_val91 = 1;
        for (hv_ITIdx=0; hv_ITIdx.Continue(end_val91, step_val91); hv_ITIdx += step_val91)
        {
          CreateDict(&hv_PerIoUAP);
          SetDictTuple(hv_CurrentEvaluationResult, "ap_iou_"+((""+HTuple(hv_IoUThresholds[hv_ITIdx])).TupleRegexpReplace("\\.","")), 
              hv_PerIoUAP);
          if (0 != hv_CalcSoAP)
          {
            CreateDict(&hv_PerIoUSoAP);
            SetDictTuple(hv_CurrentEvaluationResult, "soap_tp_iou_"+((""+HTuple(hv_IoUThresholds[hv_ITIdx])).TupleRegexpReplace("\\.","")), 
                hv_PerIoUSoAP);
          }
          if (0 != hv_DetailedEvaluation)
          {
            //Initialize detailed measures.
            CreateDict(&hv_PerIoUDetailedEvaluation);
            {
            HTuple end_val101 = hv_NumClasses-1;
            HTuple step_val101 = 1;
            for (hv_ClsIdx=0; hv_ClsIdx.Continue(end_val101, step_val101); hv_ClsIdx += step_val101)
            {
              CreateDict(&hv_PerClassDetailedEvaluation);
              //Initialize with zeros in case there is no ground truth for this class.
              SetDictTuple(hv_PerClassDetailedEvaluation, "num_tp", 0);
              SetDictTuple(hv_PerClassDetailedEvaluation, "num_fn", 0);
              SetDictTuple(hv_PerClassDetailedEvaluation, "num_fp", 0);
              SetDictTuple(hv_PerClassDetailedEvaluation, "num_fp_class", 0);
              SetDictTuple(hv_PerClassDetailedEvaluation, "num_fp_background", 0);
              SetDictTuple(hv_PerClassDetailedEvaluation, "num_fp_duplicate", 0);
              SetDictTuple(hv_PerClassDetailedEvaluation, "num_fp_localization", 
                  0);
              SetDictTuple(hv_PerClassDetailedEvaluation, "num_fp_multiple", 0);
              if (0 != hv_CalcSoAP)
              {
                SetDictTuple(hv_PerClassDetailedEvaluation, "soap_fp_class", -1);
                SetDictTuple(hv_PerClassDetailedEvaluation, "soap_fp_duplicate", 
                    -1);
                SetDictTuple(hv_PerClassDetailedEvaluation, "soap_fp_localization", 
                    -1);
                SetDictTuple(hv_PerClassDetailedEvaluation, "soap_fp_multiple", -1);
              }
              SetDictTuple(hv_PerIoUDetailedEvaluation, "class_"+HTuple(hv_ClassIDs[hv_ClsIdx]), 
                  hv_PerClassDetailedEvaluation);
            }
            }
            CreateMatrix(hv_NumClasses+1, hv_NumClasses+4, 0, &hv_DetectionConfusionMatrix);
            SetDictTuple(hv_PerIoUDetailedEvaluation, "detection_confusion_matrix", 
                hv_DetectionConfusionMatrix);
            //
            //Get and set image IDs with false negatives and false positives.
            GetDictTuple(hv_AreaRunningMeasures, "iou_"+((""+HTuple(hv_IoUThresholds[hv_ITIdx])).TupleRegexpReplace("\\.","")), 
                &hv_PerIoUMeasure);
            //Get image IDs with false negatives and false positives, respectively.
            GetDictTuple(hv_PerIoUMeasure, "num_image_ids_with_false_negatives", 
                &hv_NumImgIDsWithFN);
            GetDictTuple(hv_PerIoUMeasure, "num_image_ids_with_false_positives", 
                &hv_NumImgIDsWithFP);
            GetDictTuple(hv_PerIoUMeasure, "image_ids_with_false_negatives", &hv_ImgIDsWithFN);
            GetDictTuple(hv_PerIoUMeasure, "image_ids_with_false_positives", &hv_ImgIDsWithFP);
            //Set in current output.
            SetDictTuple(hv_PerIoUDetailedEvaluation, "image_ids_with_false_negatives", 
                hv_ImgIDsWithFN.TupleSelectRange(0,hv_NumImgIDsWithFN-1));
            SetDictTuple(hv_PerIoUDetailedEvaluation, "image_ids_with_false_positives", 
                hv_ImgIDsWithFP.TupleSelectRange(0,hv_NumImgIDsWithFP-1));
            //
            //Set output for this IoU.
            SetDictTuple(hv_CurrentEvaluationResult, "detailed_evaluation_iou_"+((""+HTuple(hv_IoUThresholds[hv_ITIdx])).TupleRegexpReplace("\\.","")), 
                hv_PerIoUDetailedEvaluation);
          }
        }
        }
        CreateDict(&hv_ClassMAPDict);
        SetDictTuple(hv_CurrentEvaluationResult, "mean_iou_ap", hv_ClassMAPDict);
        if (0 != hv_CalcSoAP)
        {
          CreateDict(&hv_ClassMSoAPDict);
          SetDictTuple(hv_CurrentEvaluationResult, "mean_iou_soap_tp", hv_ClassMSoAPDict);
        }
        //
        //Check which classes have ground truth annotations.
        hv_ClassesWithGt = ((hv_PerClassNumGt-hv_PerClassNumGtIgnore).TupleGreaterElem(0)).TupleFind(1);
        if (0 != (int(hv_ClassesWithGt==-1)))
        {
          hv_ClassesWithGt = HTuple();
        }
        //
        //Initialize PerClassMAP, i.e. mean average precision over IoU-thresholds per class.
        hv_PerClassMAP = HTuple(hv_NumClasses,-1.0);
        //
        //Initialize PerIoUMAP, i.e. mean average precision over classes per IoU-threshold.
        hv_PerIoUMAP = HTuple(hv_IoUThresholds.TupleLength(),0.0);
        //
        if (0 != hv_CalcSoAP)
        {
          //Initialize PerClassMSoAP, i.e. mean SoAP over IoU-thresholds per class.
          hv_PerClassMSoAP = HTuple(hv_NumClasses,-1.0);
          //Initialize PerIoUMSoAP, i.e. mean SoAP over classes per IoU-threshold.
          hv_PerIoUMSoAP = HTuple(hv_IoUThresholds.TupleLength(),0.0);
          //Initialize PerIoUNumClassesWithTP to store the class-indices where true positives occured.
          hv_PerIoUNumClassesWithTP = HTuple(hv_IoUThresholds.TupleLength(),0);
        }
        //
        if (0 != hv_DetailedEvaluation)
        {
          //Initialize overall num_fn, num_tp, ...
          hv_PerIoUTP = HTuple(hv_IoUThresholds.TupleLength(),0);
          hv_PerIoUFN = HTuple(hv_IoUThresholds.TupleLength(),0);
          hv_PerIoUFP = HTuple(hv_IoUThresholds.TupleLength(),0);
          hv_PerIoUFPClass = HTuple(hv_IoUThresholds.TupleLength(),0);
          hv_PerIoUFPBackground = HTuple(hv_IoUThresholds.TupleLength(),0);
          hv_PerIoUFPLocalization = HTuple(hv_IoUThresholds.TupleLength(),0);
          hv_PerIoUFPDuplicate = HTuple(hv_IoUThresholds.TupleLength(),0);
          hv_PerIoUFPMultiple = HTuple(hv_IoUThresholds.TupleLength(),0);
          if (0 != hv_CalcSoAP)
          {
            hv_PerIoUSoAPClass = HTuple(hv_IoUThresholds.TupleLength(),-1);
            hv_PerIoUSoAPLocalization = HTuple(hv_IoUThresholds.TupleLength(),-1);
            hv_PerIoUSoAPDuplicate = HTuple(hv_IoUThresholds.TupleLength(),-1);
            hv_PerIoUSoAPMultiple = HTuple(hv_IoUThresholds.TupleLength(),-1);
            hv_PerIoUNumClassesWithFPClass = HTuple(hv_IoUThresholds.TupleLength(),0);
            hv_PerIoUNumClassesWithFPLocalization = HTuple(hv_IoUThresholds.TupleLength(),0);
            hv_PerIoUNumClassesWithFPDuplicate = HTuple(hv_IoUThresholds.TupleLength(),0);
            hv_PerIoUNumClassesWithFPMultiple = HTuple(hv_IoUThresholds.TupleLength(),0);
          }
        }
        //Loop over all classes.
        {
        HTuple end_val188 = hv_NumClasses-1;
        HTuple step_val188 = 1;
        for (hv_ClsIdx=0; hv_ClsIdx.Continue(end_val188, step_val188); hv_ClsIdx += step_val188)
        {
          //
          //Initialize per-class AP per IoU-threshold (only for one class).
          hv_ClassAPPerIoU = HTuple(hv_IoUThresholds.TupleLength(),-1.0);
          //
          if (0 != hv_CalcSoAP)
          {
            hv_ClassSoAPPerIoU = HTuple(hv_IoUThresholds.TupleLength(),-1.0);
          }
          //Get results for this class.
          hv_NumGt = HTuple(hv_PerClassNumGt[hv_ClsIdx]);
          hv_NumGtIgnore = HTuple(hv_PerClassNumGtIgnore[hv_ClsIdx]);
          if (0 != (int((hv_NumGt-hv_NumGtIgnore)>0)))
          {
            hv_NumPred = HTuple(hv_PerClassNumPred[hv_ClsIdx]);
            GetDictTuple(hv_PerClassConfidence, "class_"+HTuple(hv_ClassIDs[hv_ClsIdx]), 
                &hv_Confidences);
            //
            //Sort the confidences in descending order and
            //only take the first NumPred ones due to block allocation.
            hv_SortIdxs = (-(hv_Confidences.TupleSelectRange(0,hv_NumPred-1))).TupleSortIndex();
            hv_Confidences = HTuple(hv_Confidences[hv_SortIdxs]);
          }
          //
          //Loop over IoU thresholds.
          {
          HTuple end_val210 = (hv_IoUThresholds.TupleLength())-1;
          HTuple step_val210 = 1;
          for (hv_ITIdx=0; hv_ITIdx.Continue(end_val210, step_val210); hv_ITIdx += step_val210)
          {
            //
            //Check if there are ground truth labels for this class.
            if (0 != (int((hv_NumGt-hv_NumGtIgnore)>0)))
            {
              //
              //Get results for this class and IoU-threshold.
              GetDictTuple(hv_AreaRunningMeasures, "iou_"+((""+HTuple(hv_IoUThresholds[hv_ITIdx])).TupleRegexpReplace("\\.","")), 
                  &hv_PerIoUMeasure);
              GetDictTuple(hv_PerIoUMeasure, "class_"+HTuple(hv_ClassIDs[hv_ClsIdx]), 
                  &hv_CurrentClassMeasures);
              GetDictTuple(hv_CurrentClassMeasures, "is_tp", &hv_IsTP);
              GetDictTuple(hv_CurrentClassMeasures, "ignore", &hv_Ignore);
              //
              //Sort the arrays IsTP and Ignore according to the confidence values.
              hv_IsTP = HTuple(hv_IsTP[hv_SortIdxs]);
              hv_Ignore = HTuple(hv_Ignore[hv_SortIdxs]);
              //
              //Sort out the ignored results.
              if (0 != (int((hv_IsTP.TupleLength())>0)))
              {
                hv_NoIgnoreIdxs = hv_Ignore.TupleFind(0);
                if (0 != (int(hv_NoIgnoreIdxs!=-1)))
                {
                  hv_IsTP = HTuple(hv_IsTP[hv_NoIgnoreIdxs]);
                  hv_IsFP = hv_IsTP.TupleNot();
                }
                else
                {
                  hv_IsTP = HTuple();
                  hv_IsFP = HTuple();
                }
              }
              else
              {
                hv_IsFP = HTuple();
              }
              //
              //Accumulate IsTP and IsFP.
              hv_AccumulatedIsTP = hv_IsTP.TupleCumul();
              hv_AccumulatedIsFP = hv_IsFP.TupleCumul();
              //
              //Compute recall.
              //The recall is computed with respect to all ground truth instances,
              //independent of MaxNum.
              hv_Recall = (hv_AccumulatedIsTP.TupleReal())/(hv_NumGt-hv_NumGtIgnore);
              //
              //Compute precision.
              hv_Precision = (hv_AccumulatedIsTP.TupleReal())/(hv_AccumulatedIsTP+hv_AccumulatedIsFP);
              //
              //(Optionally) smooth precision-recall curve.
              hv_InterpolatedPrecision = hv_Precision;
              if (0 != hv_InterpolatePRCurves)
              {
                for (hv_PIdx=hv_NumPred-2; hv_PIdx>=0; hv_PIdx+=-1)
                {
                  hv_InterpolatedPrecision[hv_PIdx] = HTuple(hv_InterpolatedPrecision[hv_PIdx]).TupleMax2(HTuple(hv_InterpolatedPrecision[hv_PIdx+1]));
                }
              }
              //Compute approximated area under the Precision-Recall curve using Recall-Thresholds.
              hv_PrecisionAtRecThreshs = HTuple(hv_RecThreshs.TupleLength(),0.);
              {
              HTuple end_val260 = (hv_RecThreshs.TupleLength())-1;
              HTuple step_val260 = 1;
              for (hv_RTIdx=0; hv_RTIdx.Continue(end_val260, step_val260); hv_RTIdx += step_val260)
              {
                hv_RecQuantile = (hv_Recall.TupleGreaterEqualElem(HTuple(hv_RecThreshs[hv_RTIdx]))).TupleFindFirst(1);
                if (0 != (int(hv_RecQuantile>-1)))
                {
                  hv_PrecisionAtRecThreshs[hv_RTIdx] = HTuple(hv_InterpolatedPrecision[hv_RecQuantile]);
                }
              }
              }
              //
              //Calculate AP as mean of precision at equidistant recall values.
              hv_ClassAPPerIoU[hv_ITIdx] = hv_PrecisionAtRecThreshs.TupleMean();
              //
              //Accumulate AP over classes.
              hv_PerIoUMAP[hv_ITIdx] = HTuple(hv_PerIoUMAP[hv_ITIdx])+HTuple(hv_ClassAPPerIoU[hv_ITIdx]);
              //
              if (0 != hv_CalcSoAP)
              {
                //Calculate SoAP out of the mean over absolute orientation differences.
                GetDictTuple(hv_CurrentClassMeasures, "abs_orientation_diff", &hv_AOD);
                hv_IdxsTP = hv_IsTP.TupleFind(1);
                if (0 != (HTuple(int((hv_IdxsTP.TupleLength())>0)).TupleAnd(int(hv_IdxsTP!=-1))))
                {
                  hv_ClassSoAPPerIoU[hv_ITIdx] = 1.0-((HTuple(hv_AOD[HTuple(hv_SortIdxs[HTuple(hv_NoIgnoreIdxs[hv_IdxsTP])])]).TupleMean())/(HTuple(180).TupleRad()));
                  //Accumulate SoAP over classes.
                  hv_PerIoUMSoAP[hv_ITIdx] = HTuple(hv_PerIoUMSoAP[hv_ITIdx])+HTuple(hv_ClassSoAPPerIoU[hv_ITIdx]);
                }
                //Update PerIoUNumClassesWithTP.
                if (0 != (int((hv_AccumulatedIsTP.TupleLength())>0)))
                {
                  hv_PerIoUNumClassesWithTP[hv_ITIdx] = HTuple(hv_PerIoUNumClassesWithTP[hv_ITIdx])+(HTuple(hv_AccumulatedIsTP[(hv_AccumulatedIsTP.TupleLength())-1]).TupleGreaterElem(0));
                }
              }
              //
              if (0 != hv_DetailedEvaluation)
              {
                //Summarize detailed evaluation running measures, set matrix-values and update overall detailed measures.
                //
                //Get the necessary running measures.
                GetDictTuple(hv_CurrentClassMeasures, "is_fp_class", &hv_IsFPClass);
                GetDictTuple(hv_CurrentClassMeasures, "is_fp_background", &hv_IsFPBackground);
                GetDictTuple(hv_CurrentClassMeasures, "is_fp_localization", &hv_IsFPLocalization);
                GetDictTuple(hv_CurrentClassMeasures, "is_fp_duplicate", &hv_IsFPDuplicate);
                GetDictTuple(hv_CurrentClassMeasures, "is_fp_multiple", &hv_IsFPMultiple);
                //
                //We use the values with maximal recall,
                //in case a higher precision is desired, increase 'min_confidence'.
                if (0 != (int((hv_AccumulatedIsTP.TupleLength())>0)))
                {
                  hv_NumTP = ((const HTuple&)hv_AccumulatedIsTP)[(hv_AccumulatedIsTP.TupleLength())-1];
                }
                else
                {
                  hv_NumTP = 0;
                }
                if (0 != (int((hv_AccumulatedIsFP.TupleLength())>0)))
                {
                  hv_NumFP = ((const HTuple&)hv_AccumulatedIsFP)[(hv_AccumulatedIsFP.TupleLength())-1];
                }
                else
                {
                  hv_NumFP = 0;
                }
                hv_NumFN = (hv_NumGt-hv_NumGtIgnore)-hv_NumTP;
                hv_NumFPClass = (HTuple(hv_IsFPClass[hv_SortIdxs]).TupleGreaterElem(-1)).TupleSum();
                hv_NumFPBackground = (HTuple(hv_IsFPBackground[hv_SortIdxs]).TupleGreaterElem(0)).TupleSum();
                hv_NumFPLocalization = (HTuple(hv_IsFPLocalization[hv_SortIdxs]).TupleGreaterElem(0)).TupleSum();
                hv_NumFPDuplicate = (HTuple(hv_IsFPDuplicate[hv_SortIdxs]).TupleGreaterElem(0)).TupleSum();
                hv_NumFPMultiple = (HTuple(hv_IsFPMultiple[hv_SortIdxs]).TupleGreaterElem(0)).TupleSum();
                if (0 != (int((hv_SortIdxs.TupleLength())==0)))
                {
                  hv_NumFPClass = 0;
                  hv_NumFPBackground = 0;
                  hv_NumFPLocalization = 0;
                  hv_NumFPDuplicate = 0;
                  hv_NumFPMultiple = 0;
                }
                //Consistency checks.
                if (0 != (int(((((((((hv_NumTP.TupleConcat(hv_NumFN)).TupleConcat(hv_NumFP)).TupleConcat(hv_NumFPClass)).TupleConcat(hv_NumFPBackground)).TupleConcat(hv_NumFPLocalization)).TupleConcat(hv_NumFPDuplicate)).TupleConcat(hv_NumFPMultiple)).TupleMin())<0)))
                {
                  throw HException("Fatal error while calculating instance measures.");
                }
                if (0 != (int(hv_NumFP!=((((hv_NumFPClass+hv_NumFPBackground)+hv_NumFPLocalization)+hv_NumFPDuplicate)+hv_NumFPMultiple))))
                {
                  throw HException("Fatal error while calculating instance measures.");
                }
                //
                //Set per-class measures.
                GetDictTuple(hv_CurrentEvaluationResult, "detailed_evaluation_iou_"+((""+HTuple(hv_IoUThresholds[hv_ITIdx])).TupleRegexpReplace("\\.","")), 
                    &hv_PerIoUDetailedEvaluation);
                GetDictTuple(hv_PerIoUDetailedEvaluation, "class_"+HTuple(hv_ClassIDs[hv_ClsIdx]), 
                    &hv_PerClassDetailedEvaluation);
                SetDictTuple(hv_PerClassDetailedEvaluation, "num_tp", hv_NumTP);
                SetDictTuple(hv_PerClassDetailedEvaluation, "num_fn", hv_NumFN);
                SetDictTuple(hv_PerClassDetailedEvaluation, "num_fp", hv_NumFP);
                SetDictTuple(hv_PerClassDetailedEvaluation, "num_fp_class", hv_NumFPClass);
                SetDictTuple(hv_PerClassDetailedEvaluation, "num_fp_background", 
                    hv_NumFPBackground);
                SetDictTuple(hv_PerClassDetailedEvaluation, "num_fp_duplicate", hv_NumFPDuplicate);
                SetDictTuple(hv_PerClassDetailedEvaluation, "num_fp_localization", 
                    hv_NumFPLocalization);
                SetDictTuple(hv_PerClassDetailedEvaluation, "num_fp_multiple", hv_NumFPMultiple);
                //
                //Set detection confusion matrix values.
                GetDictTuple(hv_PerIoUDetailedEvaluation, "detection_confusion_matrix", 
                    &hv_DetectionConfusionMatrix);
                SetValueMatrix(hv_DetectionConfusionMatrix, hv_ClsIdx, hv_ClsIdx, 
                    hv_NumTP);
                SetValueMatrix(hv_DetectionConfusionMatrix, hv_NumClasses, hv_ClsIdx, 
                    hv_NumFN);
                SetValueMatrix(hv_DetectionConfusionMatrix, hv_ClsIdx, hv_NumClasses, 
                    hv_NumFPBackground);
                SetValueMatrix(hv_DetectionConfusionMatrix, hv_ClsIdx, hv_NumClasses+1, 
                    hv_NumFPLocalization);
                SetValueMatrix(hv_DetectionConfusionMatrix, hv_ClsIdx, hv_NumClasses+2, 
                    hv_NumFPDuplicate);
                SetValueMatrix(hv_DetectionConfusionMatrix, hv_ClsIdx, hv_NumClasses+3, 
                    hv_NumFPMultiple);
                //
                //Go over IsFPClass and set confusions in matrix.
                hv_IndicesWithClassConfusion = (HTuple(hv_IsFPClass[hv_SortIdxs]).TupleGreaterElem(-1)).TupleFind(1);
                hv_IsFPClassIdxs = HTuple();
                if (0 != (int(hv_IndicesWithClassConfusion>-1)))
                {
                  hv_IsFPClassIdxs = HTuple(hv_IsFPClass[HTuple(hv_SortIdxs[hv_IndicesWithClassConfusion])]);
                }
                hv_ClassIdxsConfused = (hv_IsFPClassIdxs.TupleSort()).TupleUniq();
                {
                HTuple end_val359 = (hv_ClassIdxsConfused.TupleLength())-1;
                HTuple step_val359 = 1;
                for (hv_Idx=0; hv_Idx.Continue(end_val359, step_val359); hv_Idx += step_val359)
                {
                  hv_NumConfusedThisIdx = ((hv_IsFPClassIdxs.TupleFind(HTuple(hv_ClassIdxsConfused[hv_Idx]))).TupleGreaterElem(-1)).TupleSum();
                  SetValueMatrix(hv_DetectionConfusionMatrix, hv_ClsIdx, HTuple(hv_ClassIdxsConfused[hv_Idx]), 
                      hv_NumConfusedThisIdx);
                }
                }
                //
                //Update overall measures.
                hv_PerIoUFN[hv_ITIdx] = HTuple(hv_PerIoUFN[hv_ITIdx])+hv_NumFN;
                hv_PerIoUTP[hv_ITIdx] = HTuple(hv_PerIoUTP[hv_ITIdx])+hv_NumTP;
                hv_PerIoUFP[hv_ITIdx] = HTuple(hv_PerIoUFP[hv_ITIdx])+hv_NumFP;
                hv_PerIoUFPClass[hv_ITIdx] = HTuple(hv_PerIoUFPClass[hv_ITIdx])+hv_NumFPClass;
                hv_PerIoUFPBackground[hv_ITIdx] = HTuple(hv_PerIoUFPBackground[hv_ITIdx])+hv_NumFPBackground;
                hv_PerIoUFPLocalization[hv_ITIdx] = HTuple(hv_PerIoUFPLocalization[hv_ITIdx])+hv_NumFPLocalization;
                hv_PerIoUFPDuplicate[hv_ITIdx] = HTuple(hv_PerIoUFPDuplicate[hv_ITIdx])+hv_NumFPDuplicate;
                hv_PerIoUFPMultiple[hv_ITIdx] = HTuple(hv_PerIoUFPMultiple[hv_ITIdx])+hv_NumFPMultiple;
                if (0 != hv_CalcSoAP)
                {
                  //Calculate and update absolute difference of orientation for class false positives.
                  GetDictTuple(hv_CurrentClassMeasures, "abs_orientation_diff_class", 
                      &hv_AODClass);
                  hv_IdxsClass = (hv_AODClass.TupleGreaterElem(-1)).TupleFind(1);
                  if (0 != (int(hv_IdxsClass!=-1)))
                  {
                    hv_ResSoAPClass = 1.0-((HTuple(hv_AODClass[hv_IdxsClass]).TupleMean())/(HTuple(180).TupleRad()));
                    SetDictTuple(hv_PerClassDetailedEvaluation, "soap_fp_class", 
                        hv_ResSoAPClass);
                    //Update mean over classes.
                    hv_PerIoUNumClassesWithFPClass[hv_ITIdx] = HTuple(hv_PerIoUNumClassesWithFPClass[hv_ITIdx])+1;
                    if (0 != (int(HTuple(hv_PerIoUSoAPClass[hv_ITIdx])==-1)))
                    {
                      hv_PerIoUSoAPClass[hv_ITIdx] = hv_ResSoAPClass;
                    }
                    else
                    {
                      hv_PerIoUSoAPClass[hv_ITIdx] = HTuple(hv_PerIoUSoAPClass[hv_ITIdx])+hv_ResSoAPClass;
                    }
                  }
                  //Calculate and update absolute difference of orientation for localization false positives.
                  GetDictTuple(hv_CurrentClassMeasures, "abs_orientation_diff_localization", 
                      &hv_AODLocalization);
                  hv_IdxsLocalization = (hv_AODLocalization.TupleGreaterElem(-1)).TupleFind(1);
                  if (0 != (int(hv_IdxsLocalization!=-1)))
                  {
                    hv_ResSoAPLocalization = 1.0-((HTuple(hv_AODLocalization[hv_IdxsLocalization]).TupleMean())/(HTuple(180).TupleRad()));
                    SetDictTuple(hv_PerClassDetailedEvaluation, "soap_fp_localization", 
                        hv_ResSoAPLocalization);
                    //Update mean over classes.
                    hv_PerIoUNumClassesWithFPLocalization[hv_ITIdx] = HTuple(hv_PerIoUNumClassesWithFPLocalization[hv_ITIdx])+1;
                    if (0 != (int(HTuple(hv_PerIoUSoAPLocalization[hv_ITIdx])==-1)))
                    {
                      hv_PerIoUSoAPLocalization[hv_ITIdx] = hv_ResSoAPLocalization;
                    }
                    else
                    {
                      hv_PerIoUSoAPLocalization[hv_ITIdx] = HTuple(hv_PerIoUSoAPLocalization[hv_ITIdx])+hv_ResSoAPLocalization;
                    }
                  }
                  //Calculate and update absolute difference of orientation for class false positives.
                  GetDictTuple(hv_CurrentClassMeasures, "abs_orientation_diff_duplicate", 
                      &hv_AODDuplicate);
                  hv_IdxsDuplicate = (hv_AODDuplicate.TupleGreaterElem(-1)).TupleFind(1);
                  if (0 != (int(hv_IdxsDuplicate!=-1)))
                  {
                    hv_ResSoAPDuplicate = 1.0-((HTuple(hv_AODDuplicate[hv_IdxsDuplicate]).TupleMean())/(HTuple(180).TupleRad()));
                    SetDictTuple(hv_PerClassDetailedEvaluation, "soap_fp_duplicate", 
                        hv_ResSoAPDuplicate);
                    //Update mean over classes.
                    hv_PerIoUNumClassesWithFPDuplicate[hv_ITIdx] = HTuple(hv_PerIoUNumClassesWithFPDuplicate[hv_ITIdx])+1;
                    if (0 != (int(HTuple(hv_PerIoUSoAPDuplicate[hv_ITIdx])==-1)))
                    {
                      hv_PerIoUSoAPDuplicate[hv_ITIdx] = hv_ResSoAPDuplicate;
                    }
                    else
                    {
                      hv_PerIoUSoAPDuplicate[hv_ITIdx] = HTuple(hv_PerIoUSoAPDuplicate[hv_ITIdx])+hv_ResSoAPDuplicate;
                    }
                  }
                  //Calculate and update absolute difference of orientation for multiple false positives.
                  GetDictTuple(hv_CurrentClassMeasures, "abs_orientation_diff_multiple", 
                      &hv_AODMultiple);
                  hv_IdxsMultiple = (hv_AODMultiple.TupleGreaterElem(-1)).TupleFind(1);
                  if (0 != (int(hv_IdxsMultiple!=-1)))
                  {
                    hv_ResSoAPMultiple = 1.0-((HTuple(hv_AODMultiple[hv_IdxsMultiple]).TupleMean())/(HTuple(180).TupleRad()));
                    SetDictTuple(hv_PerClassDetailedEvaluation, "soap_fp_multiple", 
                        hv_ResSoAPMultiple);
                    //Update mean over classes.
                    hv_PerIoUNumClassesWithFPMultiple[hv_ITIdx] = HTuple(hv_PerIoUNumClassesWithFPMultiple[hv_ITIdx])+1;
                    if (0 != (int(HTuple(hv_PerIoUSoAPMultiple[hv_ITIdx])==-1)))
                    {
                      hv_PerIoUSoAPMultiple[hv_ITIdx] = hv_ResSoAPMultiple;
                    }
                    else
                    {
                      hv_PerIoUSoAPMultiple[hv_ITIdx] = HTuple(hv_PerIoUSoAPMultiple[hv_ITIdx])+hv_ResSoAPMultiple;
                    }
                  }
                }
              }
            }
            //
            //Write to output.
            GetDictTuple(hv_CurrentEvaluationResult, "ap_iou_"+((""+HTuple(hv_IoUThresholds[hv_ITIdx])).TupleRegexpReplace("\\.","")), 
                &hv_PerIoUAP);
            SetDictTuple(hv_PerIoUAP, "class_"+HTuple(hv_ClassIDs[hv_ClsIdx]), HTuple(hv_ClassAPPerIoU[hv_ITIdx]));
            if (0 != hv_CalcSoAP)
            {
              GetDictTuple(hv_CurrentEvaluationResult, "soap_tp_iou_"+((""+HTuple(hv_IoUThresholds[hv_ITIdx])).TupleRegexpReplace("\\.","")), 
                  &hv_PerIoUSoAP);
              SetDictTuple(hv_PerIoUSoAP, "class_"+HTuple(hv_ClassIDs[hv_ClsIdx]), 
                  HTuple(hv_ClassSoAPPerIoU[hv_ITIdx]));
            }
          }
          }
          //
          //Class mAP is the mean over IoU-thresholds.
          hv_PerClassMAP[hv_ClsIdx] = hv_ClassAPPerIoU.TupleMean();
          GetDictTuple(hv_CurrentEvaluationResult, "mean_iou_ap", &hv_ClassMAPDict);
          SetDictTuple(hv_ClassMAPDict, "class_"+HTuple(hv_ClassIDs[hv_ClsIdx]), 
              HTuple(hv_PerClassMAP[hv_ClsIdx]));
          if (0 != hv_CalcSoAP)
          {
            //ClassMSoAP is the mean over IoU-thresholds.
            hv_SoAPIoUIdxsPositive = (hv_ClassSoAPPerIoU.TupleGreaterEqualElem(0.0)).TupleFind(1);
            if (0 != (int(hv_SoAPIoUIdxsPositive!=-1)))
            {
              hv_PerClassMSoAP[hv_ClsIdx] = HTuple(hv_ClassSoAPPerIoU[hv_SoAPIoUIdxsPositive]).TupleMean();
            }
            else
            {
              hv_PerClassMSoAP[hv_ClsIdx] = -1.0;
            }
            GetDictTuple(hv_CurrentEvaluationResult, "mean_iou_soap_tp", &hv_ClassMSoAPDict);
            SetDictTuple(hv_ClassMSoAPDict, "class_"+HTuple(hv_ClassIDs[hv_ClsIdx]), 
                HTuple(hv_PerClassMSoAP[hv_ClsIdx]));
          }
        }
        }
        //
        //Calculate the mean AP and optionally mean SoAP (over classes) per IoU-threshold.
        {
        HTuple end_val461 = (hv_IoUThresholds.TupleLength())-1;
        HTuple step_val461 = 1;
        for (hv_ITIdx=0; hv_ITIdx.Continue(end_val461, step_val461); hv_ITIdx += step_val461)
        {
          GetDictTuple(hv_CurrentEvaluationResult, "ap_iou_"+((""+HTuple(hv_IoUThresholds[hv_ITIdx])).TupleRegexpReplace("\\.","")), 
              &hv_PerIoUAP);
          //
          //Consider only present classes.
          hv_MeanClassAP = -1.0;
          if (0 != (int((hv_ClassesWithGt.TupleLength())>0)))
          {
            hv_MeanClassAP = HTuple(hv_PerIoUMAP[hv_ITIdx])/(hv_ClassesWithGt.TupleLength());
          }
          SetDictTuple(hv_PerIoUAP, "mean_class_ap", hv_MeanClassAP);
          if (0 != hv_CalcSoAP)
          {
            GetDictTuple(hv_CurrentEvaluationResult, "soap_tp_iou_"+((""+HTuple(hv_IoUThresholds[hv_ITIdx])).TupleRegexpReplace("\\.","")), 
                &hv_PerIoUSoAP);
            //
            //Consider only present classes.
            hv_MeanClassSoAP = -1.0;
            if (0 != (int(HTuple(hv_PerIoUNumClassesWithTP[hv_ITIdx])>0)))
            {
              hv_MeanClassSoAP = HTuple(hv_PerIoUMSoAP[hv_ITIdx])/HTuple(hv_PerIoUNumClassesWithTP[hv_ITIdx]);
            }
            SetDictTuple(hv_PerIoUSoAP, "mean_class_soap_tp", hv_MeanClassSoAP);
          }
          //
          if (0 != hv_DetailedEvaluation)
          {
            //Add overall measures for TP, FN, FP, ...
            GetDictTuple(hv_CurrentEvaluationResult, "detailed_evaluation_iou_"+((""+HTuple(hv_IoUThresholds[hv_ITIdx])).TupleRegexpReplace("\\.","")), 
                &hv_PerIoUDetailedEvaluation);
            SetDictTuple(hv_PerIoUDetailedEvaluation, "num_tp", HTuple(hv_PerIoUTP[hv_ITIdx]));
            SetDictTuple(hv_PerIoUDetailedEvaluation, "num_fn", HTuple(hv_PerIoUFN[hv_ITIdx]));
            SetDictTuple(hv_PerIoUDetailedEvaluation, "num_fp", HTuple(hv_PerIoUFP[hv_ITIdx]));
            SetDictTuple(hv_PerIoUDetailedEvaluation, "num_fp_class", HTuple(hv_PerIoUFPClass[hv_ITIdx]));
            SetDictTuple(hv_PerIoUDetailedEvaluation, "num_fp_background", HTuple(hv_PerIoUFPBackground[hv_ITIdx]));
            SetDictTuple(hv_PerIoUDetailedEvaluation, "num_fp_duplicate", HTuple(hv_PerIoUFPDuplicate[hv_ITIdx]));
            SetDictTuple(hv_PerIoUDetailedEvaluation, "num_fp_localization", HTuple(hv_PerIoUFPLocalization[hv_ITIdx]));
            SetDictTuple(hv_PerIoUDetailedEvaluation, "num_fp_multiple", HTuple(hv_PerIoUFPMultiple[hv_ITIdx]));
            if (0 != hv_CalcSoAP)
            {
              if (0 != (int(HTuple(hv_PerIoUNumClassesWithFPClass[hv_ITIdx])>0)))
              {
                hv_PerIoUSoAPClass[hv_ITIdx] = HTuple(hv_PerIoUSoAPClass[hv_ITIdx])/HTuple(hv_PerIoUNumClassesWithFPClass[hv_ITIdx]);
              }
              if (0 != (int(HTuple(hv_PerIoUNumClassesWithFPLocalization[hv_ITIdx])>0)))
              {
                hv_PerIoUSoAPLocalization[hv_ITIdx] = HTuple(hv_PerIoUSoAPLocalization[hv_ITIdx])/HTuple(hv_PerIoUNumClassesWithFPLocalization[hv_ITIdx]);
              }
              if (0 != (int(HTuple(hv_PerIoUNumClassesWithFPDuplicate[hv_ITIdx])>0)))
              {
                hv_PerIoUSoAPDuplicate[hv_ITIdx] = HTuple(hv_PerIoUSoAPDuplicate[hv_ITIdx])/HTuple(hv_PerIoUNumClassesWithFPDuplicate[hv_ITIdx]);
              }
              if (0 != (int(HTuple(hv_PerIoUNumClassesWithFPMultiple[hv_ITIdx])>0)))
              {
                hv_PerIoUSoAPMultiple[hv_ITIdx] = HTuple(hv_PerIoUSoAPMultiple[hv_ITIdx])/HTuple(hv_PerIoUNumClassesWithFPMultiple[hv_ITIdx]);
              }
              SetDictTuple(hv_PerIoUDetailedEvaluation, "soap_fp_class", HTuple(hv_PerIoUSoAPClass[hv_ITIdx]));
              SetDictTuple(hv_PerIoUDetailedEvaluation, "soap_fp_localization", HTuple(hv_PerIoUSoAPLocalization[hv_ITIdx]));
              SetDictTuple(hv_PerIoUDetailedEvaluation, "soap_fp_duplicate", HTuple(hv_PerIoUSoAPDuplicate[hv_ITIdx]));
              SetDictTuple(hv_PerIoUDetailedEvaluation, "soap_fp_multiple", HTuple(hv_PerIoUSoAPMultiple[hv_ITIdx]));
            }
          }
        }
        }
        //
        //Calculate overall mean AP (over classes and IoU-thresholds).
        //Also here only classes with ground truth annotations are taken into account.
        hv_MAP = -1.0;
        if (0 != (int((hv_ClassesWithGt.TupleLength())>0)))
        {
          hv_MAP = (HTuple(hv_PerClassMAP[hv_ClassesWithGt]).TupleSum())/(hv_ClassesWithGt.TupleLength());
        }
        SetDictTuple(hv_CurrentEvaluationResult, "mean_ap", hv_MAP);
        if (0 != hv_CalcSoAP)
        {
          hv_MSoAP = -1.0;
          if (0 != (int((hv_PerIoUNumClassesWithTP.TupleSum())>0)))
          {
            hv_IoUsWithTP = (hv_PerIoUNumClassesWithTP.TupleGreaterElem(0)).TupleFind(1);
            hv_MSoAP = ((HTuple(hv_PerIoUMSoAP[hv_IoUsWithTP])/HTuple(hv_PerIoUNumClassesWithTP[hv_IoUsWithTP])).TupleSum())/(hv_IoUsWithTP.TupleLength());
          }
          SetDictTuple(hv_CurrentEvaluationResult, "mean_soap_tp", hv_MSoAP);
          if (0 != hv_DetailedEvaluation)
          {
            hv_MSoAPAll = 0.0;
            hv_NumSoAPAll = 0;
            if (0 != (int((hv_PerIoUNumClassesWithTP.TupleSum())>0)))
            {
              hv_IoUsWithTP = (hv_PerIoUNumClassesWithTP.TupleGreaterElem(0)).TupleFind(1);
              hv_MSoAPAll = hv_MSoAP*(HTuple(hv_PerIoUNumClassesWithTP[hv_IoUsWithTP]).TupleSum());
              hv_NumSoAPAll += HTuple(hv_PerIoUNumClassesWithTP[hv_IoUsWithTP]).TupleSum();
            }
            if (0 != (int((hv_PerIoUNumClassesWithFPClass.TupleSum())>0)))
            {
              hv_IoUsWithFPClass = (hv_PerIoUNumClassesWithFPClass.TupleGreaterElem(0)).TupleFind(1);
              hv_MSoAPAll += ((HTuple(hv_PerIoUSoAPClass[hv_IoUsWithFPClass]).TupleSum())/(hv_IoUsWithFPClass.TupleLength()))*(HTuple(hv_PerIoUNumClassesWithFPClass[hv_IoUsWithFPClass]).TupleSum());
              hv_NumSoAPAll += HTuple(hv_PerIoUNumClassesWithFPClass[hv_IoUsWithFPClass]).TupleSum();
            }
            if (0 != (int((hv_PerIoUNumClassesWithFPLocalization.TupleSum())>0)))
            {
              hv_IoUsWithFPLocalization = (hv_PerIoUNumClassesWithFPLocalization.TupleGreaterElem(0)).TupleFind(1);
              hv_MSoAPAll += ((HTuple(hv_PerIoUSoAPLocalization[hv_IoUsWithFPLocalization]).TupleSum())/(hv_IoUsWithFPLocalization.TupleLength()))*(HTuple(hv_PerIoUNumClassesWithFPLocalization[hv_IoUsWithFPLocalization]).TupleSum());
              hv_NumSoAPAll += HTuple(hv_PerIoUNumClassesWithFPLocalization[hv_IoUsWithFPLocalization]).TupleSum();
            }
            if (0 != (int((hv_PerIoUNumClassesWithFPDuplicate.TupleSum())>0)))
            {
              hv_IoUsWithFPDuplicate = (hv_PerIoUNumClassesWithFPDuplicate.TupleGreaterElem(0)).TupleFind(1);
              hv_MSoAPAll += ((HTuple(hv_PerIoUSoAPDuplicate[hv_IoUsWithFPDuplicate]).TupleSum())/(hv_IoUsWithFPDuplicate.TupleLength()))*(HTuple(hv_PerIoUNumClassesWithFPDuplicate[hv_IoUsWithFPDuplicate]).TupleSum());
              hv_NumSoAPAll += HTuple(hv_PerIoUNumClassesWithFPDuplicate[hv_IoUsWithFPDuplicate]).TupleSum();
            }
            if (0 != (int((hv_PerIoUNumClassesWithFPMultiple.TupleSum())>0)))
            {
              hv_IoUsWithFPMultiple = (hv_PerIoUNumClassesWithFPMultiple.TupleGreaterElem(0)).TupleFind(1);
              hv_MSoAPAll += ((HTuple(hv_PerIoUSoAPMultiple[hv_IoUsWithFPMultiple]).TupleSum())/(hv_IoUsWithFPMultiple.TupleLength()))*(HTuple(hv_PerIoUNumClassesWithFPMultiple[hv_IoUsWithFPMultiple]).TupleSum());
              hv_NumSoAPAll += HTuple(hv_PerIoUNumClassesWithFPMultiple[hv_IoUsWithFPMultiple]).TupleSum();
            }
            if (0 != (int(hv_NumSoAPAll>0)))
            {
              SetDictTuple(hv_CurrentEvaluationResult, "mean_soap_all", (hv_MSoAPAll.TupleReal())/hv_NumSoAPAll);
            }
            else
            {
              SetDictTuple(hv_CurrentEvaluationResult, "mean_soap_all", -1);
            }
            //
          }
        }
        //
        //Add CurrentEvaluationResult to output.
        SetDictTuple(hv_PerMaxNumEvaluationResult, "area_"+hv_AreaName, hv_CurrentEvaluationResult);
      }
      }
      //Add PerMaxNumEvaluationResult to output.
      SetDictTuple((*hv_EvaluationResult), "max_num_detections_"+hv_MaxNumStr, hv_PerMaxNumEvaluationResult);
    }
    }
  }
  //
  return;
}

// Chapter: Deep Learning / Evaluation
// Short Description: Calculate pixel measures based on RunningMeasures. 
void calculate_pixel_measures (HTuple hv_RunningMeasures, HTuple hv_EvalParams, HTuple *hv_EvaluationResult)
{

  // Local iconic variables

  // Local control variables
  HTuple  hv_CalcClassPixelAccuracy, hv_CalcPixelAccuracy;
  HTuple  hv_CalcPixelConfusionMatrix, hv_CalcMeanAccuracy;
  HTuple  hv_CalcMeanIou, hv_CalcClassIou, hv_CalcFWIou, hv_Measures;
  HTuple  hv_M, hv_ConfMatrix, hv_TPMat, hv_TP, hv_SumRowMat;
  HTuple  hv_RowSum, hv_FP, hv_SumColMat, hv_ColSum, hv_FN;
  HTuple  hv_IgnoreClassIDs, hv_Rows, hv_Columns, hv_FPIgnore;
  HTuple  hv_GT, hv_ClsIdxValid, hv_ClassPixelAccuracy, hv_MeanAccuracy;
  HTuple  hv_PixelAccuracy, hv_ClassIoU, hv_MeanIoU, hv_FWIoU;
  HTuple  hv_FwWeights;

  //
  //This procedure calculates the pixel-wise measures based on the values in running measures.
  //
  //Set default values.
  hv_CalcClassPixelAccuracy = 0;
  hv_CalcPixelAccuracy = 0;
  hv_CalcPixelConfusionMatrix = 0;
  hv_CalcMeanAccuracy = 0;
  hv_CalcMeanIou = 0;
  hv_CalcClassIou = 0;
  hv_CalcFWIou = 0;
  //Check which measures are to be calculated.
  GetDictTuple(hv_EvalParams, "measures", &hv_Measures);
  CreateDict(&(*hv_EvaluationResult));
  {
  HTuple end_val14 = (hv_Measures.TupleLength())-1;
  HTuple step_val14 = 1;
  for (hv_M=0; hv_M.Continue(end_val14, step_val14); hv_M += step_val14)
  {
    if (0 != (int(HTuple(hv_Measures[hv_M])==HTuple("pixel_accuracy"))))
    {
      hv_CalcPixelAccuracy = 1;
    }
    else if (0 != (int(HTuple(hv_Measures[hv_M])==HTuple("class_pixel_accuracy"))))
    {
      hv_CalcClassPixelAccuracy = 1;
    }
    else if (0 != (int(HTuple(hv_Measures[hv_M])==HTuple("pixel_confusion_matrix"))))
    {
      hv_CalcPixelConfusionMatrix = 1;
    }
    else if (0 != (int(HTuple(hv_Measures[hv_M])==HTuple("mean_accuracy"))))
    {
      hv_CalcMeanAccuracy = 1;
    }
    else if (0 != (int(HTuple(hv_Measures[hv_M])==HTuple("mean_iou"))))
    {
      hv_CalcMeanIou = 1;
    }
    else if (0 != (int(HTuple(hv_Measures[hv_M])==HTuple("class_iou"))))
    {
      hv_CalcClassIou = 1;
    }
    else if (0 != (int(HTuple(hv_Measures[hv_M])==HTuple("frequency_weighted_iou"))))
    {
      hv_CalcFWIou = 1;
    }
    else if (0 != (int(HTuple(hv_Measures[hv_M])==HTuple("all"))))
    {
      hv_CalcPixelAccuracy = 1;
      hv_CalcClassPixelAccuracy = 1;
      hv_CalcPixelConfusionMatrix = 1;
      hv_CalcMeanAccuracy = 1;
      hv_CalcMeanIou = 1;
      hv_CalcClassIou = 1;
      hv_CalcFWIou = 1;
    }
    else
    {
      throw HException("Unknown Segmentation Measure: "+HTuple(hv_Measures[hv_M]));
    }
  }
  }
  //
  //Depending on the running measure values (ConfusionMatrix or TP/FP/FN),
  //we first calculate TP/FP/FN from the ConfusionMatrix.
  if (0 != hv_CalcPixelConfusionMatrix)
  {
    //Get the running measures.
    GetDictTuple(hv_RunningMeasures, "pixel_confusion_matrix", &hv_ConfMatrix);
    //Get the per-class true positives as the diagonal of the matrix.
    GetDiagonalMatrix(hv_ConfMatrix, 0, &hv_TPMat);
    GetFullMatrix(hv_TPMat, &hv_TP);
    //For the confusion matrix, the row determines the predicted class-IDs,
    //the column determines the ground truth class-IDs.
    //Get the per-class false positives (FP) as the sum over the rows minus the diagonal (TP).
    SumMatrix(hv_ConfMatrix, "rows", &hv_SumRowMat);
    GetFullMatrix(hv_SumRowMat, &hv_RowSum);
    hv_FP = hv_RowSum-hv_TP;
    //Get the per-class false negatives (FN) as the sum over the columns minus the diagonal (TP).
    SumMatrix(hv_ConfMatrix, "columns", &hv_SumColMat);
    GetFullMatrix(hv_SumColMat, &hv_ColSum);
    hv_FN = hv_ColSum-hv_TP;
    //We do not want to count the false positives (FP) in the ignore region.
    //The false negatives (FN) are not affected, since the model does not predict the ignore class.
    GetDictTuple(hv_EvalParams, "ignore_class_ids", &hv_IgnoreClassIDs);
    if (0 != (int((hv_IgnoreClassIDs.TupleLength())>0)))
    {
      //The ignore class corresponds to the last row/column in the confusion matrix.
      GetSizeMatrix(hv_ConfMatrix, &hv_Rows, &hv_Columns);
      GetValueMatrix(hv_ConfMatrix, HTuple::TupleGenSequence(0,hv_Rows-1,1), HTuple(hv_Rows,hv_Columns-1), 
          &hv_FPIgnore);
      hv_FP = hv_FP-hv_FPIgnore;
      //Remove last entries of TP, FP, FN (those related to the ignore class).
      hv_TP = hv_TP.TupleSelectRange(0,(hv_TP.TupleLength())-2);
      hv_FP = hv_FP.TupleSelectRange(0,(hv_FP.TupleLength())-2);
      hv_FN = hv_FN.TupleSelectRange(0,(hv_FN.TupleLength())-2);
      //Remove last row/column from confusion matrix.
      GetSubMatrix(hv_ConfMatrix, 0, 0, hv_Rows-1, hv_Columns-1, &hv_ConfMatrix);
    }
    //Paste the confusion matrix to the output.
    SetDictTuple((*hv_EvaluationResult), "pixel_confusion_matrix", hv_ConfMatrix);
  }
  else
  {
    //Get the running measure values.
    GetDictTuple(hv_RunningMeasures, "tp", &hv_TP);
    GetDictTuple(hv_RunningMeasures, "fp", &hv_FP);
    GetDictTuple(hv_RunningMeasures, "fn", &hv_FN);
  }
  //
  //It might be the case, that some of the classes are not present in the set of validation images.
  //--> Exclude these classes (they are indirectly present as they reduce the number of TP for other classes).
  hv_GT = hv_TP+hv_FN;
  hv_ClsIdxValid = (hv_GT.TupleGreaterElem(0)).TupleFind(1);
  //
  //Mean Accuracy, Class Pixel Accuracy.
  //-> If one of 'mean_accuracy', 'class_pixel_accuracy' is specified, we give back both of them
  //   as they have to be calculated anyway (to the most part).
  if (0 != (hv_CalcClassPixelAccuracy.TupleOr(hv_CalcMeanAccuracy)))
  {
    //Compute pixel accuracy per class (although we might only use it for the overall pixel accuracy).
    hv_ClassPixelAccuracy = HTuple(hv_GT.TupleLength(),-1);
    hv_MeanAccuracy = -1;
    if (0 != (int(HTuple(hv_ClsIdxValid[0])>-1)))
    {
      hv_ClassPixelAccuracy[hv_ClsIdxValid] = (HTuple(hv_TP[hv_ClsIdxValid]).TupleReal())/HTuple(hv_GT[hv_ClsIdxValid]);
      hv_MeanAccuracy = HTuple(hv_ClassPixelAccuracy[hv_ClsIdxValid]).TupleMean();
    }
    SetDictTuple((*hv_EvaluationResult), "class_pixel_accuracy", hv_ClassPixelAccuracy);
    SetDictTuple((*hv_EvaluationResult), "mean_accuracy", hv_MeanAccuracy);
  }
  //Pixel Accuracy
  if (0 != hv_CalcPixelAccuracy)
  {
    //Compute pixel accuracy as the total ratio of pixels that have been correctly predicted.
    hv_PixelAccuracy = -1;
    if (0 != (int(HTuple(hv_ClsIdxValid[0])>-1)))
    {
      hv_PixelAccuracy = ((HTuple(hv_TP[hv_ClsIdxValid]).TupleSum()).TupleReal())/(HTuple(hv_GT[hv_ClsIdxValid]).TupleSum());
    }
    SetDictTuple((*hv_EvaluationResult), "pixel_accuracy", hv_PixelAccuracy);
  }
  //Mean IoU, class IoU, frequency weighted IoU:
  //-> If the measures 'class_iou', 'mean_iou' or 'frequency_weighted_iou' is specified,
  //   we return all three of them as they have to be calculated anyway (to the most part).
  if (0 != (HTuple(hv_CalcMeanIou.TupleOr(hv_CalcClassIou)).TupleOr(hv_CalcFWIou)))
  {
    hv_ClassIoU = HTuple(hv_GT.TupleLength(),-1);
    hv_MeanIoU = -1;
    hv_FWIoU = -1;
    if (0 != (int(HTuple(hv_ClsIdxValid[0])>-1)))
    {
      hv_ClassIoU[hv_ClsIdxValid] = (HTuple(hv_TP[hv_ClsIdxValid]).TupleReal())/(HTuple(hv_GT[hv_ClsIdxValid])+HTuple(hv_FP[hv_ClsIdxValid]));
      hv_MeanIoU = HTuple(hv_ClassIoU[hv_ClsIdxValid]).TupleMean();
      hv_FwWeights = (hv_GT.TupleReal())/(hv_GT.TupleSum());
      hv_FWIoU = (HTuple(hv_FwWeights[hv_ClsIdxValid])*HTuple(hv_ClassIoU[hv_ClsIdxValid])).TupleSum();
    }
    SetDictTuple((*hv_EvaluationResult), "class_iou", hv_ClassIoU);
    SetDictTuple((*hv_EvaluationResult), "mean_iou", hv_MeanIoU);
    SetDictTuple((*hv_EvaluationResult), "frequency_weighted_iou", hv_FWIoU);
  }
  //
  return;
}

// Chapter: Deep Learning / Model
// Short Description: Checks the content of the parameter dictionary DLPreprocessParam. 
void check_dl_preprocess_param (HTuple hv_DLPreprocessParam)
{

  // Local iconic variables

  // Local control variables
  HTuple  hv_CheckParams, hv_KeyExists, hv_DLModelType;
  HTuple  hv_Exception, hv_SupportedModelTypes, hv_Index;
  HTuple  hv_ParamNamesGeneral, hv_ParamNamesSegmentation;
  HTuple  hv_ParamNamesDetectionOptional, hv_ParamNamesPreprocessingOptional;
  HTuple  hv_ParamNamesAll, hv_ParamNames, hv_KeysExists;
  HTuple  hv_I, hv_Exists, hv_InputKeys, hv_Key, hv_Value;
  HTuple  hv_Indices, hv_ValidValues, hv_ValidTypes, hv_V;
  HTuple  hv_T, hv_IsInt, hv_ValidTypesListing, hv_ValidValueListing;
  HTuple  hv_EmptyStrings, hv_ImageRangeMinExists, hv_ImageRangeMaxExists;
  HTuple  hv_ImageRangeMin, hv_ImageRangeMax, hv_IndexParam;
  HTuple  hv_SetBackgroundID, hv_ClassIDsBackground, hv_Intersection;
  HTuple  hv_IgnoreClassIDs, hv_KnownClasses, hv_IgnoreClassID;
  HTuple  hv_OptionalKeysExist, hv_InstanceType, hv_IgnoreDirection;
  HTuple  hv_ClassIDsNoOrientation, hv_SemTypes;

  //
  //This procedure checks a dictionary with parameters for DL preprocessing.
  //
  hv_CheckParams = 1;
  //If check_params is set to false, do not check anything.
  GetDictParam(hv_DLPreprocessParam, "key_exists", "check_params", &hv_KeyExists);
  if (0 != hv_KeyExists)
  {
    GetDictTuple(hv_DLPreprocessParam, "check_params", &hv_CheckParams);
    if (0 != (hv_CheckParams.TupleNot()))
    {
      return;
    }
  }
  //
  try
  {
    GetDictTuple(hv_DLPreprocessParam, "model_type", &hv_DLModelType);
  }
  // catch (Exception) 
  catch (HException &HDevExpDefaultException)
  {
    HDevExpDefaultException.ToHTuple(&hv_Exception);
    throw HException(HTuple(HTuple("DLPreprocessParam needs the parameter: '")+"model_type")+"'");
  }
  //
  //Check for correct model type.
  hv_SupportedModelTypes.Clear();
  hv_SupportedModelTypes[0] = "anomaly_detection";
  hv_SupportedModelTypes[1] = "classification";
  hv_SupportedModelTypes[2] = "detection";
  hv_SupportedModelTypes[3] = "segmentation";
  TupleFind(hv_SupportedModelTypes, hv_DLModelType, &hv_Index);
  if (0 != (HTuple(int(hv_Index==-1)).TupleOr(int(hv_Index==HTuple()))))
  {
    throw HException(HTuple("Only models of type 'anomaly_detection', 'classification', 'detection', or 'segmentation' are supported"));
    return;
  }
  //
  //Parameter names that are required.
  //General parameters.
  hv_ParamNamesGeneral.Clear();
  hv_ParamNamesGeneral[0] = "model_type";
  hv_ParamNamesGeneral[1] = "image_width";
  hv_ParamNamesGeneral[2] = "image_height";
  hv_ParamNamesGeneral[3] = "image_num_channels";
  hv_ParamNamesGeneral[4] = "image_range_min";
  hv_ParamNamesGeneral[5] = "image_range_max";
  hv_ParamNamesGeneral[6] = "normalization_type";
  hv_ParamNamesGeneral[7] = "domain_handling";
  //Segmentation specific parameters.
  hv_ParamNamesSegmentation.Clear();
  hv_ParamNamesSegmentation[0] = "ignore_class_ids";
  hv_ParamNamesSegmentation[1] = "set_background_id";
  hv_ParamNamesSegmentation[2] = "class_ids_background";
  //Detection specific parameters.
  hv_ParamNamesDetectionOptional.Clear();
  hv_ParamNamesDetectionOptional[0] = "instance_type";
  hv_ParamNamesDetectionOptional[1] = "ignore_direction";
  hv_ParamNamesDetectionOptional[2] = "class_ids_no_orientation";
  //Normalization specific parameters.
  hv_ParamNamesPreprocessingOptional.Clear();
  hv_ParamNamesPreprocessingOptional[0] = "mean_values_normalization";
  hv_ParamNamesPreprocessingOptional[1] = "deviation_values_normalization";
  //All parameters
  hv_ParamNamesAll.Clear();
  hv_ParamNamesAll.Append(hv_ParamNamesGeneral);
  hv_ParamNamesAll.Append(hv_ParamNamesSegmentation);
  hv_ParamNamesAll.Append(hv_ParamNamesDetectionOptional);
  hv_ParamNamesAll.Append(hv_ParamNamesPreprocessingOptional);
  hv_ParamNames = hv_ParamNamesGeneral;
  if (0 != (int(hv_DLModelType==HTuple("segmentation"))))
  {
    //Extend ParamNames for models of type segmentation.
    hv_ParamNames = hv_ParamNames.TupleConcat(hv_ParamNamesSegmentation);
  }
  //
  //Check if legacy parameter exist.
  //Otherwise map it to the legal parameter.
  replace_legacy_preprocessing_parameters(hv_DLPreprocessParam);
  //
  //Check that all necessary parameters are included.
  //
  GetDictParam(hv_DLPreprocessParam, "key_exists", hv_ParamNames, &hv_KeysExists);
  if (0 != (int(((hv_KeysExists.TupleEqualElem(0)).TupleSum())>0)))
  {
    {
    HTuple end_val52 = hv_KeysExists.TupleLength();
    HTuple step_val52 = 1;
    for (hv_I=0; hv_I.Continue(end_val52, step_val52); hv_I += step_val52)
    {
      hv_Exists = HTuple(hv_KeysExists[hv_I]);
      if (0 != (hv_Exists.TupleNot()))
      {
        throw HException(("DLPreprocessParam needs the parameter: '"+HTuple(hv_ParamNames[hv_I]))+"'");
      }
    }
    }
  }
  //
  //Check the keys provided.
  GetDictParam(hv_DLPreprocessParam, "keys", HTuple(), &hv_InputKeys);
  {
  HTuple end_val62 = (hv_InputKeys.TupleLength())-1;
  HTuple step_val62 = 1;
  for (hv_I=0; hv_I.Continue(end_val62, step_val62); hv_I += step_val62)
  {
    hv_Key = HTuple(hv_InputKeys[hv_I]);
    GetDictTuple(hv_DLPreprocessParam, hv_Key, &hv_Value);
    //Check that the key is known.
    TupleFind(hv_ParamNamesAll, hv_Key, &hv_Indices);
    if (0 != (int(hv_Indices==-1)))
    {
      throw HException(("Unknown key for DLPreprocessParam: '"+HTuple(hv_InputKeys[hv_I]))+"'");
      return;
    }
    //Set expected values and types.
    hv_ValidValues = HTuple();
    hv_ValidTypes = HTuple();
    if (0 != (int(hv_Key==HTuple("normalization_type"))))
    {
      hv_ValidValues.Clear();
      hv_ValidValues[0] = "all_channels";
      hv_ValidValues[1] = "first_channel";
      hv_ValidValues[2] = "constant_values";
      hv_ValidValues[3] = "none";
    }
    else if (0 != (int(hv_Key==HTuple("domain_handling"))))
    {
      if (0 != (int(hv_DLModelType==HTuple("anomaly_detection"))))
      {
        hv_ValidValues.Clear();
        hv_ValidValues[0] = "full_domain";
        hv_ValidValues[1] = "crop_domain";
        hv_ValidValues[2] = "keep_domain";
      }
      else
      {
        hv_ValidValues.Clear();
        hv_ValidValues[0] = "full_domain";
        hv_ValidValues[1] = "crop_domain";
      }
    }
    else if (0 != (int(hv_Key==HTuple("model_type"))))
    {
      hv_ValidValues.Clear();
      hv_ValidValues[0] = "anomaly_detection";
      hv_ValidValues[1] = "classification";
      hv_ValidValues[2] = "detection";
      hv_ValidValues[3] = "segmentation";
    }
    else if (0 != (int(hv_Key==HTuple("set_background_id"))))
    {
      hv_ValidTypes = "int";
    }
    else if (0 != (int(hv_Key==HTuple("class_ids_background"))))
    {
      hv_ValidTypes = "int";
    }
    //Check that type is valid.
    if (0 != (int((hv_ValidTypes.TupleLength())>0)))
    {
      {
      HTuple end_val91 = (hv_ValidTypes.TupleLength())-1;
      HTuple step_val91 = 1;
      for (hv_V=0; hv_V.Continue(end_val91, step_val91); hv_V += step_val91)
      {
        hv_T = HTuple(hv_ValidTypes[hv_V]);
        if (0 != (int(hv_T==HTuple("int"))))
        {
          TupleIsInt(hv_Value, &hv_IsInt);
          if (0 != (hv_IsInt.TupleNot()))
          {
            hv_ValidTypes = ("'"+hv_ValidTypes)+"'";
            if (0 != (int((hv_ValidTypes.TupleLength())<2)))
            {
              hv_ValidTypesListing = hv_ValidTypes;
            }
            else
            {
              hv_ValidTypesListing = (((hv_ValidTypes.TupleSelectRange(0,HTuple(0).TupleMax2((hv_ValidTypes.TupleLength())-2)))+HTuple(", "))+HTuple(hv_ValidTypes[(hv_ValidTypes.TupleLength())-1])).TupleSum();
            }
            throw HException(((((("The value given in the key '"+hv_Key)+"' of DLPreprocessParam is invalid. Valid types are: ")+hv_ValidTypesListing)+". The given value was '")+hv_Value)+"'.");
            return;
          }
        }
        else
        {
          throw HException("Internal error. Unknown valid type.");
        }
      }
      }
    }
    //Check that value is valid.
    if (0 != (int((hv_ValidValues.TupleLength())>0)))
    {
      TupleFindFirst(hv_ValidValues, hv_Value, &hv_Index);
      if (0 != (int(hv_Index==-1)))
      {
        hv_ValidValues = ("'"+hv_ValidValues)+"'";
        if (0 != (int((hv_ValidValues.TupleLength())<2)))
        {
          hv_ValidValueListing = hv_ValidValues;
        }
        else
        {
          hv_EmptyStrings = HTuple((hv_ValidValues.TupleLength())-2,"");
          hv_ValidValueListing = (((hv_ValidValues.TupleSelectRange(0,HTuple(0).TupleMax2((hv_ValidValues.TupleLength())-2)))+HTuple(", "))+(hv_EmptyStrings.TupleConcat(HTuple(hv_ValidValues[(hv_ValidValues.TupleLength())-1])))).TupleSum();
        }
        throw HException(((((("The value given in the key '"+hv_Key)+"' of DLPreprocessParam is invalid. Valid values are: ")+hv_ValidValueListing)+". The given value was '")+hv_Value)+"'.");
      }
    }
  }
  }
  //
  //Check the correct setting of ImageRangeMin and ImageRangeMax.
  if (0 != (HTuple(int(hv_DLModelType==HTuple("classification"))).TupleOr(int(hv_DLModelType==HTuple("detection")))))
  {
    //Check ImageRangeMin and ImageRangeMax.
    GetDictParam(hv_DLPreprocessParam, "key_exists", "image_range_min", &hv_ImageRangeMinExists);
    GetDictParam(hv_DLPreprocessParam, "key_exists", "image_range_max", &hv_ImageRangeMaxExists);
    //If they are present, check that they are set correctly.
    if (0 != hv_ImageRangeMinExists)
    {
      GetDictTuple(hv_DLPreprocessParam, "image_range_min", &hv_ImageRangeMin);
      if (0 != (int(hv_ImageRangeMin!=-127)))
      {
        throw HException(("For model type "+hv_DLModelType)+" ImageRangeMin has to be -127.");
      }
    }
    if (0 != hv_ImageRangeMaxExists)
    {
      GetDictTuple(hv_DLPreprocessParam, "image_range_max", &hv_ImageRangeMax);
      if (0 != (int(hv_ImageRangeMax!=128)))
      {
        throw HException(("For model type "+hv_DLModelType)+" ImageRangeMax has to be 128.");
      }
    }
  }
  //
  //Check segmentation specific parameters.
  if (0 != (int(hv_DLModelType==HTuple("segmentation"))))
  {
    //Check if detection specific parameters are set.
    GetDictParam(hv_DLPreprocessParam, "key_exists", hv_ParamNamesDetectionOptional, 
        &hv_KeysExists);
    //If they are present, check that they are [].
    {
    HTuple end_val151 = (hv_ParamNamesDetectionOptional.TupleLength())-1;
    HTuple step_val151 = 1;
    for (hv_IndexParam=0; hv_IndexParam.Continue(end_val151, step_val151); hv_IndexParam += step_val151)
    {
      if (0 != (HTuple(hv_KeysExists[hv_IndexParam])))
      {
        GetDictTuple(hv_DLPreprocessParam, HTuple(hv_ParamNamesDetectionOptional[hv_IndexParam]), 
            &hv_Value);
        if (0 != (int(hv_Value!=HTuple())))
        {
          throw HException(((("The preprocessing parameter '"+HTuple(hv_ParamNamesDetectionOptional[hv_IndexParam]))+"' was set to ")+hv_Value)+HTuple(" but for segmentation it should be set to [], as it is not used for this method."));
        }
      }
    }
    }
    //Check 'set_background_id'.
    GetDictTuple(hv_DLPreprocessParam, "set_background_id", &hv_SetBackgroundID);
    if (0 != (int((hv_SetBackgroundID.TupleLength())>1)))
    {
      throw HException("Only one class_id as 'set_background_id' allowed.");
    }
    //Check 'class_ids_background'.
    GetDictTuple(hv_DLPreprocessParam, "class_ids_background", &hv_ClassIDsBackground);
    if (0 != (HTuple(HTuple(int((hv_SetBackgroundID.TupleLength())>0)).TupleAnd(HTuple(int((hv_ClassIDsBackground.TupleLength())>0)).TupleNot())).TupleOr(HTuple(int((hv_ClassIDsBackground.TupleLength())>0)).TupleAnd(HTuple(int((hv_SetBackgroundID.TupleLength())>0)).TupleNot()))))
    {
      throw HException("Both keys 'set_background_id' and 'class_ids_background' are required.");
    }
    //Check that 'class_ids_background' and 'set_background_id' are disjoint.
    if (0 != (int((hv_SetBackgroundID.TupleLength())>0)))
    {
      TupleIntersection(hv_SetBackgroundID, hv_ClassIDsBackground, &hv_Intersection);
      if (0 != (hv_Intersection.TupleLength()))
      {
        throw HException("Class IDs in 'set_background_id' and 'class_ids_background' need to be disjoint.");
      }
    }
    //Check 'ignore_class_ids'.
    GetDictTuple(hv_DLPreprocessParam, "ignore_class_ids", &hv_IgnoreClassIDs);
    hv_KnownClasses.Clear();
    hv_KnownClasses.Append(hv_SetBackgroundID);
    hv_KnownClasses.Append(hv_ClassIDsBackground);
    {
    HTuple end_val179 = (hv_IgnoreClassIDs.TupleLength())-1;
    HTuple step_val179 = 1;
    for (hv_I=0; hv_I.Continue(end_val179, step_val179); hv_I += step_val179)
    {
      hv_IgnoreClassID = HTuple(hv_IgnoreClassIDs[hv_I]);
      TupleFindFirst(hv_KnownClasses, hv_IgnoreClassID, &hv_Index);
      if (0 != (HTuple(int((hv_Index.TupleLength())>0)).TupleAnd(int(hv_Index!=-1))))
      {
        throw HException("The given 'ignore_class_ids' must not be included in the 'class_ids_background' or 'set_background_id'.");
      }
    }
    }
  }
  else if (0 != (int(hv_DLModelType==HTuple("detection"))))
  {
    //Check if segmentation specific parameters are set.
    GetDictParam(hv_DLPreprocessParam, "key_exists", hv_ParamNamesSegmentation, &hv_KeysExists);
    //If they are present, check that they are [].
    {
    HTuple end_val190 = (hv_ParamNamesSegmentation.TupleLength())-1;
    HTuple step_val190 = 1;
    for (hv_IndexParam=0; hv_IndexParam.Continue(end_val190, step_val190); hv_IndexParam += step_val190)
    {
      if (0 != (HTuple(hv_KeysExists[hv_IndexParam])))
      {
        GetDictTuple(hv_DLPreprocessParam, HTuple(hv_ParamNamesSegmentation[hv_IndexParam]), 
            &hv_Value);
        if (0 != (int(hv_Value!=HTuple())))
        {
          throw HException(((("The preprocessing parameter '"+HTuple(hv_ParamNamesSegmentation[hv_IndexParam]))+"' was set to ")+hv_Value)+HTuple(" but for detection it should be set to [], as it is not used for this method."));
        }
      }
    }
    }
    //Check optional parameters.
    GetDictParam(hv_DLPreprocessParam, "key_exists", hv_ParamNamesDetectionOptional, 
        &hv_OptionalKeysExist);
    if (0 != (HTuple(hv_OptionalKeysExist[0])))
    {
      //Check 'instance_type'.
      GetDictTuple(hv_DLPreprocessParam, HTuple(hv_ParamNamesDetectionOptional[0]), 
          &hv_InstanceType);
      if (0 != (int(((HTuple("rectangle1").Append("rectangle2")).TupleFind(hv_InstanceType))==-1)))
      {
        throw HException(("Invalid generic parameter for 'instance_type': "+hv_InstanceType)+HTuple(", only 'rectangle1' and 'rectangle2' are allowed"));
      }
    }
    GetDictParam(hv_DLPreprocessParam, "key_exists", hv_ParamNamesDetectionOptional, 
        &hv_OptionalKeysExist);
    if (0 != (HTuple(hv_OptionalKeysExist[1])))
    {
      //Check 'ignore_direction'.
      GetDictTuple(hv_DLPreprocessParam, HTuple(hv_ParamNamesDetectionOptional[1]), 
          &hv_IgnoreDirection);
      if (0 != (int(((HTuple(1).Append(0)).TupleFind(hv_IgnoreDirection))==-1)))
      {
        throw HException(("Invalid generic parameter for 'ignore_direction': "+hv_IgnoreDirection)+HTuple(", only true and false are allowed"));
      }
    }
    if (0 != (HTuple(hv_OptionalKeysExist[2])))
    {
      //Check 'class_ids_no_orientation'.
      GetDictTuple(hv_DLPreprocessParam, HTuple(hv_ParamNamesDetectionOptional[2]), 
          &hv_ClassIDsNoOrientation);
      TupleSemTypeElem(hv_ClassIDsNoOrientation, &hv_SemTypes);
      if (0 != (HTuple(int(hv_ClassIDsNoOrientation!=HTuple())).TupleAnd(int(((hv_SemTypes.TupleEqualElem("integer")).TupleSum())!=(hv_ClassIDsNoOrientation.TupleLength())))))
      {
        throw HException(("Invalid generic parameter for 'class_ids_no_orientation': "+hv_ClassIDsNoOrientation)+HTuple(", only integers are allowed"));
      }
      else
      {
        if (0 != (HTuple(int(hv_ClassIDsNoOrientation!=HTuple())).TupleAnd(int(((hv_ClassIDsNoOrientation.TupleGreaterEqualElem(0)).TupleSum())!=(hv_ClassIDsNoOrientation.TupleLength())))))
        {
          throw HException(("Invalid generic parameter for 'class_ids_no_orientation': "+hv_ClassIDsNoOrientation)+HTuple(", only non-negative integers are allowed"));
        }
      }
    }
  }
  //
  return;
}

// Chapter: Deep Learning / Model
void check_train_dl_model_params (HTuple hv_DLDataset, HTuple hv_DLModelHandle, HTuple hv_NumTrainSamples, 
    HTuple hv_StartEpoch, HTuple hv_TrainParam)
{

  // Local iconic variables

  // Local control variables
  HTuple  hv_ModelType, hv_PreprocessedDataset;
  HTuple  hv_PreprocessParam, hv_Exception, hv_TrainParamAnomaly;
  HTuple  hv_DomainRatio, hv_ErrorThreshold, hv_RegularizationNoise;
  HTuple  hv_NumEpochs, hv_BatchSizeDevice, hv_BatchSizeMultiplier;
  HTuple  hv_BatchSize, hv_ClassIDsModel, hv_ClassIDsDataset;
  HTuple  hv_Index, hv_IndexFind, hv_ClassIDsModelStr, hv_ClassIDsDatasetStr;
  HTuple  hv_DisplayParam, hv_DisplayKeys, hv_KeyName, hv_KeyValue;
  HTuple  hv_TrainParamCopy, hv__;

  //
  //This procedure checks the parameters used in the procedure train_dl_model for consistency.
  //
  GetDlModelParam(hv_DLModelHandle, "type", &hv_ModelType);
  //
  //Check the NumEpochs parameter.
  if (0 != (hv_StartEpoch.TupleIsNumber()))
  {
    if (0 != (int(hv_StartEpoch<0.0)))
    {
      throw HException("Error: StartEpoch < 0 is not allowed.");
    }
  }
  //
  //Check if the dataset is already preprocessed.
  hv_PreprocessedDataset = 0;
  try
  {
    GetDictTuple(hv_DLDataset, "preprocess_param", &hv_PreprocessParam);
    hv_PreprocessedDataset = 1;
  }
  // catch (Exception) 
  catch (HException &HDevExpDefaultException)
  {
    HDevExpDefaultException.ToHTuple(&hv_Exception);
  }
  if (0 != (hv_PreprocessedDataset.TupleNot()))
  {
    throw HException("Error: The supplied dataset needs to be preprocessed already. Use the standard procedure preprocess_dl_dataset.");
  }
  //
  //Check parameters for anomaly detection models.
  if (0 != (int(hv_ModelType==HTuple("anomaly_detection"))))
  {
    GetDictTuple(hv_TrainParam, "anomaly_param", &hv_TrainParamAnomaly);
    GetDictTuple(hv_TrainParamAnomaly, "domain_ratio", &hv_DomainRatio);
    if (0 != (HTuple(int(hv_DomainRatio<=0)).TupleOr(int(hv_DomainRatio>1.0))))
    {
      throw HException("Error: The anomaly detection parameter 'domain_ratio' must be between 0 and 1.");
    }
    GetDictTuple(hv_TrainParamAnomaly, "error_threshold", &hv_ErrorThreshold);
    if (0 != (HTuple(int(hv_ErrorThreshold<0)).TupleOr(int(hv_ErrorThreshold>1))))
    {
      throw HException("Error: The anomaly detection parameter 'error_threshold' must be between 0 and 1.");
    }
    GetDictTuple(hv_TrainParamAnomaly, "regularization_noise", &hv_RegularizationNoise);
    if (0 != (int(hv_RegularizationNoise<0)))
    {
      throw HException("Error: The anomaly detection parameter 'regularization_noise' must be greater than or equal to 0.");
    }
    return;
  }
  //
  //Check parameters for other models.
  //
  //Check the NumEpochs parameter.
  if (0 != (hv_StartEpoch.TupleIsNumber()))
  {
    GetDictTuple(hv_TrainParam, "num_epochs", &hv_NumEpochs);
    if (0 != (int(hv_StartEpoch>hv_NumEpochs)))
    {
      throw HException("Error: StartEpoch > NumEpochs is not allowed.");
    }
  }
  else
  {
    if (0 != (int(hv_StartEpoch!=HTuple("resume"))))
    {
      throw HException("Error: StartEpoch has to be a number or equal to 'resume'.");
    }
  }
  //
  //Check that the number of training samples is at least as big as the total batch size.
  GetDlModelParam(hv_DLModelHandle, "batch_size", &hv_BatchSizeDevice);
  GetDlModelParam(hv_DLModelHandle, "batch_size_multiplier", &hv_BatchSizeMultiplier);
  hv_BatchSize = hv_BatchSizeDevice*hv_BatchSizeMultiplier;
  if (0 != (int(hv_NumTrainSamples<hv_BatchSize)))
  {
    throw HException("Error: Number of training samples is smaller than the batch size.");
  }
  //
  //Check that all model class IDs are a part of the DLDataset class IDs.
  GetDlModelParam(hv_DLModelHandle, "class_ids", &hv_ClassIDsModel);
  GetDictTuple(hv_DLDataset, "class_ids", &hv_ClassIDsDataset);
  {
  HTuple end_val66 = (hv_ClassIDsModel.TupleLength())-1;
  HTuple step_val66 = 1;
  for (hv_Index=0; hv_Index.Continue(end_val66, step_val66); hv_Index += step_val66)
  {
    TupleFindFirst(hv_ClassIDsDataset, HTuple(hv_ClassIDsModel[hv_Index]), &hv_IndexFind);
    if (0 != (int(hv_IndexFind<0)))
    {
      hv_ClassIDsModelStr = (" "+hv_ClassIDsModel).TupleSum();
      hv_ClassIDsDatasetStr = (" "+hv_ClassIDsDataset).TupleSum();
      throw HException((((("Error: A model class ID is not part of the DLDataset class IDs. DLModelHandle class ID: "+hv_ClassIDsModelStr)+". ")+"DLDataset class IDs: ")+hv_ClassIDsDatasetStr)+".");
    }
  }
  }
  //
  //Check display parameters.
  GetDictTuple(hv_TrainParam, "display_param", &hv_DisplayParam);
  GetDictParam(hv_DisplayParam, "keys", HTuple(), &hv_DisplayKeys);
  {
  HTuple end_val78 = (hv_DisplayKeys.TupleLength())-1;
  HTuple step_val78 = 1;
  for (hv_Index=0; hv_Index.Continue(end_val78, step_val78); hv_Index += step_val78)
  {
    hv_KeyName = HTuple(hv_DisplayKeys[hv_Index]);
    if (0 != (int(hv_KeyName==HTuple("enabled"))))
    {
      GetDictTuple(hv_DisplayParam, hv_KeyName, &hv_KeyValue);
      if (0 != (HTuple(int(hv_KeyValue!=1)).TupleAnd(int(hv_KeyValue!=0))))
      {
        throw HException("The value for 'enabled' is not supported.");
      }
    }
    else if (0 != (int(hv_KeyName==HTuple("change_plot_interval_seconds"))))
    {
      GetDictTuple(hv_DisplayParam, hv_KeyName, &hv_KeyValue);
      if (0 != (HTuple((hv_KeyValue.TupleIsInt()).TupleOr(hv_KeyValue.TupleIsReal())).TupleNot()))
      {
        throw HException("The value of 'change_plot_interval_seconds' has to be of type integer or real");
      }
    }
    else if (0 != (int(hv_KeyName==HTuple("num_images"))))
    {
      GetDictTuple(hv_DisplayParam, hv_KeyName, &hv_KeyValue);
      if (0 != (HTuple((hv_KeyValue.TupleIsInt()).TupleNot()).TupleOr(int(hv_KeyValue<1))))
      {
        throw HException("The value of 'num_images' has to be of type integer and larger or equal to one");
      }
    }
    else if (0 != (int(hv_KeyName==HTuple("selected_percentage_train_samples"))))
    {
      GetDictTuple(hv_DisplayParam, hv_KeyName, &hv_KeyValue);
      if (0 != (HTuple((hv_KeyValue.TupleIsInt()).TupleOr(hv_KeyValue.TupleIsReal())).TupleNot()))
      {
        throw HException("The value of 'selected_percentage_train_samples' has to be of type integer or real");
      }
      if (0 != (HTuple(int(hv_KeyValue<0)).TupleOr(int(hv_KeyValue>100))))
      {
        throw HException(HTuple("The value of 'selected_percentage_train_samples' has to be in [0,100]"));
      }
    }
    else if (0 != (int(hv_KeyName==HTuple("update_images_interval_epochs"))))
    {
      GetDictTuple(hv_DisplayParam, hv_KeyName, &hv_KeyValue);
      if (0 != (HTuple((hv_KeyValue.TupleIsInt()).TupleOr(hv_KeyValue.TupleIsReal())).TupleNot()))
      {
        throw HException("The value of 'update_images_interval_epochs' has to be of type integer or real");
      }
      if (0 != (int(hv_KeyValue<=0)))
      {
        throw HException("The value of 'update_images_interval_epochs' has to be larger than zero");
      }
    }
    else if (0 != (int(hv_KeyName==HTuple("x_axis_label"))))
    {
      GetDictTuple(hv_DisplayParam, hv_KeyName, &hv_KeyValue);
      if (0 != (HTuple(int(hv_KeyValue!=HTuple("epochs"))).TupleAnd(int(hv_KeyValue!=HTuple("iterations")))))
      {
        throw HException("The value for 'x_axis_label' is not supported.");
      }
    }
    else if (0 != (HTuple(HTuple(int(hv_KeyName==HTuple("status_model_params"))).TupleOr(int(hv_KeyName==HTuple("tiled_param")))).TupleOr(int(hv_KeyName==HTuple("randomize_images")))))
    {
      //No check for these advanced settings.
      //No check for randomize_images for backward compatibility.
      continue;
    }
    else
    {
      throw HException(("The provided key "+hv_KeyName)+" for 'display' is invalid.");
    }
  }
  }
  //
  //Initialize change and serialization strategies in order to test for valid values.
  CopyDict(hv_TrainParam, HTuple(), HTuple(), &hv_TrainParamCopy);
  init_train_dl_model_change_strategies(hv_TrainParamCopy, &hv__);
  init_train_dl_model_serialization_strategies(hv_TrainParamCopy, &hv__);
  //
  return;
}

// Chapter: Deep Learning / Model
// Short Description: Collect the information required for displaying the training progress update. 
void collect_train_dl_model_info (HTuple hv_DLModelHandle, HTuple hv_TrainResults, 
    HTuple hv_EvaluationInfos, HTuple hv_EvaluationComparisonKeys, HTuple hv_Iteration, 
    HTuple hv_NumIterations, HTuple hv_NumIterationsPerEpoch, HTuple hv_NumSamplesMeanLoss, 
    HTuple *hv_TrainInfo)
{

  // Local iconic variables

  // Local control variables
  HTuple  hv_EpochReal, hv_NumEpochs, hv_TrainResultsStored;
  HTuple  hv_Exception, hv_ModelType, hv_PossibleParamNames;
  HTuple  hv_ModelParams, hv_Index, hv_ParamName, hv_DeviceHandles;
  HTuple  hv_DeviceLength, hv_DeviceTypes, hv_DeviceNames;
  HTuple  hv_DeviceIndex, hv_DeviceType, hv_DeviceName, hv_GenParamValue;
  HTuple  hv_LossSamplesTrainResults, hv_Indices, hv_TrainResultsUsed;
  HTuple  hv_BatchSizeDevice, hv_BatchSizeMultiplier, hv_BatchSize;
  HTuple  hv_NumIterationsMean, hv_LossParam, hv_LossValues;
  HTuple  hv_TrainResult, hv_LossValue, hv_LossMean, hv_BestEvaluationInfo;
  HTuple  hv_BestEvaluationInfoTrain, hv_BestEvaluationValue;
  HTuple  hv_BestEvaluationValueTrain, hv_BestEvaluationKeys;
  HTuple  hv_BestEvaluationKeysTrain, hv_EvaluationInfo, hv_ValidationEvaluationResult;
  HTuple  hv_TrainEvaluationResult, hv_Value, hv_ValidEvaluationKeys;
  HTuple  hv_ValueTrain, hv_ValidEvaluationKeysTrain, hv_BestEvaluationData;

  //
  //This procedure computes training information for the given iteration.
  //
  CreateDict(&(*hv_TrainInfo));
  //
  //General iteration and epoch status.
  hv_EpochReal = (hv_Iteration+1)/(hv_NumIterationsPerEpoch.TupleReal());
  //Important note:
  //Inside of this procedure, we compute iterations like that:
  //*  IterationTmp := int(round(EpochReal * (NumIterationsPerEpoch))-1)
  //If a caller of this procedure supplies a value we should use:
  //*  IterationTmp := int(floor(EpochReal * NumIterationsPerEpoch))
  //
  hv_NumEpochs = hv_NumIterations/(hv_NumIterationsPerEpoch.TupleReal());
  //
  //Note, iterations depend on a specific batch size,
  //hence only epochs are expressive.
  SetDictTuple((*hv_TrainInfo), "epoch", hv_EpochReal);
  SetDictTuple((*hv_TrainInfo), "num_epochs", hv_NumEpochs);
  SetDictTuple((*hv_TrainInfo), "num_iterations_per_epoch", hv_NumIterationsPerEpoch);
  //
  try
  {
    hv_TrainResultsStored = HTuple(hv_TrainResults[(hv_TrainResults.TupleNotEqualElem(-1)).TupleFind(1)]);
  }
  // catch (Exception) 
  catch (HException &HDevExpDefaultException)
  {
    HDevExpDefaultException.ToHTuple(&hv_Exception);
    hv_TrainResultsStored = HTuple();
  }
  //
  //Collect all model parameters.
  GetDlModelParam(hv_DLModelHandle, "type", &hv_ModelType);
  GetParamInfo("get_dl_model_param", "GenParamName", "value_list", &hv_PossibleParamNames);
  CreateDict(&hv_ModelParams);
  SetDictTuple((*hv_TrainInfo), "model_params", hv_ModelParams);
  {
  HTuple end_val32 = (hv_PossibleParamNames.TupleLength())-1;
  HTuple step_val32 = 1;
  for (hv_Index=0; hv_Index.Continue(end_val32, step_val32); hv_Index += step_val32)
  {
    hv_ParamName = HTuple(hv_PossibleParamNames[hv_Index]);
    //Do not collect summary as it cannot change during training and consumes much space.
    if (0 != (int(hv_ParamName==HTuple("summary"))))
    {
      continue;
    }
    //
    try
    {
      if (0 != (int(hv_ParamName==HTuple("device"))))
      {
        //The device handle cannot be serialized. Therefore we get the
        //information via keys and serialize them.
        GetDlModelParam(hv_DLModelHandle, hv_ParamName, &hv_DeviceHandles);
        TupleLength(hv_DeviceHandles, &hv_DeviceLength);
        hv_DeviceTypes = HTuple();
        hv_DeviceNames = HTuple();
        TupleGenConst(hv_DeviceLength, "", &hv_DeviceTypes);
        TupleGenConst(hv_DeviceLength, "", &hv_DeviceNames);
        {
        HTuple end_val49 = (hv_DeviceLength.TupleLength())-1;
        HTuple step_val49 = 1;
        for (hv_DeviceIndex=0; hv_DeviceIndex.Continue(end_val49, step_val49); hv_DeviceIndex += step_val49)
        {
          GetHandleTuple(HTuple(hv_DeviceHandles[hv_DeviceIndex]), "type", &hv_DeviceType);
          GetHandleTuple(HTuple(hv_DeviceHandles[hv_DeviceIndex]), "name", &hv_DeviceName);
          hv_DeviceTypes[hv_DeviceIndex] = hv_DeviceType;
          hv_DeviceNames[hv_DeviceIndex] = hv_DeviceName;
        }
        }
      }
      else
      {
        GetDlModelParam(hv_DLModelHandle, hv_ParamName, &hv_GenParamValue);
      }
    }
    // catch (Exception) 
    catch (HException &HDevExpDefaultException)
    {
      HDevExpDefaultException.ToHTuple(&hv_Exception);
      continue;
    }
    //
    if (0 != (int(hv_ParamName==HTuple("device"))))
    {
      SetDictTuple(hv_ModelParams, "device_type", hv_DeviceTypes);
      SetDictTuple(hv_ModelParams, "device_name", hv_DeviceNames);
    }
    else
    {
      SetDictTuple(hv_ModelParams, hv_ParamName, hv_GenParamValue);
    }
  }
  }
  //
  //Calculate a mean loss value.
  SetDictTuple((*hv_TrainInfo), "mean_loss", HTuple());
  SetDictTuple((*hv_TrainInfo), "mean_loss_samples", 0);
  //
  hv_LossSamplesTrainResults = HTuple();
  TupleFind(hv_TrainResults.TupleNotEqualElem(-1), 1, &hv_Indices);
  if (0 != (int(hv_Indices!=-1)))
  {
    hv_TrainResultsUsed = HTuple(hv_TrainResults[hv_Indices]);
  }
  else
  {
    hv_TrainResultsUsed = HTuple();
  }
  if (0 != (int((hv_TrainResultsUsed.TupleLength())>0)))
  {
    GetDlModelParam(hv_DLModelHandle, "batch_size", &hv_BatchSizeDevice);
    GetDlModelParam(hv_DLModelHandle, "batch_size_multiplier", &hv_BatchSizeMultiplier);
    hv_BatchSize = hv_BatchSizeDevice*hv_BatchSizeMultiplier;
    hv_NumIterationsMean = ((hv_NumSamplesMeanLoss/(hv_BatchSize.TupleReal())).TupleCeil()).TupleInt();
    //
    if (0 != (int(hv_NumIterationsMean==0)))
    {
      hv_TrainResultsUsed = ((const HTuple&)hv_TrainResultsUsed)[(hv_TrainResultsUsed.TupleLength())-1];
    }
    else
    {
      hv_TrainResultsUsed = hv_TrainResultsUsed.TupleSelectRange(((hv_TrainResultsUsed.TupleLength())-hv_NumIterationsMean).TupleMax2(0),(hv_TrainResultsUsed.TupleLength())-1);
    }
    //
    hv_LossParam = "total_loss";
    TupleGenConst(hv_TrainResultsUsed.TupleLength(), -1, &hv_LossValues);
    {
    HTuple end_val95 = (hv_TrainResultsUsed.TupleLength())-1;
    HTuple step_val95 = 1;
    for (hv_Index=0; hv_Index.Continue(end_val95, step_val95); hv_Index += step_val95)
    {
      hv_TrainResult = HTuple(hv_TrainResultsUsed[hv_Index]);
      GetDictTuple(hv_TrainResult, hv_LossParam, &hv_LossValue);
      hv_LossValues[hv_Index] = hv_LossValue;
    }
    }
    hv_LossMean = hv_LossValues.TupleMean();
    SetDictTuple((*hv_TrainInfo), "mean_loss", hv_LossMean);
    SetDictTuple((*hv_TrainInfo), "mean_loss_samples", hv_LossValues.TupleLength());
  }
  //
  //Collect the best evaluation infos.
  hv_BestEvaluationInfo = HTuple();
  hv_BestEvaluationInfoTrain = HTuple();
  hv_BestEvaluationValue = HTuple();
  hv_BestEvaluationValueTrain = HTuple();
  hv_BestEvaluationKeys = HTuple();
  hv_BestEvaluationKeysTrain = HTuple();
  {
  HTuple end_val112 = (hv_EvaluationInfos.TupleLength())-1;
  HTuple step_val112 = 1;
  for (hv_Index=0; hv_Index.Continue(end_val112, step_val112); hv_Index += step_val112)
  {
    hv_EvaluationInfo = HTuple(hv_EvaluationInfos[hv_Index]);
    //Ignore missing infos
    if (0 != (int(hv_EvaluationInfo==-1)))
    {
      continue;
    }
    //
    GetDictTuple(hv_EvaluationInfo, "result", &hv_ValidationEvaluationResult);
    GetDictTuple(hv_EvaluationInfo, "result_train", &hv_TrainEvaluationResult);
    //
    //Reduce the result to a single (mean) value.
    reduce_dl_evaluation_result(hv_ValidationEvaluationResult, hv_EvaluationComparisonKeys, 
        &hv_Value, &hv_ValidEvaluationKeys);
    reduce_dl_evaluation_result(hv_TrainEvaluationResult, hv_EvaluationComparisonKeys, 
        &hv_ValueTrain, &hv_ValidEvaluationKeysTrain);
    //
    //Compare current evaluation result with the best one.
    if (0 != (int(hv_ModelType==HTuple("classification"))))
    {
      //Validation.
      if (0 != (HTuple(int((hv_BestEvaluationInfo.TupleLength())==0)).TupleOr(int(hv_Value<=hv_BestEvaluationValue))))
      {
        hv_BestEvaluationInfo = hv_EvaluationInfo;
        hv_BestEvaluationValue = hv_Value;
        hv_BestEvaluationKeys = hv_ValidEvaluationKeys;
      }
      //Training.
      if (0 != (HTuple(int((hv_BestEvaluationInfoTrain.TupleLength())==0)).TupleOr(int(hv_ValueTrain<=hv_BestEvaluationValueTrain))))
      {
        hv_BestEvaluationInfoTrain = hv_EvaluationInfo;
        hv_BestEvaluationValueTrain = hv_ValueTrain;
        hv_BestEvaluationKeysTrain = hv_ValidEvaluationKeysTrain;
      }
    }
    else
    {
      //Validation.
      if (0 != (HTuple(int((hv_BestEvaluationInfo.TupleLength())==0)).TupleOr(int(hv_Value>=hv_BestEvaluationValue))))
      {
        hv_BestEvaluationInfo = hv_EvaluationInfo;
        hv_BestEvaluationValue = hv_Value;
        hv_BestEvaluationKeys = hv_ValidEvaluationKeys;
      }
      //Training.
      if (0 != (HTuple(int((hv_BestEvaluationInfoTrain.TupleLength())==0)).TupleOr(int(hv_ValueTrain>=hv_BestEvaluationValueTrain))))
      {
        hv_BestEvaluationInfoTrain = hv_EvaluationInfo;
        hv_BestEvaluationValueTrain = hv_ValueTrain;
        hv_BestEvaluationKeysTrain = hv_ValidEvaluationKeysTrain;
      }
    }
  }
  }
  //
  //Store best evaluation information.
  if (0 != (int((hv_BestEvaluationInfo.TupleLength())>0)))
  {
    CreateDict(&hv_BestEvaluationData);
    SetDictTuple(hv_BestEvaluationData, "comparison_keys", hv_BestEvaluationKeys);
    SetDictTuple(hv_BestEvaluationData, "best_info", hv_BestEvaluationInfo);
    SetDictTuple(hv_BestEvaluationData, "best_value", hv_BestEvaluationValue);
    SetDictTuple(hv_BestEvaluationData, "comparison_keys_train", hv_BestEvaluationKeysTrain);
    SetDictTuple(hv_BestEvaluationData, "best_info_train", hv_BestEvaluationInfoTrain);
    SetDictTuple(hv_BestEvaluationData, "best_value_train", hv_BestEvaluationValueTrain);
    SetDictTuple((*hv_TrainInfo), "best_evaluation", hv_BestEvaluationData);
  }
  else
  {
    SetDictTuple((*hv_TrainInfo), "best_evaluation", HTuple());
  }
  //
  return;
}

// Chapter: Deep Learning / Anomaly Detection
// Short Description: Compute thresholds for DL Anomaly Detection. 
void compute_dl_anomaly_thresholds (HTuple hv_DLModelHandle, HTuple hv_DLDataset, 
    HTuple hv_GenParam, HTuple *hv_AnomalySegmentationThreshold, HTuple *hv_AnomalyClassificationThresholds)
{

  // Local iconic variables
  HObject  ho_AnomalyImage, ho_Domain;

  // Local control variables
  HTuple  hv_ModelType, hv_DLSamples, hv_SplitExists;
  HTuple  hv_UseOkSamplesOnly, hv_EnableDisplay, hv_UseOkSamplesOnlyExists;
  HTuple  hv_UseOkSamplesOnlyKey, hv_EnableDisplayExists;
  HTuple  hv_ValidationIndices, hv_WindowWidth, hv_WindowHeight;
  HTuple  hv_WindowBGColor, hv_WindowHandleInfo, hv_ThresholdInformation;
  HTuple  hv_ValidationSamples, hv_ValidationResults, hv_Values;
  HTuple  hv_Index, hv_Rows, hv_Columns, hv_Grayval, hv_Eps;
  HTuple  hv_StandardDeviationFactor, hv_TestIndices, hv_TestIndicesNOK;
  HTuple  hv_TestIndicesOK, hv_TestSamplesOK, hv_TestResultsOK;
  HTuple  hv_AnomalyScoresOK, hv_AnomalyScore, hv_TestSamplesNOK;
  HTuple  hv_TestResultsNOK, hv_AnomalyScoresNOK, hv_MaxScoreOK;
  HTuple  hv_MinScoreNOK, hv_IntermediateThreshold, hv_AnomalyScores;
  HTuple  hv_AnomalyScoresSortIndices, hv_AnomalyScoresSorted;
  HTuple  hv_TmpOK, hv_TrueNegativesPerScore, hv_FPRate, hv_TmpNOK;
  HTuple  hv_FalseNegativesPerScore, hv_FNRate, hv_FprFnrSum;
  HTuple  hv_MinFprFnRSum, hv_ThresholdIndex;

  //This procedure estimates two different thresholds used in deep-learning-based anomaly detection.
  //These thresholds are used for:
  //1) Region segmentation: AnomalySegmentationThreshold can be used as threshold
  //    whether a pixel within the anomaly image belongs to a region of an anomaly.
  //   The threshold is estimated based on the samples in the validation split of DLDataset.
  //2) Image classification: AnomalyClassificationThresholds can be used as threshold
  //   whether the image is counted as containing an anomaly.
  //   It consists of a tuple with the following values:
  // - the maximal anomaly score obtianed for the ok test samples
  // - the minimal anomaly score obtained for the nok test samples
  // - an intermediate threshold for the anomaly scores that minimizes
  //     the sum of the false negative rate and the false positive rate.
  //   Note, the latter two values are only calculated if nok test samples are available.
  //
  //Check model type.
  GetDlModelParam(hv_DLModelHandle, "type", &hv_ModelType);
  if (0 != (int(hv_ModelType!=HTuple("anomaly_detection"))))
  {
    throw HException("This procedure can only be used for models of type 'anomaly_detection'.");
  }
  //
  //Get sample entries in DLDataset.
  GetDictTuple(hv_DLDataset, "samples", &hv_DLSamples);
  //
  //Check whether the dataset is split.
  GetDictParam(HTuple(hv_DLSamples[0]), "key_exists", "split", &hv_SplitExists);
  if (0 != (hv_SplitExists.TupleNot()))
  {
    throw HException("This procedure can only be used if DLDataset has already been split.");
  }
  //
  //Test GenParam whether 'nok' samples should be processed.
  hv_UseOkSamplesOnly = 0;
  hv_EnableDisplay = 0;
  if (0 != (int(hv_GenParam!=HTuple())))
  {
    GetDictParam(hv_GenParam, "key_exists", "use_ok_samples_only", &hv_UseOkSamplesOnlyExists);
    if (0 != hv_UseOkSamplesOnlyExists)
    {
      GetDictTuple(hv_GenParam, "use_ok_samples_only", &hv_UseOkSamplesOnlyKey);
      if (0 != (HTuple(int(hv_UseOkSamplesOnlyKey==HTuple("true"))).TupleOr(int(hv_UseOkSamplesOnlyKey==1))))
      {
        hv_UseOkSamplesOnly = 1;
      }
    }
    GetDictParam(hv_GenParam, "key_exists", "enable_display", &hv_EnableDisplayExists);
    if (0 != hv_EnableDisplayExists)
    {
      GetDictTuple(hv_GenParam, "enable_display", &hv_EnableDisplay);
      if (0 != (HTuple(int(hv_EnableDisplay==HTuple("true"))).TupleOr(int(hv_EnableDisplay==1))))
      {
        hv_EnableDisplay = 1;
      }
    }
  }
  //
  //Determine the threshold for region segmentation: AnomalySegmentationThreshold.
  //
  //Get the samples in the validation split.
  find_dl_samples(hv_DLSamples, "split", "validation", "or", &hv_ValidationIndices);
  //If the validation split is empty, the segmentation threshold cannot be estimated.
  if (0 != (int((hv_ValidationIndices.TupleLength())==0)))
  {
    throw HException("This procedure can only be used with at least one validation image.");
  }
  //
  //Display progress message.
  if (0 != hv_EnableDisplay)
  {
    hv_WindowWidth = 500;
    hv_WindowHeight = 200;
    hv_WindowBGColor = "light gray";
    //
    //Open and setup text window.
    SetWindowAttr("background_color",hv_WindowBGColor);
    OpenWindow(0,0,hv_WindowWidth,hv_WindowHeight,0,"visible","",&hv_WindowHandleInfo);
    HDevWindowStack::Push(hv_WindowHandleInfo);
    set_display_font(hv_WindowHandleInfo, 16, "mono", "true", "false");
    hv_ThresholdInformation.Clear();
    hv_ThresholdInformation[0] = "Computing thresholds.";
    hv_ThresholdInformation[1] = "";
    hv_ThresholdInformation[2] = "This may take some time...";
    if (HDevWindowStack::IsOpen())
      DispText(HDevWindowStack::GetActive(),hv_ThresholdInformation, "window", "top", 
          "left", "black", "box", "false");
  }
  //
  read_dl_samples(hv_DLDataset, hv_ValidationIndices, &hv_ValidationSamples);
  //
  //Get the gray values of the anomaly images for the validation split.
  ApplyDlModel(hv_DLModelHandle, hv_ValidationSamples, HTuple(), &hv_ValidationResults);
  hv_Values = HTuple();
  {
  HTuple end_val76 = (hv_ValidationResults.TupleLength())-1;
  HTuple step_val76 = 1;
  for (hv_Index=0; hv_Index.Continue(end_val76, step_val76); hv_Index += step_val76)
  {
    GetDictObject(&ho_AnomalyImage, HTuple(hv_ValidationResults[hv_Index]), "anomaly_image");
    GetDomain(ho_AnomalyImage, &ho_Domain);
    GetRegionPoints(ho_Domain, &hv_Rows, &hv_Columns);
    GetGrayval(ho_AnomalyImage, hv_Rows, hv_Columns, &hv_Grayval);
    hv_Values = hv_Values.TupleConcat(hv_Grayval);
  }
  }
  //
  //Set a small value which is used to slightly increase the calculated thresholds
  //to ensure correct classification of OK scores.
  hv_Eps = 1e-15;
  //
  //Compute the estimated threshold.
  //We take the mean of the anomaly images plus four times their standard deviation.
  //This ensures that almost all gray values in the anomaly images
  //of the validation samples are below threshold, while the estimate
  //is still robust against unexpected outliers.
  hv_StandardDeviationFactor = 4.0;
  (*hv_AnomalySegmentationThreshold) = ((hv_Values.TupleMean())+(hv_StandardDeviationFactor*(hv_Values.TupleDeviation()))).TupleMin2(1.0);
  //
  //Determine the thresholds for anomaly image classification: AnomalyClassificationThresholds.
  //
  //Get the indices of the ok and nok test samples.
  find_dl_samples(hv_DLSamples, "split", "test", "or", &hv_TestIndices);
  find_dl_samples(hv_DLSamples, "anomaly_label", "ng", "or", &hv_TestIndicesNOK);
  hv_TestIndicesOK = hv_TestIndices.TupleDifference(hv_TestIndicesNOK);
  //
  if (0 != (int((hv_TestIndicesOK.TupleLength())==0)))
  {
    //No thresholds are computed.
    throw HException("This procedure requires at least one test image labeled as 'ok'.");
  }
  //
  //Compute the anomaly scores of the ok test samples
  read_dl_samples(hv_DLDataset, hv_TestIndicesOK, &hv_TestSamplesOK);
  ApplyDlModel(hv_DLModelHandle, hv_TestSamplesOK, HTuple(), &hv_TestResultsOK);
  hv_AnomalyScoresOK = HTuple(hv_TestResultsOK.TupleLength(),0.0);
  {
  HTuple end_val112 = (hv_TestResultsOK.TupleLength())-1;
  HTuple step_val112 = 1;
  for (hv_Index=0; hv_Index.Continue(end_val112, step_val112); hv_Index += step_val112)
  {
    GetDictTuple(HTuple(hv_TestResultsOK[hv_Index]), "anomaly_score", &hv_AnomalyScore);
    hv_AnomalyScoresOK[hv_Index] = hv_AnomalyScore;
  }
  }
  //
  //In case only test samples labeled as ok are used,
  //only one anomaly image classification threshold is returned.
  if (0 != (HTuple(int((hv_TestIndicesNOK.TupleLength())==0)).TupleOr(hv_UseOkSamplesOnly)))
  {
    (*hv_AnomalyClassificationThresholds) = (hv_AnomalyScoresOK.TupleMax())*(1+hv_Eps);
  }
  else
  {
    //Compute the anomaly scores of the nok test samples.
    read_dl_samples(hv_DLDataset, hv_TestIndicesNOK, &hv_TestSamplesNOK);
    ApplyDlModel(hv_DLModelHandle, hv_TestSamplesNOK, HTuple(), &hv_TestResultsNOK);
    hv_AnomalyScoresNOK = HTuple(hv_TestResultsNOK.TupleLength(),0.0);
    {
    HTuple end_val126 = (hv_TestResultsNOK.TupleLength())-1;
    HTuple step_val126 = 1;
    for (hv_Index=0; hv_Index.Continue(end_val126, step_val126); hv_Index += step_val126)
    {
      GetDictTuple(HTuple(hv_TestResultsNOK[hv_Index]), "anomaly_score", &hv_AnomalyScore);
      hv_AnomalyScoresNOK[hv_Index] = hv_AnomalyScore;
    }
    }
    //Get minimum and maximum values of the anomaly scores.
    hv_MaxScoreOK = hv_AnomalyScoresOK.TupleMax();
    hv_MinScoreNOK = hv_AnomalyScoresNOK.TupleMin();
    if (0 != (int(hv_MaxScoreOK<=hv_MinScoreNOK)))
    {
      //In this case the ok and nok samples can be perfectly separated
      //by a threshold.
      hv_IntermediateThreshold = (hv_MaxScoreOK+hv_MinScoreNOK)/2;
    }
    else
    {
      //In this case there will be false positives or false negatives
      //for any threshold.
      //
      //Compute the histograms of the ok and nok scores.
      //
      //Sort the anomaly scores.
      hv_AnomalyScores.Clear();
      hv_AnomalyScores.Append(hv_AnomalyScoresOK);
      hv_AnomalyScores.Append(hv_AnomalyScoresNOK);
      hv_AnomalyScoresSortIndices = hv_AnomalyScores.TupleSortIndex();
      hv_AnomalyScoresSorted = HTuple(hv_AnomalyScores[hv_AnomalyScoresSortIndices]);
      //
      //Compute the false positive rates.
      hv_TmpOK.Clear();
      hv_TmpOK.Append(HTuple(hv_AnomalyScoresOK.TupleLength(),1.0));
      hv_TmpOK.Append(HTuple(hv_AnomalyScoresNOK.TupleLength(),0.0));
      hv_TrueNegativesPerScore = HTuple(hv_TmpOK[hv_AnomalyScoresSortIndices]).TupleCumul();
      hv_FPRate = 1.0-(hv_TrueNegativesPerScore/(hv_AnomalyScoresOK.TupleLength()));
      //
      //Compute the false negative rates.
      hv_TmpNOK.Clear();
      hv_TmpNOK.Append(HTuple(hv_AnomalyScoresOK.TupleLength(),0.0));
      hv_TmpNOK.Append(HTuple(hv_AnomalyScoresNOK.TupleLength(),1.0));
      hv_FalseNegativesPerScore = HTuple(hv_TmpNOK[hv_AnomalyScoresSortIndices]).TupleCumul();
      hv_FNRate = hv_FalseNegativesPerScore/(hv_AnomalyScoresNOK.TupleLength());
      //
      //Get the threshold for which the sum of the false positive
      //and false negative rates is the lowest.
      hv_FprFnrSum = hv_FPRate+hv_FNRate;
      hv_MinFprFnRSum = hv_FprFnrSum.TupleMin();
      hv_ThresholdIndex = hv_FprFnrSum.TupleFindFirst(hv_MinFprFnRSum);
      hv_IntermediateThreshold = HTuple(hv_AnomalyScoresSorted[hv_ThresholdIndex]);
      //
      //In some cases IntermediateThreshold may be smaller than MinScoreNOK.
      //We set it to at least that value.
      hv_IntermediateThreshold = hv_IntermediateThreshold.TupleMax2(hv_MinScoreNOK);
      //
    }
    //Set the tuple of anomaly classification thresholds.
    (*hv_AnomalyClassificationThresholds).Clear();
    (*hv_AnomalyClassificationThresholds).Append(hv_MaxScoreOK*(1+hv_Eps));
    (*hv_AnomalyClassificationThresholds).Append(hv_MinScoreNOK);
    (*hv_AnomalyClassificationThresholds).Append(hv_IntermediateThreshold);
  }
  //
  return;
}

// Chapter: Deep Learning / Classification
// Short Description: Calculate top-K error. 
void compute_top_k_error (HTuple hv_ImageLabelIDs, HTuple hv_TopKPredictions, HTuple hv_K, 
    HTuple *hv_TopKError)
{

  // Local iconic variables

  // Local control variables
  HTuple  hv_NumMatches, hv_Index, hv_Predictions;
  HTuple  hv_PredictedClasses;

  //
  //This procedure calculates the top-K error out of the given predictions and labels.
  //
  hv_NumMatches = 0;
  //
  //Loop through all selected ground truth labels.
  {
  HTuple end_val6 = (hv_ImageLabelIDs.TupleLength())-1;
  HTuple step_val6 = 1;
  for (hv_Index=0; hv_Index.Continue(end_val6, step_val6); hv_Index += step_val6)
  {
    //Get the K best results.
    GetDictTuple(HTuple(hv_TopKPredictions[hv_Index]), "predictions", &hv_Predictions);
    hv_PredictedClasses = hv_Predictions.TupleSelectRange(0,hv_K-1);
    //Count how often the ground truth label
    //and K predicted classes match.
    if (0 != (int((hv_PredictedClasses.TupleFind(HTuple(hv_ImageLabelIDs[hv_Index])))!=-1)))
    {
      hv_NumMatches += 1;
    }
  }
  }
  (*hv_TopKError) = 1.0-((hv_NumMatches.TupleReal())/(hv_ImageLabelIDs.TupleLength()));
  //
  return;
}

// Chapter: Tools / Geometry
// Short Description: Convert the parameters of rectangles with format rectangle2 to the coordinates of its 4 corner-points. 
void convert_rect2_5to8param (HTuple hv_Row, HTuple hv_Col, HTuple hv_Length1, HTuple hv_Length2, 
    HTuple hv_Phi, HTuple *hv_Row1, HTuple *hv_Col1, HTuple *hv_Row2, HTuple *hv_Col2, 
    HTuple *hv_Row3, HTuple *hv_Col3, HTuple *hv_Row4, HTuple *hv_Col4)
{

  // Local iconic variables

  // Local control variables
  HTuple  hv_Co1, hv_Co2, hv_Si1, hv_Si2;

  //This procedure takes the parameters for a rectangle of type 'rectangle2'
  //and returns the coordinates of the four corners.
  //
  hv_Co1 = (hv_Phi.TupleCos())*hv_Length1;
  hv_Co2 = (hv_Phi.TupleCos())*hv_Length2;
  hv_Si1 = (hv_Phi.TupleSin())*hv_Length1;
  hv_Si2 = (hv_Phi.TupleSin())*hv_Length2;

  (*hv_Col1) = (hv_Co1-hv_Si2)+hv_Col;
  (*hv_Row1) = ((-hv_Si1)-hv_Co2)+hv_Row;
  (*hv_Col2) = ((-hv_Co1)-hv_Si2)+hv_Col;
  (*hv_Row2) = (hv_Si1-hv_Co2)+hv_Row;
  (*hv_Col3) = ((-hv_Co1)+hv_Si2)+hv_Col;
  (*hv_Row3) = (hv_Si1+hv_Co2)+hv_Row;
  (*hv_Col4) = (hv_Co1+hv_Si2)+hv_Col;
  (*hv_Row4) = ((-hv_Si1)+hv_Co2)+hv_Row;

  return;
}

// Chapter: Tools / Geometry
// Short Description: Convert for four-sided figures the coordinates of the 4 corner-points to the parameters of format rectangle2. 
void convert_rect2_8to5param (HTuple hv_Row1, HTuple hv_Col1, HTuple hv_Row2, HTuple hv_Col2, 
    HTuple hv_Row3, HTuple hv_Col3, HTuple hv_Row4, HTuple hv_Col4, HTuple hv_ForceL1LargerL2, 
    HTuple *hv_Row, HTuple *hv_Col, HTuple *hv_Length1, HTuple *hv_Length2, HTuple *hv_Phi)
{

  // Local iconic variables

  // Local control variables
  HTuple  hv_Hor, hv_Vert, hv_IdxSwap, hv_Tmp;

  //This procedure takes the corners of four-sided figures
  //and returns the parameters of type 'rectangle2'.
  //
  //Calculate center row and column.
  (*hv_Row) = (((hv_Row1+hv_Row2)+hv_Row3)+hv_Row4)/4.0;
  (*hv_Col) = (((hv_Col1+hv_Col2)+hv_Col3)+hv_Col4)/4.0;
  //Length1 and Length2.
  (*hv_Length1) = ((((hv_Row1-hv_Row2)*(hv_Row1-hv_Row2))+((hv_Col1-hv_Col2)*(hv_Col1-hv_Col2))).TupleSqrt())/2.0;
  (*hv_Length2) = ((((hv_Row2-hv_Row3)*(hv_Row2-hv_Row3))+((hv_Col2-hv_Col3)*(hv_Col2-hv_Col3))).TupleSqrt())/2.0;
  //Calculate the angle phi.
  hv_Hor = hv_Col1-hv_Col2;
  hv_Vert = hv_Row2-hv_Row1;
  if (0 != hv_ForceL1LargerL2)
  {
    //Swap length1 and length2 if necessary.
    hv_IdxSwap = (((*hv_Length2)-(*hv_Length1)).TupleGreaterElem(1e-9)).TupleFind(1);
    if (0 != (int(hv_IdxSwap!=-1)))
    {
      hv_Tmp = HTuple((*hv_Length1)[hv_IdxSwap]);
      (*hv_Length1)[hv_IdxSwap] = HTuple((*hv_Length2)[hv_IdxSwap]);
      (*hv_Length2)[hv_IdxSwap] = hv_Tmp;
      hv_Hor[hv_IdxSwap] = HTuple(hv_Col2[hv_IdxSwap])-HTuple(hv_Col3[hv_IdxSwap]);
      hv_Vert[hv_IdxSwap] = HTuple(hv_Row3[hv_IdxSwap])-HTuple(hv_Row2[hv_IdxSwap]);
    }
  }
  (*hv_Phi) = hv_Vert.TupleAtan2(hv_Hor);
  //
  return;
}

// Chapter: Deep Learning / Anomaly Detection
// Short Description: This procedure creates a block of samples for a given thread. 
void create_dl_dataset_anomaly_samples (HTuple hv_ImageList, HTuple hv_LabelList, 
    HTuple hv_AnomalyList, HTuple hv_AnomalyDirGiven, HTuple hv_NumSamplesPerThread, 
    HTuple hv_UniqueIndex, HTuple *hv_SamplesOutput, HTuple *hv_Exception)
{

  // Local iconic variables

  // Local control variables
  HTuple  hv_SampleIndex, hv_Sample, hv_Label, hv_AnomalyFileName;

  //This procedure creates a block of samples for a given thread.
  TupleGenConst(hv_NumSamplesPerThread, "", &(*hv_SamplesOutput));
  (*hv_Exception) = HTuple();
  //
  //Loop over samples.
  {
  HTuple end_val5 = hv_NumSamplesPerThread-1;
  HTuple step_val5 = 1;
  for (hv_SampleIndex=0; hv_SampleIndex.Continue(end_val5, step_val5); hv_SampleIndex += step_val5)
  {
    //
    //Create the dictionary Sample.
    CreateDict(&hv_Sample);
    //
    //Set information about this sample.
    SetDictTuple(hv_Sample, "image_id", hv_UniqueIndex);
    SetDictTuple(hv_Sample, "image_file_name", HTuple(hv_ImageList[hv_SampleIndex]));
    hv_UniqueIndex += 1;
    //
    hv_Label = HTuple(hv_LabelList[hv_SampleIndex]);
    if (0 != (int(hv_Label==HTuple(""))))
    {
      (*hv_Exception) = "Could not determine label for image: "+HTuple(hv_ImageList[hv_SampleIndex]);
      return;
    }
    //
    //Match for good labels.
    if (0 != (HTuple(int((hv_Label.TupleRegexpMatch((HTuple("^ok$").Append("ignore_case"))))!=HTuple(""))).TupleOr(int((hv_Label.TupleRegexpMatch((HTuple("^good$").Append("ignore_case"))))!=HTuple("")))))
    {
      SetDictTuple(hv_Sample, "anomaly_label", "ok");
    }
    else
    {
      SetDictTuple(hv_Sample, "anomaly_label", "nok");
    }
    //
    //Check if sample has an anomaly annotation.
    if (0 != hv_AnomalyDirGiven)
    {
      hv_AnomalyFileName = HTuple(hv_AnomalyList[hv_SampleIndex]);
      if (0 != (int(hv_AnomalyFileName!=HTuple(""))))
      {
        SetDictTuple(hv_Sample, "anomaly_file_name", hv_AnomalyFileName);
      }
    }
    //
    //Append this sample.
    (*hv_SamplesOutput)[hv_SampleIndex] = hv_Sample;
  }
  }
  //
  return;
}

// Chapter: Deep Learning / Model
// Short Description: Creates a dictionary with preprocessing parameters. 
void create_dl_preprocess_param (HTuple hv_DLModelType, HTuple hv_ImageWidth, HTuple hv_ImageHeight, 
    HTuple hv_ImageNumChannels, HTuple hv_ImageRangeMin, HTuple hv_ImageRangeMax, 
    HTuple hv_NormalizationType, HTuple hv_DomainHandling, HTuple hv_IgnoreClassIDs, 
    HTuple hv_SetBackgroundID, HTuple hv_ClassIDsBackground, HTuple hv_GenParam, 
    HTuple *hv_DLPreprocessParam)
{

  // Local control variables
  HTuple  hv_GenParamNames, hv_GenParamIndex, hv_GenParamValue;
  HTuple  hv_KeysExist, hv_InstanceType;

  //
  //This procedure creates a dictionary with all parameters needed for preprocessing.
  //
  CreateDict(&(*hv_DLPreprocessParam));
  SetDictTuple((*hv_DLPreprocessParam), "model_type", hv_DLModelType);
  SetDictTuple((*hv_DLPreprocessParam), "image_width", hv_ImageWidth);
  SetDictTuple((*hv_DLPreprocessParam), "image_height", hv_ImageHeight);
  SetDictTuple((*hv_DLPreprocessParam), "image_num_channels", hv_ImageNumChannels);
  if (0 != (int(hv_ImageRangeMin==HTuple())))
  {
    SetDictTuple((*hv_DLPreprocessParam), "image_range_min", -127);
  }
  else
  {
    SetDictTuple((*hv_DLPreprocessParam), "image_range_min", hv_ImageRangeMin);
  }
  if (0 != (int(hv_ImageRangeMax==HTuple())))
  {
    SetDictTuple((*hv_DLPreprocessParam), "image_range_max", 128);
  }
  else
  {
    SetDictTuple((*hv_DLPreprocessParam), "image_range_max", hv_ImageRangeMax);
  }
  SetDictTuple((*hv_DLPreprocessParam), "normalization_type", hv_NormalizationType);
  //Replace possible legacy parameters.
  replace_legacy_preprocessing_parameters((*hv_DLPreprocessParam));
  SetDictTuple((*hv_DLPreprocessParam), "domain_handling", hv_DomainHandling);
  //
  //Set segmentation specific parameters.
  if (0 != (int(hv_DLModelType==HTuple("segmentation"))))
  {
    SetDictTuple((*hv_DLPreprocessParam), "ignore_class_ids", hv_IgnoreClassIDs);
    SetDictTuple((*hv_DLPreprocessParam), "set_background_id", hv_SetBackgroundID);
    SetDictTuple((*hv_DLPreprocessParam), "class_ids_background", hv_ClassIDsBackground);
  }
  //
  //Set generic parameters.
  if (0 != (int(hv_GenParam!=HTuple())))
  {
    GetDictParam(hv_GenParam, "keys", HTuple(), &hv_GenParamNames);
    {
    HTuple end_val33 = (hv_GenParamNames.TupleLength())-1;
    HTuple step_val33 = 1;
    for (hv_GenParamIndex=0; hv_GenParamIndex.Continue(end_val33, step_val33); hv_GenParamIndex += step_val33)
    {
      GetDictTuple(hv_GenParam, HTuple(hv_GenParamNames[hv_GenParamIndex]), &hv_GenParamValue);
      SetDictTuple((*hv_DLPreprocessParam), HTuple(hv_GenParamNames[hv_GenParamIndex]), 
          hv_GenParamValue);
    }
    }
  }
  //
  //Set necessary default values.
  if (0 != (int(hv_DLModelType==HTuple("detection"))))
  {
    GetDictParam((*hv_DLPreprocessParam), "key_exists", (HTuple("instance_type").Append("ignore_direction")), 
        &hv_KeysExist);
    if (0 != (HTuple(hv_KeysExist[0]).TupleNot()))
    {
      SetDictTuple((*hv_DLPreprocessParam), "instance_type", "rectangle1");
    }
    //Set default for 'ignore_direction' only if instance_type is 'rectangle2'.
    GetDictTuple((*hv_DLPreprocessParam), "instance_type", &hv_InstanceType);
    if (0 != (HTuple(int(hv_InstanceType==HTuple("rectangle2"))).TupleAnd(HTuple(hv_KeysExist[1]).TupleNot())))
    {
      SetDictTuple((*hv_DLPreprocessParam), "ignore_direction", 0);
    }
  }
  //
  //Check the validity of the preprocessing parameters.
  check_dl_preprocess_param((*hv_DLPreprocessParam));
  //
  return;
}

// Chapter: Deep Learning / Model
// Short Description: Create a training parameter dictionary which is used in train_dl_model. 
void create_dl_train_param (HTuple hv_DLModelHandle, HTuple hv_NumEpochs, HTuple hv_EvaluationIntervalEpochs, 
    HTuple hv_EnableDisplay, HTuple hv_RandomSeed, HTuple hv_GenParamName, HTuple hv_GenParamValue, 
    HTuple *hv_TrainParam)
{

  // Local iconic variables

  // Local control variables
  HTuple  hv_ModelType, hv_AvailableGenParam, hv_IndexGenParam;
  HTuple  hv_IndexFind, hv_IsString, hv_TrainParamAnomaly;
  HTuple  hv_DomainRatioKeyExists, hv_ErrorThresholdKeyExists;
  HTuple  hv_RegularizationNoiseKeyExists, hv_DisplayParam;
  HTuple  hv_EvaluationParam, hv_AugmentationParam, hv_ClassIDsNoOrientation;
  HTuple  hv_Exception, hv_ChangeStrategies, hv_Indices, hv_SerializationStrategy;
  HTuple  hv_SerializationStrategies, hv_Seconds, hv_SetDisplayParam;
  HTuple  hv_EvaluationComparisonKeys, hv_BestScoreHighest;
  HTuple  hv_ConvertToMean, hv_Index, hv_FoundIndices;

  //
  //This procedure creates a dictionary with all needed training parameters,
  //as required by train_dl_model as input.
  //
  //Check length of input GenParam tuple.
  if (0 != (int((hv_GenParamName.TupleLength())!=(hv_GenParamValue.TupleLength()))))
  {
    throw HException("GenParamName and GenParamValue have to have the same length.");
  }
  //
  //Some default parameters depend on model type.
  GetDlModelParam(hv_DLModelHandle, "type", &hv_ModelType);
  if (0 != (HTuple(HTuple(HTuple(int(hv_ModelType!=HTuple("classification"))).TupleAnd(int(hv_ModelType!=HTuple("detection")))).TupleAnd(int(hv_ModelType!=HTuple("segmentation")))).TupleAnd(int(hv_ModelType!=HTuple("anomaly_detection")))))
  {
    throw HException(("Current model type is not supported: \""+hv_ModelType)+"\"");
  }
  //
  //Check if the given GenParamName strings are available.
  if (0 != (int(hv_ModelType!=HTuple("anomaly_detection"))))
  {
    hv_AvailableGenParam.Clear();
    hv_AvailableGenParam[0] = "evaluate";
    hv_AvailableGenParam[1] = "augment";
    hv_AvailableGenParam[2] = "change";
    hv_AvailableGenParam[3] = "serialize";
    hv_AvailableGenParam[4] = "display";
  }
  else
  {
    hv_AvailableGenParam = "anomaly";
  }
  {
  HTuple end_val21 = (hv_GenParamName.TupleLength())-1;
  HTuple step_val21 = 1;
  for (hv_IndexGenParam=0; hv_IndexGenParam.Continue(end_val21, step_val21); hv_IndexGenParam += step_val21)
  {
    hv_IndexFind = hv_AvailableGenParam.TupleFind(HTuple(hv_GenParamName[hv_IndexGenParam]));
    if (0 != (int(hv_IndexFind==-1)))
    {
      throw HException(("The provided GenParamName "+HTuple(hv_GenParamName[hv_IndexGenParam]))+" is invalid.");
    }
  }
  }
  //
  //Check if display is enabled.
  TupleIsString(hv_EnableDisplay, &hv_IsString);
  if (0 != hv_IsString)
  {
    hv_EnableDisplay = int(hv_EnableDisplay==HTuple("true"));
  }
  else
  {
    hv_EnableDisplay = int(hv_EnableDisplay==1);
  }
  //
  //Initialize the dictionary holding the training parameters.
  CreateDict(&(*hv_TrainParam));
  //
  //** User supplied parameters: ***
  //
  //Set training parameters for anomaly detection models.
  if (0 != (int(hv_ModelType==HTuple("anomaly_detection"))))
  {
    get_genparam_single_value(hv_GenParamName, hv_GenParamValue, "anomaly", &hv_TrainParamAnomaly);
    //Set default values in case no values are provided.
    if (0 != (int(hv_TrainParamAnomaly==HTuple())))
    {
      CreateDict(&hv_TrainParamAnomaly);
    }
    GetDictParam(hv_TrainParamAnomaly, "key_exists", "domain_ratio", &hv_DomainRatioKeyExists);
    if (0 != (hv_DomainRatioKeyExists.TupleNot()))
    {
      SetDictTuple(hv_TrainParamAnomaly, "domain_ratio", 0.1);
    }
    GetDictParam(hv_TrainParamAnomaly, "key_exists", "error_threshold", &hv_ErrorThresholdKeyExists);
    if (0 != (hv_ErrorThresholdKeyExists.TupleNot()))
    {
      SetDictTuple(hv_TrainParamAnomaly, "error_threshold", 0.001);
    }
    GetDictParam(hv_TrainParamAnomaly, "key_exists", "regularization_noise", &hv_RegularizationNoiseKeyExists);
    if (0 != (hv_RegularizationNoiseKeyExists.TupleNot()))
    {
      SetDictTuple(hv_TrainParamAnomaly, "regularization_noise", 0.0001);
    }
    //
    SetDictTuple(hv_TrainParamAnomaly, "max_num_epochs", hv_NumEpochs);
    SetDictTuple((*hv_TrainParam), "anomaly_param", hv_TrainParamAnomaly);
    CreateDict(&hv_DisplayParam);
    SetDictTuple(hv_DisplayParam, "enabled", hv_EnableDisplay);
    SetDictTuple((*hv_TrainParam), "display_param", hv_DisplayParam);
    return;
  }
  //
  //Set training parameters for non-anomaly models.
  //
  //Number of epochs to train the model on the train split of the dataset.
  SetDictTuple((*hv_TrainParam), "num_epochs", hv_NumEpochs);
  //
  //Interval (in epochs) to evaluate the model on the validation split of the dataset.
  SetDictTuple((*hv_TrainParam), "evaluation_interval_epochs", hv_EvaluationIntervalEpochs);
  //
  //Transfer evaluation parameters used in further steps.
  get_genparam_single_value(hv_GenParamName, hv_GenParamValue, "evaluate", &hv_EvaluationParam);
  SetDictTuple((*hv_TrainParam), "evaluation_param", hv_EvaluationParam);
  //
  //Transfer augmentation parameters used in further steps.
  get_genparam_single_value(hv_GenParamName, hv_GenParamValue, "augment", &hv_AugmentationParam);
  if (0 != (int(hv_ModelType==HTuple("detection"))))
  {
    //In addition, add class IDs without orientation, since these classes require
    //special treatment during the augmentation.
    try
    {
      GetDlModelParam(hv_DLModelHandle, "class_ids_no_orientation", &hv_ClassIDsNoOrientation);
    }
    // catch (Exception) 
    catch (HException &HDevExpDefaultException)
    {
      HDevExpDefaultException.ToHTuple(&hv_Exception);
      hv_ClassIDsNoOrientation = HTuple();
    }
    if (0 != (int((hv_ClassIDsNoOrientation.TupleLength())>0)))
    {
      if (0 != (int(hv_AugmentationParam==HTuple())))
      {
        CreateDict(&hv_AugmentationParam);
      }
      SetDictTuple(hv_AugmentationParam, "class_ids_no_orientation", hv_ClassIDsNoOrientation);
    }
  }
  SetDictTuple((*hv_TrainParam), "augmentation_param", hv_AugmentationParam);
  //
  //Change strategies for any parameters that need to be changed during training.
  get_genparam_single_value(hv_GenParamName, hv_GenParamValue, "change", &hv_ChangeStrategies);
  SetDictTuple((*hv_TrainParam), "change_strategies", hv_ChangeStrategies);
  //
  //Serialization strategies used during training.
  hv_Indices = hv_GenParamName.TupleFind("serialize");
  if (0 != (HTuple(int((hv_Indices.TupleLength())==0)).TupleOr(int(hv_Indices==-1))))
  {
    //Set a default in case no value is provided.
    CreateDict(&hv_SerializationStrategy);
    SetDictTuple(hv_SerializationStrategy, "type", "best");
    SetDictTuple(hv_SerializationStrategy, "basename", "model_best");
    hv_SerializationStrategies = hv_SerializationStrategy;
  }
  else
  {
    //Set user provided values.
    hv_SerializationStrategies = HTuple(hv_GenParamValue[hv_Indices]);
  }
  SetDictTuple((*hv_TrainParam), "serialization_strategies", hv_SerializationStrategies);
  //
  //Get random seed or set a useful default value.
  if (0 != (int((hv_RandomSeed.TupleLength())>0)))
  {
    SetDictTuple((*hv_TrainParam), "seed_rand", hv_RandomSeed);
  }
  else
  {
    //If no random seed is given we will use system time as a default.
    CountSeconds(&hv_Seconds);
    hv_RandomSeed = hv_Seconds.TupleInt();
    SetDictTuple((*hv_TrainParam), "seed_rand", hv_RandomSeed);
  }
  //
  //** Display parameters: ***
  //
  //Create display parameter dictionary.
  get_genparam_single_value(hv_GenParamName, hv_GenParamValue, "display", &hv_SetDisplayParam);
  if (0 != (int(hv_SetDisplayParam!=HTuple())))
  {
    hv_DisplayParam = hv_SetDisplayParam;
  }
  else
  {
    CreateDict(&hv_DisplayParam);
  }
  //
  SetDictTuple(hv_DisplayParam, "enabled", hv_EnableDisplay);
  SetDictTuple((*hv_TrainParam), "display_param", hv_DisplayParam);
  //
  //** Generic internal defaults: ***
  //
  //Default update interval (in seconds) of TrainInfo calculation and text/plot updates
  //in case display is enabled.
  SetDictTuple((*hv_TrainParam), "update_interval_seconds", 2);
  //
  //Evaluation comparison keys. Note, that internally only those keys apply which
  //are really available. No error is thrown as long as a valid key is given.
  //Hence, we use the major defaults here for classification ('top1_error'),
  //for detection ('mean_ap'), and for segmentation ('mean_iou') if no valid key
  //is given.
  //We also set the BestScoreHighest value which describes which value should
  //achieved during the training of the chosen evaluation key.
  //For evaluation keys the score is maximized/minimized.
  hv_EvaluationComparisonKeys = HTuple();
  hv_BestScoreHighest = HTuple();
  //
  try
  {
    GetDictTuple(hv_EvaluationParam, "measures", &hv_EvaluationComparisonKeys);
  }
  // catch (Exception) 
  catch (HException &HDevExpDefaultException)
  {
    HDevExpDefaultException.ToHTuple(&hv_Exception);
  }
  //
  if (0 != (int(hv_EvaluationComparisonKeys==HTuple())))
  {
    if (0 != (int(hv_ModelType==HTuple("classification"))))
    {
      hv_EvaluationComparisonKeys = "top1_error";
      hv_BestScoreHighest = 0;
    }
    else if (0 != (int(hv_ModelType==HTuple("segmentation"))))
    {
      hv_EvaluationComparisonKeys = "mean_iou";
      hv_BestScoreHighest = 1;
    }
    else if (0 != (int(hv_ModelType==HTuple("detection"))))
    {
      hv_EvaluationComparisonKeys = "mean_ap";
      hv_BestScoreHighest = 1;
    }
  }
  else
  {
    hv_BestScoreHighest = 1;
    if (0 != (int(hv_ModelType==HTuple("classification"))))
    {
      if (0 != (int((HTuple(hv_EvaluationComparisonKeys[0]).TupleRegexpTest("top[0-9]*_error"))==1)))
      {
        hv_BestScoreHighest = 0;
      }
    }
  }
  //
  //If the evaluation metric is 'precision', 'recall', 'f_score', or
  //'soap' we always take the mean value.
  hv_ConvertToMean.Clear();
  hv_ConvertToMean[0] = "precision";
  hv_ConvertToMean[1] = "recall";
  hv_ConvertToMean[2] = "f_score";
  hv_ConvertToMean[3] = "soap";
  {
  HTuple end_val186 = (hv_ConvertToMean.TupleLength())-1;
  HTuple step_val186 = 1;
  for (hv_Index=0; hv_Index.Continue(end_val186, step_val186); hv_Index += step_val186)
  {
    hv_FoundIndices = (hv_EvaluationComparisonKeys.TupleEqualElem(HTuple(hv_ConvertToMean[hv_Index]))).TupleFind(1);
    if (0 != (int(hv_FoundIndices!=-1)))
    {
      hv_EvaluationComparisonKeys[hv_FoundIndices] = "mean_"+HTuple(hv_EvaluationComparisonKeys[hv_FoundIndices]);
      if (0 != (int(HTuple(hv_ConvertToMean[hv_Index])==HTuple("soap"))))
      {
        hv_EvaluationComparisonKeys[hv_FoundIndices] = HTuple(hv_EvaluationComparisonKeys[hv_FoundIndices])+"_tp";
      }
    }
  }
  }
  //
  SetDictTuple((*hv_TrainParam), "evaluation_comparison_keys", hv_EvaluationComparisonKeys);
  SetDictTuple((*hv_TrainParam), "evaluation_comparison_keys_best_highest", hv_BestScoreHighest);
  //
  //Number of samples used to average the loss during training. Note, this is used for display
  //and information calculation only and does not have an effect on training the model.
  SetDictTuple((*hv_TrainParam), "num_samples_mean_loss", 1000);
  //
  return;
}

// Chapter: Deep Learning / Evaluation
// Short Description: Generate a dictionary EvalParams, which contains default values for evaluation parameters. 
void create_evaluation_default_param (HTuple hv_EvaluationType, HTuple hv_ClassIDsModel, 
    HTuple *hv_EvalParams)
{

  // Local iconic variables

  // Local control variables
  HTuple  hv_AvailableEvaluationTypes, hv_EvaluationTypesStr;
  HTuple  hv_Indices, hv_EvaluateInstances, hv_Measures, hv_AreaRanges;
  HTuple  hv_AllocationBlockLength;

  //
  //This procedure generates a dictionary EvalParams,
  //which contains default values for evaluation parameters.
  //Depending on the evaluation type, the corresponding default parameters and values are set.
  //The class IDs that the model can predict must be given via ClassIDsModel.
  //
  //Check inputs.
  hv_AvailableEvaluationTypes.Clear();
  hv_AvailableEvaluationTypes[0] = "anomaly_detection";
  hv_AvailableEvaluationTypes[1] = "classification";
  hv_AvailableEvaluationTypes[2] = "detection";
  hv_AvailableEvaluationTypes[3] = "segmentation";
  TupleGenConst((2*(hv_AvailableEvaluationTypes.TupleLength()))-1, HTuple("','"), 
      &hv_EvaluationTypesStr);
  hv_EvaluationTypesStr[HTuple::TupleGenSequence(0,hv_EvaluationTypesStr.TupleLength(),2)] = hv_AvailableEvaluationTypes;
  hv_EvaluationTypesStr = hv_EvaluationTypesStr.TupleSum();
  TupleFind(hv_AvailableEvaluationTypes, hv_EvaluationType, &hv_Indices);
  if (0 != (HTuple(int(hv_Indices==-1)).TupleOr(int(hv_Indices==HTuple()))))
  {
    throw HException(((("Unknown evaluation_type: "+hv_EvaluationType)+". Choose one of ['")+hv_EvaluationTypesStr)+"']");
  }
  if (0 != (int((hv_ClassIDsModel.TupleLength())<1)))
  {
    throw HException("ClassIDsModel should have at least one entry");
  }
  //
  //Initialize EvalParams.
  CreateDict(&(*hv_EvalParams));
  SetDictTuple((*hv_EvalParams), "evaluation_type", hv_EvaluationType);
  //
  //Set the class IDs.
  SetDictTuple((*hv_EvalParams), "class_ids", hv_ClassIDsModel);
  SetDictTuple((*hv_EvalParams), "num_classes", hv_ClassIDsModel.TupleLength());
  //
  //Set specific parameters depending on the evaluation type.
  hv_EvaluateInstances = 0;
  if (0 != (int(hv_EvaluationType==HTuple("anomaly_detection"))))
  {
    //
    //Set default image level measures.
    hv_Measures = "anomaly_score_histogram";
  }
  else if (0 != (int(hv_EvaluationType==HTuple("classification"))))
  {
    //
    //Set default classification measures.
    hv_Measures = "top1_error";
    //
    //Per default all classes are used for evaluation.
    SetDictTuple((*hv_EvalParams), "class_ids_to_evaluate", "global");
  }
  else if (0 != (int(hv_EvaluationType==HTuple("detection"))))
  {
    //
    //Set default detection measures.
    hv_Measures = "mean_ap";
    //
    //Set detection-specific default values.
    hv_EvaluateInstances = 1;
    SetDictTuple((*hv_EvalParams), "instance_type", "rectangle1");
    //Generate ten IoU-thresholds from 0.5 to 0.95 in steps of 0.05.
    SetDictTuple((*hv_EvalParams), "iou_threshold", HTuple::TupleGenSequence(0.5,0.96,0.05));
    //Set maximal number of detections to -1, i.e. all results per image will be evaluated.
    SetDictTuple((*hv_EvalParams), "max_num_detections", -1);
    //Set default area range named 'all', thus areas from 0 to a value larger than all likely occurring values.
    CreateDict(&hv_AreaRanges);
    SetDictTuple(hv_AreaRanges, "name", "all");
    SetDictTuple(hv_AreaRanges, "min", 0);
    SetDictTuple(hv_AreaRanges, "max", 2e8);
    SetDictTuple((*hv_EvalParams), "area_ranges", hv_AreaRanges);
    //Some tuples are changing their length during the evaluation. As this slows down the
    //evaluation process they are allocated in blocks of AllocationBlockLength.
    hv_AllocationBlockLength = 200;
    SetDictTuple((*hv_EvalParams), "allocation_block_length", hv_AllocationBlockLength);
    //Detailed evaluation is not switched on per default, as it slows down the evaluation-process.
    SetDictTuple((*hv_EvalParams), "detailed_evaluation", 0);
    //Interpolate the precision-recall curves per default.
    SetDictTuple((*hv_EvalParams), "interpolate_pr_curves", 1);
  }
  else if (0 != (int(hv_EvaluationType==HTuple("segmentation"))))
  {
    //
    //Set default pixel measures.
    hv_Measures.Clear();
    hv_Measures[0] = "pixel_accuracy";
    hv_Measures[1] = "mean_accuracy";
    hv_Measures[2] = "mean_iou";
    //
    //Per default there are no ignored classes.
    SetDictTuple((*hv_EvalParams), "ignore_class_ids", HTuple());
  }
  //
  SetDictTuple((*hv_EvalParams), "evaluate_instances", hv_EvaluateInstances);
  SetDictTuple((*hv_EvalParams), "measures", hv_Measures);
  //
  return;
}

// Chapter: Graphics / Window
// Short Description: Close all window handles contained in a dictionary. 
void dev_close_window_dict (HTuple hv_WindowHandleDict)
{

  // Local iconic variables

  // Local control variables
  HTuple  hv_WindowHandleKeys, hv_Index, hv_WindowHandles;
  HTuple  hv_Exception, hv_RemovedWindowIndices, hv_WindowHandleIndex;

  //
  //This procedure closes all window handles
  //that are contained in the dictionary WindowHandleDict.
  //
  GetDictParam(hv_WindowHandleDict, "keys", HTuple(), &hv_WindowHandleKeys);
  {
  HTuple end_val5 = (hv_WindowHandleKeys.TupleLength())-1;
  HTuple step_val5 = 1;
  for (hv_Index=0; hv_Index.Continue(end_val5, step_val5); hv_Index += step_val5)
  {
    try
    {
      GetDictTuple(hv_WindowHandleDict, HTuple(hv_WindowHandleKeys[hv_Index]), &hv_WindowHandles);
    }
    // catch (Exception) 
    catch (HException &HDevExpDefaultException)
    {
      HDevExpDefaultException.ToHTuple(&hv_Exception);
      continue;
    }
    hv_RemovedWindowIndices = HTuple();
    {
    HTuple end_val12 = (hv_WindowHandles.TupleLength())-1;
    HTuple step_val12 = 1;
    for (hv_WindowHandleIndex=0; hv_WindowHandleIndex.Continue(end_val12, step_val12); hv_WindowHandleIndex += step_val12)
    {
      //Not every entry has to be a window handle, therefore use try-catch.
      try
      {
        //Call set_window_param to check if the handle is a window handle.
        SetWindowParam(HTuple(hv_WindowHandles[hv_WindowHandleIndex]), "flush", "true");
        HDevWindowStack::SetActive(HTuple(hv_WindowHandles[hv_WindowHandleIndex]));
        if (HDevWindowStack::IsOpen())
          CloseWindow(HDevWindowStack::Pop());
        hv_RemovedWindowIndices = hv_RemovedWindowIndices.TupleConcat(hv_WindowHandleIndex);
      }
      // catch (Exception) 
      catch (HException &HDevExpDefaultException)
      {
        HDevExpDefaultException.ToHTuple(&hv_Exception);
      }
    }
    }
    TupleRemove(hv_WindowHandles, hv_RemovedWindowIndices, &hv_WindowHandles);
    //If some entries remained, set reduced tuple. Otherwise, remove whole key entry.
    if (0 != (int((hv_WindowHandles.TupleLength())>0)))
    {
      SetDictTuple(hv_WindowHandleDict, HTuple(hv_WindowHandleKeys[hv_Index]), hv_WindowHandles);
    }
    else
    {
      RemoveDictKey(hv_WindowHandleDict, HTuple(hv_WindowHandleKeys[hv_Index]));
    }
  }
  }
  //
  return;
}

// Chapter: Deep Learning / Anomaly Detection
// Short Description: Display evaluation results of an evaluated DL model of type 'anomaly_detection'. 
void dev_display_anomaly_detection_evaluation (HTuple hv_EvaluationResult, HTuple hv_EvalParam, 
    HTuple hv_GenParam, HTuple hv_WindowHandleDict)
{

  // Local iconic variables

  // Local control variables
  HTuple  hv_BackgroundColor, hv_OKColor, hv_OKColorPlot;
  HTuple  hv_NOKColor, hv_NOKColorPlot, hv_IntermediateColor;
  HTuple  hv_PredefinedColors, hv_EvaluationThresholds, hv_AdditionalThresholds;
  HTuple  hv_EvaluationThresholdIndex, hv_RowColNameColor;
  HTuple  hv_DisplayColumnWidth, hv_DisplayMatrixColor, hv_Font;
  HTuple  hv_FontSize, hv_HistogramLegendVerticalOffset, hv_NumXTicks;
  HTuple  hv_HistogramMin, hv_HistogramMax, hv_DisplayThresholds;
  HTuple  hv_DisplayScoreHistogram, hv_DisplayScoreLegend;
  HTuple  hv_DisplayConfusionAbsolute, hv_DisplayConfusionRelative;
  HTuple  hv_DisplayPieChartPrecision, hv_DisplayPieChartRecall;
  HTuple  hv_WindowRow, hv_WindowColumn, hv_WindowMaxColumn;
  HTuple  hv_WindowColumnSeparator, hv_GenParamKeys, hv_GenParamIndex;
  HTuple  hv_ThresholdValues, hv_DisplayMode, hv_DisplayAll;
  HTuple  hv_MinValue, hv_MaxValue, hv_EvaluationType, hv_EvalThresholdsExist;
  HTuple  hv_DiffEpsilon, hv_RemoveIndices, hv_ThresholdIdx;
  HTuple  hv_MinThresholdDiff, hv_AllThresholds, hv_ThresholdColors;
  HTuple  hv_UseSpecificLegend, hv_RandomColors, hv_HistoOKXValues;
  HTuple  hv_HistoOKYValues, hv_HistoNOKXValues, hv_HistoNOKYValues;
  HTuple  hv_AnomalyScoreHistogram, hv_Exception, hv_MinScaling;
  HTuple  hv_MaxScaling, hv_AnomalyScores, hv_MinScoreScaled;
  HTuple  hv_MinScoreRange, hv_MaxScoreScaled, hv_MaxScoreRange;
  HTuple  hv_MaxScore, hv_MinScore, hv_TicksX, hv_FormatX;
  HTuple  hv_OKMinY, hv_OKMinIndex, hv_NOKMaxY, hv_NOKMaxIndex;
  HTuple  hv_MaxY, hv_PlotOKXValues, hv_PlotOKYValues, hv_OKIndex;
  HTuple  hv_PlotNOKXValues, hv_PlotNOKYValues, hv_NOKIndex;
  HTuple  hv_WindowScoreHistogramExists, hv_WindowHandleScoreHistogram;
  HTuple  hv_FlushStateScoreHistogram, hv_Index, hv_Color;
  HTuple  hv_WindowHandle, hv__, hv_Width, hv_Height, hv_WindowScoreLegendExists;
  HTuple  hv_WindowHandleScoreLegend, hv_FlushStateScoreLegend;
  HTuple  hv_Offset, hv_RunningIndex, hv_ThresholdCaptions;
  HTuple  hv_WindowExists, hv_WindowHandlePrecision, hv_WindowHandleRecall;
  HTuple  hv_RowColumnNames, hv_ValueColors, hv_DiagonalColor;
  HTuple  hv_DiagonalIndices, hv_DisplayMatrices, hv_Strings;
  HTuple  hv_CapitalStrings, hv_TitelAdditions, hv_IndexMatrixToDisplay;
  HTuple  hv_ConfusionMatrixDictionaries, hv_ConfusionMatrixDictionary;
  HTuple  hv_ConfusionMatrix, hv_ThresholdValue, hv_ConfusionMatrixDisplay;
  HTuple  hv_ValuesRelative, hv_MatrixMaxID, hv_NumberFormat;
  HTuple  hv_WindowHandleConfusion;

  //
  //This procedure visualizes the evaluation results for a model of type anomaly detection.
  //
  //Set the default values.
  hv_BackgroundColor = "light gray";
  hv_OKColor = "#1354ad";
  hv_OKColorPlot = "#3e94d8";
  hv_NOKColor = "#ff3410";
  hv_NOKColorPlot = "#ff942e";
  hv_IntermediateColor = "#444444";
  hv_PredefinedColors.Clear();
  hv_PredefinedColors.Append(hv_OKColor);
  hv_PredefinedColors.Append(hv_NOKColor);
  hv_PredefinedColors.Append(hv_IntermediateColor);
  //
  hv_EvaluationThresholds = HTuple();
  hv_AdditionalThresholds = HTuple();
  hv_EvaluationThresholdIndex = -1;
  //
  hv_RowColNameColor = "light gray";
  hv_DisplayColumnWidth = "equal";
  hv_DisplayMatrixColor = 1;
  hv_Font = "mono";
  hv_FontSize = 14;
  //
  hv_HistogramLegendVerticalOffset = 50;
  hv_NumXTicks = 10;
  hv_HistogramMin = 0;
  hv_HistogramMax = -1;
  //
  hv_DisplayThresholds = 0;
  hv_DisplayScoreHistogram = 1;
  hv_DisplayScoreLegend = 1;
  hv_DisplayConfusionAbsolute = 0;
  hv_DisplayConfusionRelative = 0;
  hv_DisplayPieChartPrecision = 0;
  hv_DisplayPieChartRecall = 0;
  //
  //Parameters used for the window placements:
  hv_WindowRow = 0;
  hv_WindowColumn = 0;
  hv_WindowMaxColumn = 0;
  hv_WindowColumnSeparator = 8;
  //
  //Handle the generic parameters.
  if (0 != (int(hv_GenParam!=HTuple())))
  {
    GetDictParam(hv_GenParam, "keys", HTuple(), &hv_GenParamKeys);
    {
    HTuple end_val44 = (hv_GenParamKeys.TupleLength())-1;
    HTuple step_val44 = 1;
    for (hv_GenParamIndex=0; hv_GenParamIndex.Continue(end_val44, step_val44); hv_GenParamIndex += step_val44)
    {
      if (0 != (int(HTuple(hv_GenParamKeys[hv_GenParamIndex])==HTuple("estimated_thresholds"))))
      {
        GetDictTuple(hv_GenParam, "estimated_thresholds", &hv_ThresholdValues);
        if (0 != (int((((hv_ThresholdValues.TupleIsIntElem())+(hv_ThresholdValues.TupleIsRealElem())).TupleFind(0))!=-1)))
        {
          throw HException("GenParam 'estimated_thresholds' must be numeric values");
        }
        if (0 != (int((hv_ThresholdValues.TupleLength())>3)))
        {
          throw HException("GenParam 'estimated_thresholds' must be of length 3 or less");
        }
        //
        hv_DisplayThresholds = 1;
        hv_AdditionalThresholds = hv_ThresholdValues;
      }
      else if (0 != (int(HTuple(hv_GenParamKeys[hv_GenParamIndex])==HTuple("display_mode"))))
      {
        hv_DisplayScoreHistogram = 0;
        hv_DisplayScoreLegend = 0;
        GetDictTuple(hv_GenParam, "display_mode", &hv_DisplayMode);
        hv_DisplayAll = HTuple(int((hv_DisplayMode.TupleFind("all"))!=-1)).TupleAnd(int(hv_DisplayMode!=HTuple()));
        if (0 != hv_DisplayAll)
        {
          hv_DisplayScoreHistogram = 1;
          hv_DisplayScoreLegend = 1;
          hv_DisplayConfusionAbsolute = 1;
          hv_DisplayConfusionRelative = 1;
          hv_DisplayPieChartPrecision = 1;
          hv_DisplayPieChartRecall = 1;
        }
        else
        {
          hv_DisplayScoreHistogram = HTuple(int((hv_DisplayMode.TupleFind("score_histogram"))!=-1)).TupleAnd(int(hv_DisplayMode!=HTuple()));
          hv_DisplayScoreLegend = HTuple(int((hv_DisplayMode.TupleFind("score_legend"))!=-1)).TupleAnd(int(hv_DisplayMode!=HTuple()));
          hv_DisplayConfusionAbsolute = HTuple(int((hv_DisplayMode.TupleFind("absolute_confusion_matrix"))!=-1)).TupleAnd(int(hv_DisplayMode!=HTuple()));
          hv_DisplayConfusionRelative = HTuple(int((hv_DisplayMode.TupleFind("relative_confusion_matrix"))!=-1)).TupleAnd(int(hv_DisplayMode!=HTuple()));
          hv_DisplayPieChartPrecision = HTuple(int((hv_DisplayMode.TupleFind("pie_charts_precision"))!=-1)).TupleAnd(int(hv_DisplayMode!=HTuple()));
          hv_DisplayPieChartRecall = HTuple(int((hv_DisplayMode.TupleFind("pie_charts_recall"))!=-1)).TupleAnd(int(hv_DisplayMode!=HTuple()));
        }
      }
      else if (0 != (int(HTuple(hv_GenParamKeys[hv_GenParamIndex])==HTuple("classification_threshold_index"))))
      {
        GetDictTuple(hv_GenParam, "classification_threshold_index", &hv_EvaluationThresholdIndex);
        if (0 != (HTuple((hv_EvaluationThresholdIndex.TupleIsInt()).TupleNot()).TupleOr(int((hv_EvaluationThresholdIndex.TupleLength())!=1))))
        {
          throw HException("GenParam 'classification_threshold_index' must be one integer value");
        }
        if (0 != (int(hv_EvaluationThresholdIndex<0)))
        {
          throw HException("GenParam 'classification_threshold_index' must be greater or equal 0");
        }
      }
      else if (0 != (int(HTuple(hv_GenParamKeys[hv_GenParamIndex])==HTuple("histogram_min"))))
      {
        GetDictTuple(hv_GenParam, "histogram_min", &hv_MinValue);
        if (0 != (int((hv_MinValue.TupleLength())!=1)))
        {
          throw HException("GenParam 'histogram_min' must be one value");
        }
        if (0 != (HTuple((hv_MinValue.TupleIsInt()).TupleOr(hv_MinValue.TupleIsReal())).TupleNot()))
        {
          throw HException("GenParam 'histogram_min' must be a numeric value");
        }
        if (0 != (HTuple(int(hv_MinValue!=-1)).TupleAnd(int(hv_MinValue<0))))
        {
          throw HException("GenParam 'histogram_min' must be -1 or a value greater or equal 0");
        }
        hv_HistogramMin = hv_MinValue;
      }
      else if (0 != (int(HTuple(hv_GenParamKeys[hv_GenParamIndex])==HTuple("histogram_max"))))
      {
        GetDictTuple(hv_GenParam, "histogram_max", &hv_MaxValue);
        if (0 != (int((hv_MaxValue.TupleLength())!=1)))
        {
          throw HException("GenParam 'histogram_max' must be one value");
        }
        if (0 != (HTuple((hv_MaxValue.TupleIsInt()).TupleOr(hv_MaxValue.TupleIsReal())).TupleNot()))
        {
          throw HException("GenParam 'histogram_max' must be a numeric value");
        }
        if (0 != (HTuple(int(hv_MaxValue!=-1)).TupleAnd(int(hv_MaxValue<=0))))
        {
          throw HException("GenParam 'histogram_max' must be -1 or a value greater 0");
        }
        hv_HistogramMax = hv_MaxValue;
      }
      else if (0 != (int(HTuple(hv_GenParamKeys[hv_GenParamIndex])==HTuple("row_col_names_color"))))
      {
        GetDictTuple(hv_GenParam, "row_col_names_color", &hv_RowColNameColor);
      }
      else if (0 != (int(HTuple(hv_GenParamKeys[hv_GenParamIndex])==HTuple("display_column_width"))))
      {
        GetDictTuple(hv_GenParam, "display_column_width", &hv_DisplayColumnWidth);
      }
      else if (0 != (int(HTuple(hv_GenParamKeys[hv_GenParamIndex])==HTuple("display_matrix_color"))))
      {
        GetDictTuple(hv_GenParam, "display_matrix_color", &hv_DisplayMatrixColor);
      }
      else if (0 != (int(HTuple(hv_GenParamKeys[hv_GenParamIndex])==HTuple("font"))))
      {
        GetDictTuple(hv_GenParam, "font", &hv_Font);
      }
      else if (0 != (int(HTuple(hv_GenParamKeys[hv_GenParamIndex])==HTuple("font_size"))))
      {
        GetDictTuple(hv_GenParam, "font_size", &hv_FontSize);
      }
      else
      {
        throw HException(("Invalid GenParam key '"+HTuple(hv_GenParamKeys[hv_GenParamIndex]))+"'");
      }
    }
    }
  }
  //
  //Check that evaluation type is correct.
  GetDictTuple(hv_EvalParam, "evaluation_type", &hv_EvaluationType);
  if (0 != (int(hv_EvaluationType!=HTuple("anomaly_detection"))))
  {
    throw HException("Wrong evaluation type for display");
  }
  //
  //Get thresholds used for evaluation.
  GetDictParam(hv_EvalParam, "key_exists", "anomaly_classification_thresholds", &hv_EvalThresholdsExist);
  if (0 != hv_EvalThresholdsExist)
  {
    GetDictTuple(hv_EvalParam, "anomaly_classification_thresholds", &hv_EvaluationThresholds);
    if (0 != (int((hv_EvaluationThresholds.TupleLength())>0)))
    {
      hv_DisplayThresholds = 1;
      //If no threshold is set, use the last evaluation index as default.
      if (0 != (int(hv_EvaluationThresholdIndex==-1)))
      {
        hv_EvaluationThresholdIndex = (hv_EvaluationThresholds.TupleLength())-1;
      }
    }
    //Check that the set threshold index is valid.
    if (0 != (int(hv_EvaluationThresholdIndex>((hv_EvaluationThresholds.TupleLength())-1))))
    {
      throw HException("No evaluation available for 'classification_threshold_index': "+hv_EvaluationThresholdIndex);
    }
    //Remove extra thresholds in case they are already included in the evaluation.
    if (0 != (HTuple(int((hv_AdditionalThresholds.TupleLength())>0)).TupleAnd(int((hv_EvaluationThresholds.TupleLength())>0))))
    {
      hv_DiffEpsilon = 1e-7;
      hv_RemoveIndices = HTuple();
      {
      HTuple end_val149 = (hv_AdditionalThresholds.TupleLength())-1;
      HTuple step_val149 = 1;
      for (hv_ThresholdIdx=0; hv_ThresholdIdx.Continue(end_val149, step_val149); hv_ThresholdIdx += step_val149)
      {
        hv_MinThresholdDiff = ((hv_EvaluationThresholds-HTuple(hv_AdditionalThresholds[hv_ThresholdIdx])).TupleAbs()).TupleMin();
        if (0 != (int(hv_MinThresholdDiff<hv_DiffEpsilon)))
        {
          hv_RemoveIndices = hv_RemoveIndices.TupleConcat(hv_ThresholdIdx);
        }
      }
      }
      hv_AdditionalThresholds = hv_AdditionalThresholds.TupleRemove(hv_RemoveIndices);
    }
  }
  //
  hv_AllThresholds.Clear();
  hv_AllThresholds.Append(hv_EvaluationThresholds);
  hv_AllThresholds.Append(hv_AdditionalThresholds);
  //
  //Set colors for the thresholds.
  hv_ThresholdColors = HTuple();
  hv_UseSpecificLegend = 0;
  //In case the overall number of thresholds is exactly 3 (either passed as EvalParam
  //or GenParam), we assume that they are the ones calculated with
  //compute_dl_anomaly_thresholds and use the specific colors and legend.
  if (0 != (HTuple(int((hv_AllThresholds.TupleLength())==3)).TupleAnd(HTuple(int((hv_EvaluationThresholds.TupleLength())==0)).TupleOr(int((hv_AdditionalThresholds.TupleLength())==0)))))
  {
    hv_UseSpecificLegend = 1;
    hv_ThresholdColors = hv_PredefinedColors;
  }
  else if (0 != (int((hv_AllThresholds.TupleLength())!=0)))
  {
    //Get random colors to display thresholds.
    get_distinct_colors(hv_AllThresholds.TupleLength(), 0, 175, 250, &hv_RandomColors);
    if (0 != (int((hv_EvaluationThresholds.TupleLength())==3)))
    {
      hv_UseSpecificLegend = 1;
      hv_ThresholdColors.Clear();
      hv_ThresholdColors.Append(hv_PredefinedColors);
      hv_ThresholdColors.Append(hv_RandomColors);
    }
    else
    {
      hv_ThresholdColors = hv_RandomColors;
    }
  }
  //
  if (0 != hv_DisplayScoreHistogram)
  {
    hv_HistoOKXValues = HTuple();
    hv_HistoOKYValues = HTuple();
    hv_HistoNOKXValues = HTuple();
    hv_HistoNOKYValues = HTuple();
    //
    //Check if the anomaly score histogram is given.
    try
    {
      GetDictTuple(hv_EvaluationResult, "anomaly_score_histogram", &hv_AnomalyScoreHistogram);
    }
    // catch (Exception) 
    catch (HException &HDevExpDefaultException)
    {
      HDevExpDefaultException.ToHTuple(&hv_Exception);
      throw HException("EvaluationResult must contain the anomaly score histogram.");
    }
    //Check if OK score x and y values exist.
    try
    {
      GetDictTuple(hv_AnomalyScoreHistogram, "ok_x", &hv_HistoOKXValues);
      GetDictTuple(hv_AnomalyScoreHistogram, "ok_y", &hv_HistoOKYValues);
    }
    // catch (Exception) 
    catch (HException &HDevExpDefaultException)
    {
      HDevExpDefaultException.ToHTuple(&hv_Exception);
      throw HException("AnomalyScoreHistogram must contain values for 'ok_x' and 'ok_y'.");
    }
    //Check if NOK score x and y values exist.
    try
    {
      GetDictTuple(hv_AnomalyScoreHistogram, "nok_x", &hv_HistoNOKXValues);
      GetDictTuple(hv_AnomalyScoreHistogram, "nok_y", &hv_HistoNOKYValues);
    }
    // catch (Exception) 
    catch (HException &HDevExpDefaultException)
    {
      HDevExpDefaultException.ToHTuple(&hv_Exception);
      throw HException("AnomalyScoreHistogram must contain values for 'nok_x' and 'nok_y'.");
    }
    //
    //Validate the histogram content.
    if (0 != (int((hv_HistoOKXValues.TupleLength())!=(hv_HistoOKYValues.TupleLength()))))
    {
      throw HException("Number of 'ok_x' values does not match number of 'ok_y' values.");
    }
    if (0 != (int((hv_HistoNOKXValues.TupleLength())!=(hv_HistoNOKYValues.TupleLength()))))
    {
      throw HException("Number of 'nok_x' values does not match number of 'nok_y' values.");
    }
    if (0 != (HTuple(int((hv_HistoOKXValues.TupleLength())==0)).TupleAnd(int((hv_HistoNOKXValues.TupleLength())==0))))
    {
      throw HException("No data for display");
    }
    //
    //In case the min or max histogram value is not specified, estimate it by
    //using min/max of the anomaly scores.
    hv_MinScaling = 0.7;
    hv_MaxScaling = 1.2;
    hv_AnomalyScores.Clear();
    hv_AnomalyScores.Append(hv_HistoOKXValues);
    hv_AnomalyScores.Append(hv_HistoNOKXValues);
    if (0 != (int(hv_HistogramMin==-1)))
    {
      hv_MinScoreScaled = hv_MinScaling*(hv_AnomalyScores.TupleMin());
      hv_MinScoreRange = (hv_AnomalyScores.TupleMin())-(0.15*((hv_AnomalyScores.TupleMax())-(hv_AnomalyScores.TupleMin())));
      hv_HistogramMin = ((HTuple(0.0).TupleConcat(hv_MinScoreScaled)).TupleConcat(hv_MinScoreRange)).TupleMax();
    }
    if (0 != (int(hv_HistogramMax==-1)))
    {
      hv_MaxScoreScaled = hv_MaxScaling*(hv_AnomalyScores.TupleMax());
      hv_MaxScoreRange = (hv_AnomalyScores.TupleMax())+(0.15*((hv_AnomalyScores.TupleMax())-(hv_AnomalyScores.TupleMin())));
      hv_HistogramMax = (hv_MaxScoreScaled.TupleConcat(hv_MaxScoreRange)).TupleMin();
    }
    //Check min and max values.
    if (0 != (int(hv_HistogramMin>=hv_HistogramMax)))
    {
      throw HException(((("Maximum histogram value needs to be greater than the minimum value. Current values: "+hv_HistogramMin)+HTuple(" (min), "))+hv_HistogramMax)+" (max).");
    }
    hv_MaxScore = hv_AnomalyScores.TupleMax();
    if (0 != (int(hv_HistogramMin>=hv_MaxScore)))
    {
      throw HException(((("Minimum histogram value ("+hv_HistogramMin)+") is larger than the highest score (")+hv_MaxScore)+").");
    }
    hv_MinScore = hv_AnomalyScores.TupleMin();
    if (0 != (int(hv_HistogramMax<=hv_MinScore)))
    {
      throw HException(((("Maximum histogram value ("+hv_HistogramMax)+") is smaller than the lowest score (")+hv_MinScore)+").");
    }
    //
    //Append HistogramMin/HistogramMax to histogram values if necessary.
    if (0 != (int((hv_HistoOKXValues.TupleLength())!=0)))
    {
      if (0 != (int(hv_HistogramMin<(hv_HistoOKXValues.TupleMin()))))
      {
        hv_HistoOKXValues = hv_HistogramMin.TupleConcat(hv_HistoOKXValues);
        hv_HistoOKYValues = HTuple(1.0).TupleConcat(hv_HistoOKYValues);
      }
    }
    if (0 != (int((hv_HistoNOKXValues.TupleLength())!=0)))
    {
      if (0 != (int(hv_HistogramMax>(hv_HistoNOKXValues.TupleMax()))))
      {
        hv_HistoNOKXValues = hv_HistoNOKXValues.TupleConcat(hv_HistogramMax);
        hv_HistoNOKYValues = hv_HistoNOKYValues.TupleConcat(1.0);
      }
    }
    //
    //Specifiy other plot parameters.
    hv_TicksX = ((hv_HistogramMax-hv_HistogramMin)/hv_NumXTicks).TupleReal();
    if (0 != (int(hv_HistogramMax>=10)))
    {
      hv_FormatX = ".1f";
    }
    else
    {
      hv_FormatX = ".2f";
    }
    //Find maximum y value to plot.
    hv_OKMinY = 0;
    hv_OKMinIndex = (hv_HistoOKXValues.TupleGreaterEqualElem(hv_HistogramMin)).TupleFindFirst(1);
    if (0 != (int(hv_OKMinIndex!=-1)))
    {
      hv_OKMinY = HTuple(hv_HistoOKYValues[hv_OKMinIndex]);
    }
    hv_NOKMaxY = 0;
    hv_NOKMaxIndex = (hv_HistoNOKXValues.TupleLessEqualElem(hv_HistogramMax)).TupleFindLast(1);
    if (0 != (int(hv_NOKMaxIndex!=-1)))
    {
      hv_NOKMaxY = HTuple(hv_HistoNOKYValues[hv_NOKMaxIndex]);
    }
    hv_MaxY = (hv_OKMinY.TupleConcat(hv_NOKMaxY)).TupleMax();
    //
    //Extend the histogram values so that they will be plotted as steps with the 'filled' style.
    hv_PlotOKXValues = HTuple();
    hv_PlotOKYValues = HTuple();
    if (0 != (int((hv_HistoOKXValues.TupleLength())!=0)))
    {
      {
      HTuple end_val285 = (hv_HistoOKXValues.TupleLength())-2;
      HTuple step_val285 = 1;
      for (hv_OKIndex=0; hv_OKIndex.Continue(end_val285, step_val285); hv_OKIndex += step_val285)
      {
        hv_PlotOKXValues = (hv_PlotOKXValues.TupleConcat(HTuple(hv_HistoOKXValues[hv_OKIndex]))).TupleConcat(HTuple(hv_HistoOKXValues[hv_OKIndex]));
        hv_PlotOKYValues = (hv_PlotOKYValues.TupleConcat(HTuple(hv_HistoOKYValues[hv_OKIndex]))).TupleConcat(HTuple(hv_HistoOKYValues[hv_OKIndex+1]));
      }
      }
      hv_PlotOKXValues = (hv_PlotOKXValues.TupleConcat(HTuple(hv_HistoOKXValues[hv_OKIndex]))).TupleConcat(HTuple(hv_HistoOKXValues[hv_OKIndex]));
      hv_PlotOKYValues = (hv_PlotOKYValues.TupleConcat(HTuple(hv_HistoOKYValues[hv_OKIndex]))).TupleConcat(0.0);
    }
    //
    hv_PlotNOKXValues = HTuple();
    hv_PlotNOKYValues = HTuple();
    if (0 != (int((hv_HistoNOKXValues.TupleLength())!=0)))
    {
      hv_PlotNOKXValues.Clear();
      hv_PlotNOKXValues.Append(HTuple(hv_HistoNOKXValues[0]));
      hv_PlotNOKXValues.Append(HTuple(hv_HistoNOKXValues[0]));
      hv_PlotNOKYValues.Clear();
      hv_PlotNOKYValues[0] = 0.0;
      hv_PlotNOKYValues.Append(HTuple(hv_HistoNOKYValues[0]));
      {
      HTuple end_val298 = (hv_HistoNOKXValues.TupleLength())-1;
      HTuple step_val298 = 1;
      for (hv_NOKIndex=1; hv_NOKIndex.Continue(end_val298, step_val298); hv_NOKIndex += step_val298)
      {
        hv_PlotNOKXValues = (hv_PlotNOKXValues.TupleConcat(HTuple(hv_HistoNOKXValues[hv_NOKIndex]))).TupleConcat(HTuple(hv_HistoNOKXValues[hv_NOKIndex]));
        hv_PlotNOKYValues = (hv_PlotNOKYValues.TupleConcat(HTuple(hv_HistoNOKYValues[hv_NOKIndex-1]))).TupleConcat(HTuple(hv_HistoNOKYValues[hv_NOKIndex]));
      }
      }
    }
    //
    //Create drawing window for histogram.
    GetDictParam(hv_WindowHandleDict, "key_exists", "window_score_histogram", &hv_WindowScoreHistogramExists);
    if (0 != (hv_WindowScoreHistogramExists.TupleNot()))
    {
      SetWindowAttr("background_color",hv_BackgroundColor);
      OpenWindow(0,0,512,512,0,"visible","",&hv_WindowHandleScoreHistogram);
      HDevWindowStack::Push(hv_WindowHandleScoreHistogram);
      SetDictTuple(hv_WindowHandleDict, "window_score_histogram", hv_WindowHandleScoreHistogram);
    }
    else
    {
      GetDictTuple(hv_WindowHandleDict, "window_score_histogram", &hv_WindowHandleScoreHistogram);
    }
    GetWindowParam(hv_WindowHandleScoreHistogram, "flush", &hv_FlushStateScoreHistogram);
    SetWindowParam(hv_WindowHandleScoreHistogram, "flush", "false");
    //
    //Adapt fonts and background colors in the histogram.
    set_display_font(hv_WindowHandleScoreHistogram, 14, hv_Font, "true", "false");
    SetWindowParam(hv_WindowHandleScoreHistogram, "background_color", hv_BackgroundColor);
    //
    plot_tuple(hv_WindowHandleScoreHistogram, hv_PlotOKXValues, hv_PlotOKYValues, 
        "Score", "Fraction of wrongly classified images", hv_OKColorPlot+"80", ((((((((((HTuple("start_x").Append("end_x")).Append("start_y")).Append("end_y")).Append("ticks_x")).Append("ticks_y")).Append("axes_color")).Append("format_x")).Append("format_y")).Append("style")).Append("clip")), 
        ((((((hv_HistogramMin.TupleConcat(hv_HistogramMax)).TupleConcat(0.0)).TupleConcat(hv_MaxY+0.05)).TupleConcat(hv_TicksX)).TupleConcat((HTuple(0.1).Append("black")))).TupleConcat(hv_FormatX)).TupleConcat(((HTuple(".1f").Append("filled")).Append("yes"))));
    plot_tuple(hv_WindowHandleScoreHistogram, hv_PlotNOKXValues, hv_PlotNOKYValues, 
        HTuple(), HTuple(), hv_NOKColorPlot+"80", ((((((((HTuple("start_x").Append("end_x")).Append("start_y")).Append("end_y")).Append("ticks_x")).Append("ticks_y")).Append("axes_color")).Append("style")).Append("clip")), 
        (((hv_HistogramMin.TupleConcat(hv_HistogramMax)).TupleConcat(0.0)).TupleConcat(hv_MaxY+0.05)).TupleConcat(((((HTuple("none").Append("none")).Append("none")).Append("filled")).Append("yes"))));
    //
    //Plot thresholds from evaluation or generic parameters if given.
    if (0 != hv_DisplayThresholds)
    {
      {
      HTuple end_val324 = (hv_AllThresholds.TupleLength())-1;
      HTuple step_val324 = 1;
      for (hv_Index=0; hv_Index.Continue(end_val324, step_val324); hv_Index += step_val324)
      {
        hv_Color = HTuple(hv_ThresholdColors[hv_Index]);
        plot_tuple(hv_WindowHandleScoreHistogram, HTuple(hv_AllThresholds[hv_Index]).TupleConcat(HTuple(hv_AllThresholds[hv_Index])), 
            HTuple(0.0).TupleConcat(hv_MaxY), HTuple(), HTuple(), hv_Color, (((((((HTuple("start_x").Append("end_x")).Append("start_y")).Append("end_y")).Append("ticks_x")).Append("ticks_y")).Append("axes_color")).Append("clip")), 
            (((hv_HistogramMin.TupleConcat(hv_HistogramMax)).TupleConcat(0.0)).TupleConcat(hv_MaxY+0.05)).TupleConcat((((HTuple("none").Append("none")).Append("none")).Append("yes"))));
      }
      }
    }
    //
    //Recalculate Window Coordinates for the following plots.
    GetDictTuple(hv_WindowHandleDict, "window_score_histogram", &hv_WindowHandle);
    GetWindowExtents(hv_WindowHandle, &hv__, &hv__, &hv_Width, &hv_Height);
    //Next plots will be shown on the right of the histograms.
    hv_WindowMaxColumn = hv_WindowMaxColumn.TupleMax2(hv_Width);
    hv_WindowRow = 0;
    hv_WindowColumn = (hv_WindowColumn+hv_WindowMaxColumn)+hv_WindowColumnSeparator;
    hv_WindowMaxColumn = 0;
    //
    FlushBuffer(hv_WindowHandleScoreHistogram);
    SetWindowParam(hv_WindowHandleScoreHistogram, "flush", hv_FlushStateScoreHistogram);
  }
  //
  //Plot histogram legend.
  if (0 != hv_DisplayScoreLegend)
  {
    //Create drawing window for legend.
    GetDictParam(hv_WindowHandleDict, "key_exists", "window_score_legend", &hv_WindowScoreLegendExists);
    if (0 != (hv_WindowScoreLegendExists.TupleNot()))
    {
      SetWindowAttr("background_color",hv_BackgroundColor);
      OpenWindow(hv_WindowRow,hv_WindowColumn,320,512,0,"visible","",&hv_WindowHandleScoreLegend);
      HDevWindowStack::Push(hv_WindowHandleScoreLegend);
      SetDictTuple(hv_WindowHandleDict, "window_score_legend", hv_WindowHandleScoreLegend);
    }
    else
    {
      GetDictTuple(hv_WindowHandleDict, "window_score_legend", &hv_WindowHandleScoreLegend);
    }
    //Adapt fonts and background colors in the histogram.
    HDevWindowStack::SetActive(hv_WindowHandleScoreLegend);
    GetWindowParam(hv_WindowHandleScoreLegend, "flush", &hv_FlushStateScoreLegend);
    SetWindowParam(hv_WindowHandleScoreLegend, "flush", "false");
    //
    HDevWindowStack::SetActive(hv_WindowHandleScoreLegend);
    SetWindowParam(hv_WindowHandleScoreLegend, "background_color", hv_BackgroundColor);
    set_display_font(hv_WindowHandleScoreLegend, hv_FontSize, hv_Font, "true", "false");
    if (HDevWindowStack::IsOpen())
      DispText(HDevWindowStack::GetActive(),"Area graphs", "image", 10+hv_HistogramLegendVerticalOffset, 
          5, "black", "box", "false");
    if (HDevWindowStack::IsOpen())
      DispText(HDevWindowStack::GetActive(),"OK", "image", 35+hv_HistogramLegendVerticalOffset, 
          20, hv_OKColorPlot, "box", "false");
    if (HDevWindowStack::IsOpen())
      DispText(HDevWindowStack::GetActive(),"NOK", "image", 60+hv_HistogramLegendVerticalOffset, 
          20, hv_NOKColorPlot, "box", "false");
    //
    if (HDevWindowStack::IsOpen())
      DispText(HDevWindowStack::GetActive(),"Thresholds", "image", 100+hv_HistogramLegendVerticalOffset, 
          5, "black", "box", "false");
    if (0 != hv_DisplayThresholds)
    {
      hv_Offset = 25;
      hv_RunningIndex = 0;
      if (0 != hv_UseSpecificLegend)
      {
        hv_ThresholdCaptions.Clear();
        hv_ThresholdCaptions[0] = "No wrongly classified OK images";
        hv_ThresholdCaptions[1] = "No wrongly classified NOK images";
        hv_ThresholdCaptions[2] = "Minimal sum of OK and NOK fractions";
        for (hv_Index=0; hv_Index<=2; hv_Index+=1)
        {
          hv_Color = HTuple(hv_ThresholdColors[hv_Index]);
          if (HDevWindowStack::IsOpen())
            DispText(HDevWindowStack::GetActive(),HTuple(hv_ThresholdCaptions[hv_Index]), 
                "image", (125+hv_HistogramLegendVerticalOffset)+(hv_Index*hv_Offset), 
                20, hv_Color, "box", "false");
        }
        hv_RunningIndex = 3;
      }
      {
      HTuple end_val377 = (hv_AllThresholds.TupleLength())-1;
      HTuple step_val377 = 1;
      for (hv_Index=hv_RunningIndex; hv_Index.Continue(end_val377, step_val377); hv_Index += step_val377)
      {
        hv_Color = HTuple(hv_ThresholdColors[hv_Index]);
        if (HDevWindowStack::IsOpen())
          DispText(HDevWindowStack::GetActive(),"Threshold "+(hv_Index+1), "image", 
              (125+hv_HistogramLegendVerticalOffset)+(hv_Index*hv_Offset), 20, hv_Color, 
              "box", "false");
      }
      }
    }
    else
    {
      if (HDevWindowStack::IsOpen())
        DispText(HDevWindowStack::GetActive(),"None available for display", "image", 
            125+hv_HistogramLegendVerticalOffset, 20, "black", "box", "false");
    }
    //
    //Recalculate Window Coordinates for the following plots.
    GetDictTuple(hv_WindowHandleDict, "window_score_legend", &hv_WindowHandle);
    GetWindowExtents(hv_WindowHandle, &hv__, &hv__, &hv_Width, &hv_Height);
    hv_WindowMaxColumn = hv_WindowMaxColumn.TupleMax2(hv_Width);
    hv_WindowColumn = (hv_WindowColumn+hv_WindowMaxColumn)+hv_WindowColumnSeparator;
    hv_WindowRow = 0;
    hv_WindowMaxColumn = 0;
    FlushBuffer(hv_WindowHandleScoreLegend);
    SetWindowParam(hv_WindowHandleScoreLegend, "flush", hv_FlushStateScoreLegend);
  }
  //
  //Visualization of precision pie charts.
  if (0 != hv_DisplayPieChartPrecision)
  {
    //
    //Check if there is already a window handle.
    GetDictParam(hv_WindowHandleDict, "key_exists", "window_pie_charts_precision", 
        &hv_WindowExists);
    if (0 != (hv_WindowExists.TupleNot()))
    {
      SetWindowAttr("background_color","black");
      OpenWindow(0,0,512,512,0,"visible","",&hv_WindowHandlePrecision);
      HDevWindowStack::Push(hv_WindowHandlePrecision);
      SetDictTuple(hv_WindowHandleDict, "window_pie_charts_precision", hv_WindowHandlePrecision);
    }
    else
    {
      GetDictTuple(hv_WindowHandleDict, "window_pie_charts_precision", &hv_WindowHandlePrecision);
    }
    //
    //Display.
    set_display_font(hv_WindowHandlePrecision, hv_FontSize, hv_Font, "true", "false");
    dev_display_anomaly_evaluation_pie_chart(hv_WindowHandlePrecision, hv_EvaluationResult, 
        "precision", hv_EvaluationThresholdIndex);
    //
    GetWindowExtents(hv_WindowHandlePrecision, &hv__, &hv__, &hv_Width, &hv_Height);
    HDevWindowStack::SetActive(hv_WindowHandlePrecision);
    if (HDevWindowStack::IsOpen())
      SetWindowExtents(HDevWindowStack::GetActive(),hv_WindowRow, hv_WindowColumn, 
          hv_Width, hv_Height);
    hv_WindowMaxColumn = hv_WindowMaxColumn.TupleMax2(hv_Width);
    hv_WindowRow = (hv_WindowRow+hv_Height)+55;
  }
  //
  // Visualization of recall pie charts.
  if (0 != hv_DisplayPieChartRecall)
  {
    //
    //Check if there is already a window handle.
    GetDictParam(hv_WindowHandleDict, "key_exists", "window_pie_charts_recall", &hv_WindowExists);
    if (0 != (hv_WindowExists.TupleNot()))
    {
      SetWindowAttr("background_color","black");
      OpenWindow(0,0,512,512,0,"visible","",&hv_WindowHandleRecall);
      HDevWindowStack::Push(hv_WindowHandleRecall);
      SetDictTuple(hv_WindowHandleDict, "window_pie_charts_recall", hv_WindowHandleRecall);
    }
    else
    {
      GetDictTuple(hv_WindowHandleDict, "window_pie_charts_recall", &hv_WindowHandleRecall);
    }
    //Display.
    set_display_font(hv_WindowHandleRecall, hv_FontSize, hv_Font, "true", "false");
    dev_display_anomaly_evaluation_pie_chart(hv_WindowHandleRecall, hv_EvaluationResult, 
        "recall", hv_EvaluationThresholdIndex);
    //Recalculate Window Coordinates for the following plots.
    GetWindowExtents(hv_WindowHandleRecall, &hv__, &hv__, &hv_Width, &hv_Height);
    HDevWindowStack::SetActive(hv_WindowHandleRecall);
    if (HDevWindowStack::IsOpen())
      SetWindowExtents(HDevWindowStack::GetActive(),hv_WindowRow, hv_WindowColumn, 
          hv_Width, hv_Height);
    hv_WindowMaxColumn = hv_WindowMaxColumn.TupleMax2(hv_Width);
    hv_WindowRow = (hv_WindowRow+hv_Height)+55;
  }
  //
  //Set window cordinates in order to display in a new column.
  hv_WindowRow = 0;
  hv_WindowColumn = (hv_WindowColumn+hv_WindowMaxColumn)+hv_WindowColumnSeparator;
  hv_WindowMaxColumn = 0;
  //
  //Visualization of confusion matrices.
  if (0 != (hv_DisplayConfusionAbsolute.TupleOr(hv_DisplayConfusionRelative)))
  {
    //Set general generic parameters for dev_display_matrix.
    CreateDict(&hv_GenParam);
    SetDictTuple(hv_GenParam, "display_column_width", hv_DisplayColumnWidth);
    SetDictTuple(hv_GenParam, "display_color", hv_DisplayMatrixColor);
    SetDictTuple(hv_GenParam, "row_col_names_color", hv_RowColNameColor);
    SetDictTuple(hv_GenParam, "axis_titles", (HTuple("Ground truth").Append("Predicted")));
    //Set row and column names.
    GetDictTuple(hv_EvalParam, "class_names", &hv_RowColumnNames);
    if (0 != (int(hv_DisplayMatrixColor==1)))
    {
      //Display off-diagonal values in red.
      hv_ValueColors = HTuple(4,"#ff4500");
      //Display diagonal values in green.
      hv_DiagonalColor = "#00ff00";
      //Matrix entries get indices numbered as following:
      //a(1,1), a(1,2), ..., a(1,n), a(2,1), a(2,2), ... , a(2,n), ... a(m,1), a(m,2), ..., a(m,n)
      hv_DiagonalIndices.Clear();
      hv_DiagonalIndices[0] = 0;
      hv_DiagonalIndices[1] = 3;
      hv_ValueColors[hv_DiagonalIndices] = hv_DiagonalColor;
      SetDictTuple(hv_GenParam, "value_colors", hv_ValueColors);
    }
    //
    //Display confusion matrices.
    hv_DisplayMatrices.Clear();
    hv_DisplayMatrices.Append(hv_DisplayConfusionAbsolute);
    hv_DisplayMatrices.Append(hv_DisplayConfusionRelative);
    hv_Strings.Clear();
    hv_Strings[0] = "absolute";
    hv_Strings[1] = "relative";
    hv_CapitalStrings.Clear();
    hv_CapitalStrings[0] = "Absolute";
    hv_CapitalStrings[1] = "Relative";
    hv_TitelAdditions.Clear();
    hv_TitelAdditions[0] = "    ";
    hv_TitelAdditions[1] = " [%]";
    //
    {
    HTuple end_val474 = (hv_DisplayMatrices.TupleLength())-1;
    HTuple step_val474 = 1;
    for (hv_IndexMatrixToDisplay=0; hv_IndexMatrixToDisplay.Continue(end_val474, step_val474); hv_IndexMatrixToDisplay += step_val474)
    {
      if (0 != (HTuple(hv_DisplayMatrices[hv_IndexMatrixToDisplay]).TupleNot()))
      {
        continue;
      }
      try
      {
        GetDictTuple(hv_EvaluationResult, HTuple(hv_Strings[hv_IndexMatrixToDisplay])+"_confusion_matrix", 
            &hv_ConfusionMatrixDictionaries);
      }
      // catch (Exception) 
      catch (HException &HDevExpDefaultException)
      {
        HDevExpDefaultException.ToHTuple(&hv_Exception);
        throw HException(("No evaluation available for "+HTuple(hv_Strings[hv_IndexMatrixToDisplay]))+" confusion matrix");
      }
      //Check that the specified index is valid.
      if (0 != (int(hv_EvaluationThresholdIndex>((hv_ConfusionMatrixDictionaries.TupleLength())-1))))
      {
        throw HException((("No "+HTuple(hv_Strings[hv_IndexMatrixToDisplay]))+" confusion matrix available for 'classification_threshold_index': ")+hv_EvaluationThresholdIndex);
      }
      hv_ConfusionMatrixDictionary = HTuple(hv_ConfusionMatrixDictionaries[hv_EvaluationThresholdIndex]);
      //
      GetDictTuple(hv_ConfusionMatrixDictionary, "confusion_matrix", &hv_ConfusionMatrix);
      GetDictTuple(hv_ConfusionMatrixDictionary, "threshold", &hv_ThresholdValue);
      //
      SetDictTuple(hv_GenParam, "title", (HTuple(hv_CapitalStrings[hv_IndexMatrixToDisplay])+" confusion matrix")+HTuple(hv_TitelAdditions[hv_IndexMatrixToDisplay]));
      SetDictTuple(hv_GenParam, "subtitle", "Threshold: "+hv_ThresholdValue);
      //
      //Copy matrix for display since it might need to be adapted.
      CopyMatrix(hv_ConfusionMatrix, &hv_ConfusionMatrixDisplay);
      //Convert relative matrix values to percent.
      if (0 != (int(HTuple(hv_Strings[hv_IndexMatrixToDisplay])==HTuple("relative"))))
      {
        GetFullMatrix(hv_ConfusionMatrixDisplay, &hv_ValuesRelative);
        SetFullMatrix(hv_ConfusionMatrixDisplay, 100*hv_ValuesRelative);
      }
      //Calculate proper number format.
      MaxMatrix(hv_ConfusionMatrixDisplay, "full", &hv_MatrixMaxID);
      GetFullMatrix(hv_MatrixMaxID, &hv_MaxValue);
      if (0 != (int(hv_MaxValue>0)))
      {
        hv_NumberFormat = (((hv_MaxValue.TupleLog10()).TupleCeil()).TupleInt())+".0f";
      }
      else
      {
        hv_NumberFormat = "1.0f";
      }
      SetDictTuple(hv_GenParam, "number_format", hv_NumberFormat);
      //
      //Check if there is already a window handle.
      GetDictParam(hv_WindowHandleDict, "key_exists", ("window_"+HTuple(hv_Strings[hv_IndexMatrixToDisplay]))+"_confusion_matrix", 
          &hv_WindowExists);
      if (0 != (hv_WindowExists.TupleNot()))
      {
        SetWindowAttr("background_color","black");
        OpenWindow(0,0,512,512,0,"visible","",&hv_WindowHandleConfusion);
        HDevWindowStack::Push(hv_WindowHandleConfusion);
        SetDictTuple(hv_WindowHandleDict, ("window_"+HTuple(hv_Strings[hv_IndexMatrixToDisplay]))+"_confusion_matrix", 
            hv_WindowHandleConfusion);
      }
      else
      {
        GetDictTuple(hv_WindowHandleDict, ("window_"+HTuple(hv_Strings[hv_IndexMatrixToDisplay]))+"_confusion_matrix", 
            &hv_WindowHandleConfusion);
      }
      //
      //Display.
      set_display_font(hv_WindowHandleConfusion, hv_FontSize, hv_Font, "true", "false");
      dev_display_matrix(hv_ConfusionMatrixDisplay, hv_RowColumnNames, hv_RowColumnNames, 
          hv_WindowHandleConfusion, hv_GenParam);
      //Recalculate Window Coordinates for the following plots.
      GetWindowExtents(hv_WindowHandleConfusion, &hv__, &hv__, &hv_Width, &hv_Height);
      HDevWindowStack::SetActive(hv_WindowHandleConfusion);
      if (HDevWindowStack::IsOpen())
        SetWindowExtents(HDevWindowStack::GetActive(),hv_WindowRow, hv_WindowColumn, 
            hv_Width, hv_Height+15);
      hv_WindowMaxColumn = hv_WindowMaxColumn.TupleMax2(hv_Width);
      hv_WindowRow = (hv_WindowRow+hv_Height)+55;
    }
    }
  }
  //
  return;
}

// Chapter: Deep Learning / Anomaly Detection
// Short Description: Display pie charts for prediction or recall of an anomaly detection evaluation. 
void dev_display_anomaly_evaluation_pie_chart (HTuple hv_WindowHandle, HTuple hv_EvaluationResult, 
    HTuple hv_Mode, HTuple hv_ThresholdIndex)
{

  // Local iconic variables
  HObject  ho_Image;

  // Local control variables
  HTuple  hv_FlushValue, hv_CenterRowOverall, hv_CenterColOverall;
  HTuple  hv_RadiusOverall, hv_RadiusClass, hv_ChartRowDistance;
  HTuple  hv_ChartColDistance, hv_StartRow, hv_StartCol, hv_KeyToPlot;
  HTuple  hv_KeyExists, hv_NumClassCharts, hv_Measures, hv_AllMeanValues;
  HTuple  hv_AllValuesPerClass, hv_MeanValue, hv_ValuesPerClass;
  HTuple  hv_MainTitle, hv_ClassTitles, hv_WindowWidth, hv_WindowHeight;
  HTuple  hv_Colors, hv_ColorDataUnavailable, hv_GenParamPieChart;
  HTuple  hv_Description, hv_PercentStr, hv_Index, hv_CenterRow;
  HTuple  hv_CenterCol;

  //
  //This procedure plots pie charts for precision and recall.
  //More detailed, the mean over all classes that have been used for
  //evaluation and the values for each class are plotted.
  //
  //Switch off automatic flushing to make visualization smooth.
  GetWindowParam(hv_WindowHandle, "flush", &hv_FlushValue);
  SetWindowParam(hv_WindowHandle, "flush", "false");
  //Drawing parameters.
  //Overall pie-chart.
  hv_CenterRowOverall = 150;
  hv_CenterColOverall = 150;
  hv_RadiusOverall = 100;
  //Per-class pie-chart.
  hv_RadiusClass = 40;
  hv_ChartRowDistance = 3*hv_RadiusClass;
  hv_ChartColDistance = 2.5*hv_RadiusClass;
  hv_StartRow = (hv_CenterRowOverall+(1.5*hv_RadiusOverall))+(1.5*hv_RadiusClass);
  hv_StartCol = 1.5*hv_RadiusClass;
  //
  //Get values.
  hv_KeyToPlot = "global_evaluation";
  GetDictParam(hv_EvaluationResult, "key_exists", hv_KeyToPlot, &hv_KeyExists);
  if (0 != hv_KeyExists)
  {
    hv_NumClassCharts = 2;
    GetDictTuple(hv_EvaluationResult, hv_KeyToPlot, &hv_Measures);
    GetDictTuple(hv_Measures, "mean_"+hv_Mode, &hv_AllMeanValues);
    GetDictTuple(hv_Measures, hv_Mode+"_per_class", &hv_AllValuesPerClass);
    //Check that the specified index is valid.
    if (0 != (HTuple(int(hv_ThresholdIndex>((hv_AllMeanValues.TupleLength())-1))).TupleOr(int(hv_ThresholdIndex>(((hv_AllValuesPerClass.TupleLength())/2)-1)))))
    {
      throw HException((("No "+hv_Mode)+" values available for 'classification_threshold_index': ")+hv_ThresholdIndex);
    }
    hv_MeanValue = HTuple(hv_AllMeanValues[hv_ThresholdIndex]);
    hv_ValuesPerClass = hv_AllValuesPerClass.TupleSelectRange(hv_ThresholdIndex*2,(hv_ThresholdIndex*2)+1);
    hv_MainTitle = "Mean class "+hv_Mode;
    hv_ClassTitles.Clear();
    hv_ClassTitles[0] = "ok";
    hv_ClassTitles[1] = "nok";
  }
  else
  {
    throw HException(((("The dictionary EvaluationResult needs a key '"+hv_KeyToPlot)+"' with the ")+hv_Mode)+" values.");
  }
  //
  //Open Window and set font.
  hv_WindowWidth = HTuple(550).TupleMax2(((hv_NumClassCharts-1)*hv_ChartColDistance)+(2*hv_StartCol));
  hv_WindowHeight = (3*hv_RadiusOverall)+(3*hv_RadiusClass);
  HDevWindowStack::SetActive(hv_WindowHandle);
  if (HDevWindowStack::IsOpen())
    SetWindowExtents(HDevWindowStack::GetActive(),0, 0, hv_WindowWidth, hv_WindowHeight);
  GenImageConst(&ho_Image, "byte", hv_WindowWidth, hv_WindowHeight);
  if (HDevWindowStack::IsOpen())
    DispObj(ho_Image, HDevWindowStack::GetActive());
  if (HDevWindowStack::IsOpen())
    ClearWindow(HDevWindowStack::GetActive());
  //
  //Draw the pie-chart.
  hv_Colors.Clear();
  hv_Colors[0] = "green";
  hv_Colors[1] = "red";
  hv_ColorDataUnavailable = "gray";
  CreateDict(&hv_GenParamPieChart);
  SetDictTuple(hv_GenParamPieChart, "title", hv_MainTitle);
  dev_display_pie_chart(hv_WindowHandle, hv_MeanValue.TupleConcat(1.0-hv_MeanValue), 
      hv_CenterRowOverall, hv_CenterColOverall, hv_RadiusOverall, hv_Colors, hv_GenParamPieChart);
  //
  //Write description for pie chart.
  if (HDevWindowStack::IsOpen())
    DispText(HDevWindowStack::GetActive(),"Results: ", "image", 20, (hv_CenterColOverall+hv_RadiusOverall)+20, 
        "white", "box", "false");
  if (0 != (int(hv_Mode==HTuple("precision"))))
  {
    hv_Description.Clear();
    hv_Description[0] = "precision (TP / (TP + FP))";
    hv_Description[1] = "false positives";
  }
  else if (0 != (int(hv_Mode==HTuple("recall"))))
  {
    hv_Description.Clear();
    hv_Description[0] = "recall (TP / (TP + FN))";
    hv_Description[1] = "false negatives";
  }
  else
  {
    throw HException("Mode not supported: "+hv_Mode);
  }
  hv_PercentStr = (((hv_MeanValue.TupleConcat(1.0-hv_MeanValue))*100).TupleString(".1f"))+"";
  {
  HTuple end_val66 = (hv_Description.TupleLength())-1;
  HTuple step_val66 = 1;
  for (hv_Index=0; hv_Index.Continue(end_val66, step_val66); hv_Index += step_val66)
  {
    if (HDevWindowStack::IsOpen())
      DispText(HDevWindowStack::GetActive(),(HTuple(hv_PercentStr[hv_Index])+"%: ")+HTuple(hv_Description[hv_Index]), 
          "image", 30+((hv_Index+1)*20), (hv_CenterColOverall+hv_RadiusOverall)+20, 
          HTuple(hv_Colors[hv_Index]), "box", "false");
  }
  }
  //
  //Per class pie charts.
  {
  HTuple end_val71 = hv_NumClassCharts-1;
  HTuple step_val71 = 1;
  for (hv_Index=0; hv_Index.Continue(end_val71, step_val71); hv_Index += step_val71)
  {
    //Get the position of the class-pie-chart.
    hv_CenterRow = hv_StartRow+((((hv_Index.TupleReal())/2).TupleFloor())*hv_ChartRowDistance);
    hv_CenterCol = hv_StartCol+((hv_Index%2)*hv_ChartColDistance);
    SetDictTuple(hv_GenParamPieChart, "title", HTuple(hv_ClassTitles[hv_Index]));
    if (0 != (int(HTuple(hv_ValuesPerClass[hv_Index])>=0)))
    {
      dev_display_pie_chart(hv_WindowHandle, HTuple(hv_ValuesPerClass[hv_Index]).TupleConcat(1.0-HTuple(hv_ValuesPerClass[hv_Index])), 
          hv_CenterRow, hv_CenterCol, hv_RadiusClass, hv_Colors, hv_GenParamPieChart);
    }
    else
    {
      //No evaluation data was available for this class.
      dev_display_pie_chart(hv_WindowHandle, 1.0, hv_CenterRow, hv_CenterCol, hv_RadiusClass, 
          hv_ColorDataUnavailable, hv_GenParamPieChart);
    }
  }
  }
  //
  //Flush buffer and reset value for 'flush'.
  FlushBuffer(hv_WindowHandle);
  SetWindowParam(hv_WindowHandle, "flush", hv_FlushValue);
  //
  return;
}

// Chapter: Graphics / Output
// Short Description: Display a map of the confidences. 
void dev_display_confidence_regions (HObject ho_ImageConfidence, HTuple hv_DrawTransparency, 
    HTuple *hv_Colors)
{

  // Local iconic variables
  HObject  ho_Region;

  // Local control variables
  HTuple  hv_NumColors, hv_WeightsColorsAlpha, hv_ColorIndex;
  HTuple  hv_Threshold, hv_MinGray, hv_MaxGray;

  //
  //This procedure displays a map of the confidences
  //given in ImageConfidence as regions.
  //DrawTransparency determines the alpha value of the colors.
  //The used colors are returned.
  //
  //Define colors.
  hv_NumColors = 20;
  get_distinct_colors(hv_NumColors, 0, 0, 100, &(*hv_Colors));
  hv_WeightsColorsAlpha = (*hv_Colors)+hv_DrawTransparency;
  hv_ColorIndex = 0;
  //
  //Threshold the image according to
  //the number of colors and
  //display resulting regions.
  {
  HTuple end_val15 = hv_NumColors-1;
  HTuple step_val15 = 1;
  for (hv_ColorIndex=0; hv_ColorIndex.Continue(end_val15, step_val15); hv_ColorIndex += step_val15)
  {
    hv_Threshold = hv_ColorIndex*(1.0/hv_NumColors);
    hv_MinGray = hv_Threshold;
    hv_MaxGray = hv_Threshold+(1/hv_NumColors);
    Threshold(ho_ImageConfidence, &ho_Region, hv_Threshold, hv_Threshold+(1.0/hv_NumColors));
    if (HDevWindowStack::IsOpen())
      SetColor(HDevWindowStack::GetActive(),HTuple(hv_WeightsColorsAlpha[hv_ColorIndex]));
    if (HDevWindowStack::IsOpen())
      DispObj(ho_Region, HDevWindowStack::GetActive());
  }
  }
  return;
}

// Chapter: Deep Learning / Model
// Short Description: Visualize different images, annotations and inference results for a sample. 
void dev_display_dl_data (HTuple hv_DLSample, HTuple hv_DLResult, HTuple hv_DLDatasetInfo, 
    HTuple hv_KeysForDisplay, HTuple hv_GenParam, HTuple hv_WindowHandleDict, HTuple* hv_WindowHandle)
{

  // Local iconic variables
  HObject  ho_Image, ho_AnomalyImage, ho_AnomalyRegion;
  HObject  ho_PredictionColorFrame, ho_ImageHeatmap, ho_ImageWeight;
  HObject  ho_ImageConfidence, ho_SegmentationImagGroundTruth;
  HObject  ho_SegmentationImageResult, ho_ImageAbsDiff, ho_DiffRegion;

  // Local control variables
  HTuple  hv_ThresholdWidth, hv_ScaleWindows, hv_Font;
  HTuple  hv_FontSize, hv_LineWidth, hv_MapTransparency, hv_MapColorBarWidth;
  HTuple  hv_AnomalyRegionThreshold, hv_AnomalyClassificationThreshold;
  HTuple  hv_AnomalyRegionLabelColor, hv_AnomalyColorTransparency;
  HTuple  hv_AnomalyRegionResultColor, hv_SegMaxWeight, hv_SegDraw;
  HTuple  hv_SegTransparency, hv_SegExcludeClassIDs, hv_BboxLabelColor;
  HTuple  hv_BboxDisplayConfidence, hv_BboxTextColor, hv_ShowBottomDesc;
  HTuple  hv_ShowLegend, hv_ShowGroundTruthAnomalyRegions;
  HTuple  hv_ShowClassificationIDs, hv_ShowClassificationColorFrame;
  HTuple  hv_ShowLabels, hv_ShowDirection, hv_HeatmapColorScheme;
  HTuple  hv_GenParamNames, hv_ParamIndex, hv_GenParamName;
  HTuple  hv_GenParamValue, hv_SampleKeys, hv_ResultKeys;
  HTuple  hv_ImageIDExists, hv_ImageID, hv_ImageIDString;
  HTuple  hv_ImageIDStringBraces, hv_ImageIDStringCapital;
  HTuple  hv_NeededKeys, hv_Index, hv_DLDatasetInfoKeys, hv_ClassNames;
  HTuple  hv_ClassIDs, hv_Colors, hv_ClassesLegend, hv_PrevWindowCoordinates;
  HTuple  hv_Keys, hv_Exception, hv_MetaInfoIndex, hv_MetaInfo;
  HTuple  hv_FlushValues, hv_WindowHandleKeys, hv_KeyIndex;
  HTuple  hv_WindowHandles, hv_WindowIndex, hv_FlushValue;
  HTuple  hv_WidthImage, hv_HeightImage, hv_CurrentWindowHandle;
  HTuple  hv_WindowImageRatio, hv_AnomalyLabelGroundTruth;
  HTuple  hv_AnomalyLabelIDGroundTruth, hv_AnomalyRegionExists;
  HTuple  hv_Text, hv_AnomalyScore, hv_AnomalyClassID, hv_AnomalyClassThresholdDisplay;
  HTuple  hv_AnomalyRegionThresholdDisplay, hv_AnomalyRegionGroundTruthExists;
  HTuple  hv_PredictionColor, hv_LineColors, hv_ResultColorOffset;
  HTuple  hv_ClassificationLabelIDGroundTruth, hv_ClassificationLabelIDResult;
  HTuple  hv_MarginBottom, hv_WindowCoordinates, hv_CurrentWindowHeight;
  HTuple  hv__, hv_MaxHeight, hv_PredictionText, hv_BoarderOffset;
  HTuple  hv_WindowImageRatioHeight, hv_WindowImageRatioWidth;
  HTuple  hv_BoarderOffsetRow, hv_BoarderOffsetCol, hv_SelectedHeatmapMethod;
  HTuple  hv_DictHeatmap, hv_MethodName, hv_HeatmapKeys, hv_HeatmapImageName;
  HTuple  hv_TargetClassID, hv_Confidences, hv_MaxDeviation;
  HTuple  hv_ClassificationLabelNameResult, hv_TargetClassConfidence;
  HTuple  hv_ClassificationLabelNamesGroundTruth, hv_BboxIDs;
  HTuple  hv_BboxColors, hv_BboxIDsUniq, hv_BboxConfidences;
  HTuple  hv_TextConf, hv_BboxClassIndex, hv_BboxColorsResults;
  HTuple  hv_BboxClassIndexUniq, hv_BboxLabelIndex, hv_BboxColorsBoth;
  HTuple  hv_BboxClassLabelIndexUniq, hv_ColorsSegmentation;
  HTuple  hv_DrawMode, hv_Width, hv_ImageClassIDs, hv_ImageClassIDsUniq;
  HTuple  hv_ImageClassIDsIndices, hv_ImageClassIDsIndex;
  HTuple  hv_ColorsResults, hv_GroundTruthIDs, hv_ResultIDs;
  HTuple  hv_StringSegExcludeClassIDs, hv_StringIndex, hv_Min;
  HTuple  hv_Max, hv_Range, hv_MinWeight, hv_WeightsColors;
  HTuple  hv_ConfidenceColors, hv_Indices, hv_WindowHandleKeysNew;

  //
  //This procedure displays the content of the provided DLSample and/or DLResult
  //depending on the input string KeysForDisplay.
  //DLDatasetInfo is a dictionary containing the information about the dataset.
  //The visualization can be adapted with GenParam.
  //
  //** Set the default values: ***
  //
  //Define the screen width when a new window row is started.
  hv_ThresholdWidth = 1024;
  //Since potentially a lot of windows are opened,
  //scale the windows consistently.
  hv_ScaleWindows = 0.8;
  //Set a font and a font size.
  hv_Font = "mono";
  hv_FontSize = 14;
  //
  hv_LineWidth = 2;
  hv_MapTransparency = "cc";
  hv_MapColorBarWidth = 140;
  //
  //Define anomaly detection-specific parameter values.
  hv_AnomalyRegionThreshold = -1;
  hv_AnomalyClassificationThreshold = -1;
  hv_AnomalyRegionLabelColor = "#40e0d0";
  hv_AnomalyColorTransparency = "40";
  hv_AnomalyRegionResultColor = "#ff0000c0";
  //
  //Define segmentation-specific parameter values.
  hv_SegMaxWeight = 0;
  hv_SegDraw = "fill";
  hv_SegTransparency = "aa";
  hv_SegExcludeClassIDs = HTuple();
  //
  //Define bounding box-specific parameter values.
  hv_BboxLabelColor = HTuple("#000000")+"99";
  hv_BboxDisplayConfidence = 1;
  hv_BboxTextColor = "#eeeeee";
  //
  //By default, display a description on the bottom.
  hv_ShowBottomDesc = 0;
  //
  //By default, show a legend with class IDs.
  hv_ShowLegend = 0;
  //
  //By default, show the anomaly ground truth regions.
  hv_ShowGroundTruthAnomalyRegions = 1;
  //
  //By default, show class IDs and color frames for classification ground truth/results.
  hv_ShowClassificationIDs = 1;
  hv_ShowClassificationColorFrame = 1;
  //
  //By default, show class labels for detection ground truth/results.
  hv_ShowLabels = 1;
  //
  //By default, show direction of the ground truth/results instances for detection with instance_type 'rectangle2'.
  hv_ShowDirection = 1;
  //
  //By default, use color scheme 'Jet' for the heatmap display.
  hv_HeatmapColorScheme = "jet";
  //** Set user defined values: ***
  //
  //Overwrite default values by given generic parameters.
  if (0 != (int(hv_GenParam!=HTuple())))
  {
    GetDictParam(hv_GenParam, "keys", HTuple(), &hv_GenParamNames);
    {
    HTuple end_val65 = (hv_GenParamNames.TupleLength())-1;
    HTuple step_val65 = 1;
    for (hv_ParamIndex=0; hv_ParamIndex.Continue(end_val65, step_val65); hv_ParamIndex += step_val65)
    {
      hv_GenParamName = HTuple(hv_GenParamNames[hv_ParamIndex]);
      GetDictTuple(hv_GenParam, hv_GenParamName, &hv_GenParamValue);
      if (0 != (int(hv_GenParamName==HTuple("threshold_width"))))
      {
        hv_ThresholdWidth = hv_GenParamValue;
      }
      else if (0 != (int(hv_GenParamName==HTuple("scale_windows"))))
      {
        hv_ScaleWindows = hv_GenParamValue;
      }
      else if (0 != (int(hv_GenParamName==HTuple("font"))))
      {
        hv_Font = hv_GenParamValue;
      }
      else if (0 != (int(hv_GenParamName==HTuple("font_size"))))
      {
        hv_FontSize = hv_GenParamValue;
      }
      else if (0 != (int(hv_GenParamName==HTuple("line_width"))))
      {
        hv_LineWidth = hv_GenParamValue;
      }
      else if (0 != (int(hv_GenParamName==HTuple("map_transparency"))))
      {
        hv_MapTransparency = hv_GenParamValue;
      }
      else if (0 != (int(hv_GenParamName==HTuple("map_color_bar_width"))))
      {
        hv_MapColorBarWidth = hv_GenParamValue;
      }
      else if (0 != (int(hv_GenParamName==HTuple("segmentation_max_weight"))))
      {
        hv_SegMaxWeight = hv_GenParamValue;
      }
      else if (0 != (int(hv_GenParamName==HTuple("segmentation_draw"))))
      {
        hv_SegDraw = hv_GenParamValue;
      }
      else if (0 != (int(hv_GenParamName==HTuple("segmentation_transparency"))))
      {
        hv_SegTransparency = hv_GenParamValue;
      }
      else if (0 != (int(hv_GenParamName==HTuple("segmentation_exclude_class_ids"))))
      {
        hv_SegExcludeClassIDs = hv_GenParamValue;
      }
      else if (0 != (int(hv_GenParamName==HTuple("bbox_label_color"))))
      {
        hv_BboxLabelColor = hv_GenParamValue;
      }
      else if (0 != (int(hv_GenParamName==HTuple("bbox_display_confidence"))))
      {
        hv_BboxDisplayConfidence = hv_GenParamValue;
      }
      else if (0 != (int(hv_GenParamName==HTuple("bbox_text_color"))))
      {
        hv_BboxTextColor = hv_GenParamValue;
      }
      else if (0 != (int(hv_GenParamName==HTuple("display_bottom_desc"))))
      {
        hv_ShowBottomDesc = hv_GenParamValue;
      }
      else if (0 != (int(hv_GenParamName==HTuple("display_legend"))))
      {
        hv_ShowLegend = hv_GenParamValue;
      }
      else if (0 != (int(hv_GenParamName==HTuple("display_classification_ids"))))
      {
        hv_ShowClassificationIDs = hv_GenParamValue;
      }
      else if (0 != (int(hv_GenParamName==HTuple("display_classification_color_frame"))))
      {
        hv_ShowClassificationColorFrame = hv_GenParamValue;
      }
      else if (0 != (int(hv_GenParamName==HTuple("display_labels"))))
      {
        hv_ShowLabels = hv_GenParamValue;
      }
      else if (0 != (int(hv_GenParamName==HTuple("display_direction"))))
      {
        hv_ShowDirection = hv_GenParamValue;
      }
      else if (0 != (int(hv_GenParamName==HTuple("heatmap_color_scheme"))))
      {
        hv_HeatmapColorScheme = hv_GenParamValue;
      }
      else if (0 != (int(hv_GenParamName==HTuple("display_ground_truth_anomaly_regions"))))
      {
        hv_ShowGroundTruthAnomalyRegions = hv_GenParamValue;
      }
      else if (0 != (int(hv_GenParamName==HTuple("anomaly_region_threshold"))))
      {
        hv_AnomalyRegionThreshold = hv_GenParamValue;
      }
      else if (0 != (int(hv_GenParamName==HTuple("anomaly_classification_threshold"))))
      {
        hv_AnomalyClassificationThreshold = hv_GenParamValue;
      }
      else if (0 != (int(hv_GenParamName==HTuple("anomaly_region_label_color"))))
      {
        hv_AnomalyRegionLabelColor = hv_GenParamValue;
      }
      else if (0 != (int(hv_GenParamName==HTuple("anomaly_region_result_color"))))
      {
        hv_AnomalyRegionResultColor = hv_GenParamValue;
      }
      else if (0 != (int(hv_GenParamName==HTuple("anomaly_color_transparency"))))
      {
        hv_AnomalyColorTransparency = hv_GenParamValue;
      }
      else
      {
        throw HException(("Unknown generic parameter: "+hv_GenParamName)+".");
      }
    }
    }
  }
  //
  //Get the dictionary keys.
  GetDictParam(hv_DLSample, "keys", HTuple(), &hv_SampleKeys);
  if (0 != (int(hv_DLResult!=HTuple())))
  {
    GetDictParam(hv_DLResult, "keys", HTuple(), &hv_ResultKeys);
  }
  //
  //Get image ID if it is available.
  GetDictParam(hv_DLSample, "key_exists", "image_id", &hv_ImageIDExists);
  if (0 != hv_ImageIDExists)
  {
    GetDictTuple(hv_DLSample, "image_id", &hv_ImageID);
    hv_ImageIDString = "image ID "+hv_ImageID;
    hv_ImageIDStringBraces = ("(image ID "+hv_ImageID)+")";
    hv_ImageIDStringCapital = "Image ID "+hv_ImageID;
  }
  else
  {
    hv_ImageIDString = "";
    hv_ImageIDStringBraces = hv_ImageIDString;
    hv_ImageIDStringCapital = hv_ImageIDString;
  }
  //
  //Check if DLDatasetInfo is valid.
  if (0 != (int(hv_DLDatasetInfo==HTuple())))
  {
    //If DLDatasetInfo is empty, 'image' is the only key allowed in KeysForDisplay.
    if (0 != (HTuple(int((hv_KeysForDisplay.TupleLength())!=1)).TupleOr(int(HTuple(hv_KeysForDisplay[0])!=HTuple("image")))))
    {
      throw HException("DLDatasetInfo is needed for requested keys in KeysForDisplay.");
    }
  }
  else
  {
    //Check if DLDatasetInfo contains necessary keys.
    hv_NeededKeys.Clear();
    hv_NeededKeys[0] = "class_names";
    hv_NeededKeys[1] = "class_ids";
    {
    HTuple end_val156 = (hv_NeededKeys.TupleLength())-1;
    HTuple step_val156 = 1;
    for (hv_Index=0; hv_Index.Continue(end_val156, step_val156); hv_Index += step_val156)
    {
      GetDictParam(hv_DLDatasetInfo, "keys", HTuple(), &hv_DLDatasetInfoKeys);
      if (0 != (int((hv_DLDatasetInfoKeys.TupleFindFirst(HTuple(hv_NeededKeys[hv_Index])))==-1)))
      {
        throw HException(("Key "+HTuple(hv_NeededKeys[hv_Index]))+" is missing in DLDatasetInfo.");
      }
    }
    }
    //
    //Get the general dataset information, if available.
    GetDictTuple(hv_DLDatasetInfo, "class_names", &hv_ClassNames);
    GetDictTuple(hv_DLDatasetInfo, "class_ids", &hv_ClassIDs);
    //
    //Define distinct colors for the classes.
    get_dl_class_colors(hv_ClassNames, &hv_Colors);
    //
    hv_ClassesLegend = (hv_ClassIDs+" : ")+hv_ClassNames;
  }
  //
  //** Set window parameters: ***
  //
  //Set previous window coordinates.
  hv_PrevWindowCoordinates.Clear();
  hv_PrevWindowCoordinates[0] = 0;
  hv_PrevWindowCoordinates[1] = 0;
  hv_PrevWindowCoordinates[2] = 0;
  hv_PrevWindowCoordinates[3] = 0;
  hv_PrevWindowCoordinates[4] = 1;
  //
  //Check that the WindowHandleDict is of type dictionary.
  try
  {
    GetDictParam(hv_WindowHandleDict, "keys", HTuple(), &hv_Keys);
  }
  // catch (Exception) 
  catch (HException &HDevExpDefaultException)
  {
    HDevExpDefaultException.ToHTuple(&hv_Exception);
    if (0 != (int(HTuple(hv_Exception[0])==1401)))
    {
      throw HException("WindowHandleDict has to be of type dictionary. Use create_dict to create an empty dictionary.");
    }
    else
    {
      throw HException(hv_Exception);
    }
  }
  //For better usage, add meta information about the window handles in WindowHandleDict.
  TupleFind(hv_Keys, "meta_information", &hv_MetaInfoIndex);
  if (0 != (HTuple(int(hv_MetaInfoIndex==-1)).TupleOr(int(hv_MetaInfoIndex==HTuple()))))
  {
    CreateDict(&hv_MetaInfo);
    SetDictTuple(hv_WindowHandleDict, "meta_information", hv_MetaInfo);
  }
  //
  //For each window, set 'flush' to 'false' to avoid flickering.
  hv_FlushValues = HTuple();
  GetDictParam(hv_WindowHandleDict, "keys", HTuple(), &hv_WindowHandleKeys);
  {
  HTuple end_val198 = (hv_WindowHandleKeys.TupleLength())-1;
  HTuple step_val198 = 1;
  for (hv_Index=0; hv_Index.Continue(end_val198, step_val198); hv_Index += step_val198)
  {
    //Only consider the WindowHandleKeys that are needed for the current visualization.
    hv_KeyIndex = hv_KeysForDisplay.TupleFind(HTuple(hv_WindowHandleKeys[hv_Index]));
    if (0 != (HTuple(int(hv_KeyIndex!=-1)).TupleAnd(int(hv_KeyIndex!=HTuple()))))
    {
      GetDictTuple(hv_WindowHandleDict, HTuple(hv_WindowHandleKeys[hv_Index]), &hv_WindowHandles);
      {
      HTuple end_val203 = (hv_WindowHandles.TupleLength())-1;
      HTuple step_val203 = 1;
      for (hv_WindowIndex=0; hv_WindowIndex.Continue(end_val203, step_val203); hv_WindowIndex += step_val203)
      {
        GetWindowParam(HTuple(hv_WindowHandles[hv_WindowIndex]), "flush", &hv_FlushValue);
        hv_FlushValues = hv_FlushValues.TupleConcat(hv_FlushValue);
        SetWindowParam(HTuple(hv_WindowHandles[hv_WindowIndex]), "flush", "false");
      }
      }
    }
  }
  }
  //
  //** Display the data: ***
  //
  //Display data dictionaries.
  {
  HTuple end_val214 = (hv_KeysForDisplay.TupleLength())-1;
  HTuple step_val214 = 1;
  for (hv_KeyIndex=0; hv_KeyIndex.Continue(end_val214, step_val214); hv_KeyIndex += step_val214)
  {
    if (0 != (int(HTuple(hv_KeysForDisplay[hv_KeyIndex])==HTuple("image"))))
    {
      //
      //Image.
      get_image(&ho_Image, hv_SampleKeys, hv_DLSample);
      //
      //Get or open next window.
      GetImageSize(ho_Image, &hv_WidthImage, &hv_HeightImage);
      get_next_window(hv_Font, hv_FontSize, hv_ShowBottomDesc, hv_WidthImage, hv_HeightImage, 
          0, hv_ScaleWindows, hv_ThresholdWidth, hv_PrevWindowCoordinates, hv_WindowHandleDict, 
          HTuple(hv_KeysForDisplay[hv_KeyIndex]), &hv_CurrentWindowHandle, &hv_WindowImageRatio, 
          &hv_PrevWindowCoordinates);
      //
      if (0 != hv_WindowHandle)
          DispObj(ho_Image, *hv_WindowHandle);
      else if (HDevWindowStack::IsOpen())
          DispObj(ho_Image, HDevWindowStack::GetActive());
      if (0 != hv_ShowBottomDesc)
      {
          if (0 != hv_WindowHandle)
              DispText(*hv_WindowHandle, hv_ImageIDStringCapital, "window",
              "bottom", "left", "white", "box", "false");
          else if (HDevWindowStack::IsOpen())
            DispText(HDevWindowStack::GetActive(),hv_ImageIDStringCapital, "window", 
              "bottom", "left", "white", "box", "false");
      }
    }
    else if (0 != (int(HTuple(hv_KeysForDisplay[hv_KeyIndex])==HTuple("anomaly_ground_truth"))))
    {
      //Image.
      get_image(&ho_Image, hv_SampleKeys, hv_DLSample);
      get_anomaly_ground_truth_label(hv_SampleKeys, hv_DLSample, &hv_AnomalyLabelGroundTruth, 
          &hv_AnomalyLabelIDGroundTruth);
      //
      //Get or open next window.
      GetImageSize(ho_Image, &hv_WidthImage, &hv_HeightImage);
      get_next_window(hv_Font, hv_FontSize, hv_ShowBottomDesc, hv_WidthImage, hv_HeightImage, 
          0, hv_ScaleWindows, hv_ThresholdWidth, hv_PrevWindowCoordinates, hv_WindowHandleDict, 
          HTuple(hv_KeysForDisplay[hv_KeyIndex]), &hv_CurrentWindowHandle, &hv_WindowImageRatio, 
          &hv_PrevWindowCoordinates);
      //
      //Visualize image.
      if (0 != hv_WindowHandle)
          DispObj(ho_Image, *hv_WindowHandle);
      else if (HDevWindowStack::IsOpen())
        DispObj(ho_Image, HDevWindowStack::GetActive());
      //
      hv_AnomalyRegionExists = "false";
      if (0 != hv_ShowGroundTruthAnomalyRegions)
      {
        //Show the ground truth region.
        dev_display_ground_truth_anomaly_regions(hv_SampleKeys, hv_DLSample, hv_CurrentWindowHandle, 
            hv_LineWidth, hv_AnomalyRegionLabelColor, hv_AnomalyColorTransparency, 
            &hv_AnomalyRegionExists);
      }
      //
      hv_Text = "Ground truth anomalies "+hv_ImageIDStringBraces;
      if (0 != hv_ShowBottomDesc)
      {
          if (0 != hv_WindowHandle)
              DispText(*hv_WindowHandle, hv_Text, "window", "bottom", "left",
                  "white", "box", "false");
          else if (HDevWindowStack::IsOpen())
          DispText(HDevWindowStack::GetActive(),hv_Text, "window", "bottom", "left", 
              "white", "box", "false");
      }
      //
      //Display the legend.
      if (0 != hv_ShowLegend)
      {
        hv_Text[hv_Text.TupleLength()] = "";
        hv_Text[hv_Text.TupleLength()] = ((hv_AnomalyLabelIDGroundTruth+" : '")+hv_AnomalyLabelGroundTruth)+"'";
        if (0 != (HTuple(HTuple(int(hv_AnomalyRegionExists==HTuple("false"))).TupleAnd(int(hv_AnomalyLabelIDGroundTruth==1))).TupleAnd(hv_ShowGroundTruthAnomalyRegions)))
        {
          hv_Text[hv_Text.TupleLength()] = "";
          hv_Text[hv_Text.TupleLength()] = "No 'anomaly_ground_truth' exists!";
        }
        //
        //Get or open next child window
        get_child_window(hv_HeightImage, hv_Font, hv_FontSize, hv_Text, hv_PrevWindowCoordinates, 
            hv_WindowHandleDict, HTuple(hv_KeysForDisplay[hv_KeyIndex]), &hv_WindowImageRatio, 
            &hv_PrevWindowCoordinates);
        if (0 != hv_WindowHandle)
            DispText(*hv_WindowHandle, hv_Text, "window", "top", "left",
                "white", "box", "false");
        else if (HDevWindowStack::IsOpen())
          DispText(HDevWindowStack::GetActive(),hv_Text, "window", "top", "left", 
              "white", "box", "false");
      }
    }
    else if (0 != (int(HTuple(hv_KeysForDisplay[hv_KeyIndex])==HTuple("anomaly_result"))))
    {
      //
      //Get image.
      get_image(&ho_Image, hv_SampleKeys, hv_DLSample);
      //
      //Get the anomaly results either by applying the specified thresholds or out of DLResult.
      get_anomaly_result(&ho_AnomalyImage, &ho_AnomalyRegion, hv_DLResult, hv_ResultKeys, 
          hv_AnomalyClassificationThreshold, hv_AnomalyRegionThreshold, &hv_AnomalyScore, 
          &hv_AnomalyClassID, &hv_AnomalyClassThresholdDisplay, &hv_AnomalyRegionThresholdDisplay);
     
      //
      //Visualize image.
      if (0 != hv_WindowHandle)
      {
          DispObj(ho_Image, *hv_WindowHandle);
      }
      else
      {
          //
     //Get or open next window.
          GetImageSize(ho_Image, &hv_WidthImage, &hv_HeightImage);
          get_next_window(hv_Font, hv_FontSize, hv_ShowBottomDesc, hv_WidthImage, hv_HeightImage,
              0, hv_ScaleWindows, hv_ThresholdWidth, hv_PrevWindowCoordinates, hv_WindowHandleDict,
              HTuple(hv_KeysForDisplay[hv_KeyIndex]), &hv_CurrentWindowHandle, &hv_WindowImageRatio,
              &hv_PrevWindowCoordinates);
          if (HDevWindowStack::IsOpen())
              DispObj(ho_Image, HDevWindowStack::GetActive());
      }
      //
      //Display anomaly regions defined by the specified threshold or from DLResult.
      if (0 != (HTuple(int(hv_AnomalyRegionThreshold!=-1)).TupleOr(int((hv_ResultKeys.TupleFind("anomaly_region"))!=-1))))
      {
        dev_display_result_anomaly_regions(ho_AnomalyRegion, hv_CurrentWindowHandle, 
            hv_LineWidth, hv_AnomalyRegionResultColor, hv_WindowHandle);
      }
      //
      if (0 != hv_ShowBottomDesc)
      {
          hv_Text = "Detected anomalies " + hv_ImageIDStringBraces;
          if (0 != hv_WindowHandle)
          {
              DispText(*hv_WindowHandle, hv_Text, "window", "bottom", "left",
                  "white", "box", "false");
          }
          else if (HDevWindowStack::IsOpen())
          DispText(HDevWindowStack::GetActive(),hv_Text, "window", "bottom", "left", 
              "white", "box", "false");
      }
      //
      //Display the legend.
      if (0 != hv_ShowLegend)
      {
        hv_Text[hv_Text.TupleLength()] = "";
        hv_Text[hv_Text.TupleLength()] = "---------------";
        hv_Text[hv_Text.TupleLength()] = "Results ";
        hv_Text[hv_Text.TupleLength()] = "---------------";
        if (0 != (int(hv_AnomalyClassID==1)))
        {
          hv_Text[hv_Text.TupleLength()] = hv_AnomalyClassID+" : 'nok'";
        }
        else if (0 != (int(hv_AnomalyClassID==0)))
        {
          hv_Text[hv_Text.TupleLength()] = hv_AnomalyClassID+" : 'ok'";
        }
        else
        {
          hv_Text[hv_Text.TupleLength()] = "No classification result found";
        }
        if (0 != (HTuple(int(hv_AnomalyRegionThreshold==-1)).TupleAnd(int((hv_ResultKeys.TupleFind("anomaly_region"))==-1))))
        {
          hv_Text[hv_Text.TupleLength()] = "";
          hv_Text[hv_Text.TupleLength()] = "No segmentation result found";
        }
        hv_Text[hv_Text.TupleLength()] = "";
        hv_Text[hv_Text.TupleLength()] = "Anomaly score: "+(hv_AnomalyScore.TupleString(".2f"));
        hv_Text[hv_Text.TupleLength()] = "";
        if (0 != (HTuple(int(hv_AnomalyClassThresholdDisplay!=-1)).TupleOr(int(hv_AnomalyRegionThresholdDisplay!=-1))))
        {
          hv_Text[hv_Text.TupleLength()] = "---------------";
          hv_Text[hv_Text.TupleLength()] = "Thresholds ";
          hv_Text[hv_Text.TupleLength()] = "---------------";
        }
        //
        if (0 != (int(hv_AnomalyClassThresholdDisplay!=-1)))
        {
          hv_Text[hv_Text.TupleLength()] = "Classification: "+(hv_AnomalyClassThresholdDisplay.TupleString(".2f"));
          hv_Text[hv_Text.TupleLength()] = "";
        }
        if (0 != (int(hv_AnomalyRegionThresholdDisplay!=-1)))
        {
          hv_Text[hv_Text.TupleLength()] = "Segmentation: "+(hv_AnomalyRegionThresholdDisplay.TupleString(".2f"));
          hv_Text[hv_Text.TupleLength()] = "";
        }
        //
        //Get or open next child window
        get_child_window(hv_HeightImage, hv_Font, hv_FontSize, hv_Text, hv_PrevWindowCoordinates, 
            hv_WindowHandleDict, HTuple(hv_KeysForDisplay[hv_KeyIndex]), &hv_WindowImageRatio, 
            &hv_PrevWindowCoordinates);
        if (0 != hv_WindowHandle)
            DispText(*hv_WindowHandle, hv_Text, "window", "top", "left",
                "white", "box", "false");
        else if (HDevWindowStack::IsOpen())
          DispText(HDevWindowStack::GetActive(),hv_Text, "window", "top", "left", 
              "white", "box", "false");
      }
    }
    else if (0 != (int(HTuple(hv_KeysForDisplay[hv_KeyIndex])==HTuple("anomaly_both"))))
    {
      //
      //Get image and ground truth.
      get_image(&ho_Image, hv_SampleKeys, hv_DLSample);
      get_anomaly_ground_truth_label(hv_SampleKeys, hv_DLSample, &hv_AnomalyLabelGroundTruth, 
          &hv_AnomalyLabelIDGroundTruth);
      //
      //Get the anomaly results either by applying the specified thresholds or out of DLResult.
      get_anomaly_result(&ho_AnomalyImage, &ho_AnomalyRegion, hv_DLResult, hv_ResultKeys, 
          hv_AnomalyClassificationThreshold, hv_AnomalyRegionThreshold, &hv_AnomalyScore, 
          &hv_AnomalyClassID, &hv_AnomalyClassThresholdDisplay, &hv_AnomalyRegionThresholdDisplay);
      //
      //Get or open next window.
      GetImageSize(ho_Image, &hv_WidthImage, &hv_HeightImage);
      get_next_window(hv_Font, hv_FontSize, hv_ShowBottomDesc, hv_WidthImage, hv_HeightImage, 
          0, hv_ScaleWindows, hv_ThresholdWidth, hv_PrevWindowCoordinates, hv_WindowHandleDict, 
          HTuple(hv_KeysForDisplay[hv_KeyIndex]), &hv_CurrentWindowHandle, &hv_WindowImageRatio, 
          &hv_PrevWindowCoordinates);
      //
      //Visualize image, ground truth (if available), and result regions.
      if (0 != hv_WindowHandle)
          DispObj(ho_Image, *hv_WindowHandle);
      else if (HDevWindowStack::IsOpen())
        DispObj(ho_Image, HDevWindowStack::GetActive());
      hv_AnomalyRegionGroundTruthExists = "false";
      if (0 != hv_ShowGroundTruthAnomalyRegions)
      {
        dev_display_ground_truth_anomaly_regions(hv_SampleKeys, hv_DLSample, hv_CurrentWindowHandle, 
            hv_LineWidth, hv_AnomalyRegionLabelColor, hv_AnomalyColorTransparency, 
            &hv_AnomalyRegionGroundTruthExists);
      }
      //
      //Display anomaly regions defined by the specified threshold or from DLResult.
      if (0 != (HTuple(int(hv_AnomalyRegionThreshold!=-1)).TupleOr(int((hv_ResultKeys.TupleFind("anomaly_region"))!=-1))))
      {
        dev_display_result_anomaly_regions(ho_AnomalyRegion, hv_CurrentWindowHandle, 
            hv_LineWidth, hv_AnomalyRegionResultColor);
      }
      //
      hv_Text = "GT and detected anomalies "+hv_ImageIDStringBraces;
      if (0 != hv_ShowBottomDesc)
      {
          if (0 != hv_WindowHandle)
              DispText(*hv_WindowHandle, hv_Text, "window", "bottom", "left",
                  "white", "box", "false");
          else if (HDevWindowStack::IsOpen())
          DispText(HDevWindowStack::GetActive(),hv_Text, "window", "bottom", "left", 
              "white", "box", "false");
      }
      //
      //Get prediction color.
      hv_PredictionColor = "white";
      if (0 != (int(hv_AnomalyLabelIDGroundTruth==hv_AnomalyClassID)))
      {
        hv_PredictionColor = "green";
      }
      else if (0 != (int(hv_AnomalyClassID!=-1)))
      {
        hv_PredictionColor = "red";
      }
      //
      //Display the legend.
      if (0 != hv_ShowLegend)
      {
        hv_Text[hv_Text.TupleLength()] = "";
        hv_Text[hv_Text.TupleLength()] = "---------------";
        hv_Text[hv_Text.TupleLength()] = "Ground truth ";
        hv_Text[hv_Text.TupleLength()] = "---------------";
        hv_Text[hv_Text.TupleLength()] = ((hv_AnomalyLabelIDGroundTruth+" : '")+hv_AnomalyLabelGroundTruth)+"'";
        if (0 != (HTuple(HTuple(int(hv_AnomalyRegionGroundTruthExists==HTuple("false"))).TupleAnd(int(hv_AnomalyLabelIDGroundTruth==1))).TupleAnd(hv_ShowGroundTruthAnomalyRegions)))
        {
          hv_Text[hv_Text.TupleLength()] = "";
          hv_Text[hv_Text.TupleLength()] = "No segmentation ground truth found";
        }
        hv_Text[hv_Text.TupleLength()] = "";
        hv_Text[hv_Text.TupleLength()] = "---------------";
        hv_Text[hv_Text.TupleLength()] = "Results ";
        hv_Text[hv_Text.TupleLength()] = "---------------";
        if (0 != (int(hv_AnomalyClassID==1)))
        {
          hv_Text[hv_Text.TupleLength()] = hv_AnomalyClassID+" : 'nok'";
        }
        else if (0 != (int(hv_AnomalyClassID==0)))
        {
          hv_Text[hv_Text.TupleLength()] = hv_AnomalyClassID+" : 'ok'";
        }
        else
        {
          hv_Text[hv_Text.TupleLength()] = "No classification result found";
        }
        if (0 != (HTuple(int(hv_AnomalyRegionThreshold==-1)).TupleAnd(int((hv_ResultKeys.TupleFind("anomaly_region"))==-1))))
        {
          hv_Text[hv_Text.TupleLength()] = "";
          hv_Text[hv_Text.TupleLength()] = "No segmentation result found";
        }
        hv_Text[hv_Text.TupleLength()] = "";
        hv_Text[hv_Text.TupleLength()] = "Anomaly score: "+(hv_AnomalyScore.TupleString(".2f"));
        hv_Text[hv_Text.TupleLength()] = "";
        if (0 != (HTuple(int(hv_AnomalyClassThresholdDisplay!=-1)).TupleOr(int(hv_AnomalyRegionThresholdDisplay!=-1))))
        {
          hv_Text[hv_Text.TupleLength()] = "---------------";
          hv_Text[hv_Text.TupleLength()] = "Thresholds ";
          hv_Text[hv_Text.TupleLength()] = "---------------";
        }
        //
        if (0 != (int(hv_AnomalyClassThresholdDisplay!=-1)))
        {
          hv_Text[hv_Text.TupleLength()] = "Classification: "+(hv_AnomalyClassThresholdDisplay.TupleString(".2f"));
          hv_Text[hv_Text.TupleLength()] = "";
        }
        if (0 != (int(hv_AnomalyRegionThresholdDisplay!=-1)))
        {
          hv_Text[hv_Text.TupleLength()] = "Segmentation: "+(hv_AnomalyRegionThresholdDisplay.TupleString(".2f"));
          hv_Text[hv_Text.TupleLength()] = "";
        }
        //Get or open next child window
        get_child_window(hv_HeightImage, hv_Font, hv_FontSize, hv_Text, hv_PrevWindowCoordinates, 
            hv_WindowHandleDict, HTuple(hv_KeysForDisplay[hv_KeyIndex]), &hv_WindowImageRatio, 
            &hv_PrevWindowCoordinates);
        TupleGenConst(hv_Text.TupleLength(), "white", &hv_LineColors);
        hv_ResultColorOffset = 10;
        if (0 != (HTuple(HTuple(int(hv_AnomalyRegionGroundTruthExists==HTuple("false"))).TupleAnd(int(hv_AnomalyLabelIDGroundTruth==1))).TupleAnd(hv_ShowGroundTruthAnomalyRegions)))
        {
          hv_ResultColorOffset += 2;
        }
        hv_LineColors[hv_ResultColorOffset] = hv_PredictionColor;
        if (0 != hv_WindowHandle)
            DispText(*hv_WindowHandle, hv_Text, "window", "top", "left",
                hv_LineColors, "box", "false");
        else if (HDevWindowStack::IsOpen())
          DispText(HDevWindowStack::GetActive(),hv_Text, "window", "top", "left", 
              hv_LineColors, "box", "false");
      }
    }
    else if (0 != (int(HTuple(hv_KeysForDisplay[hv_KeyIndex])==HTuple("anomaly_image"))))
    {
      //
      //Image.
      get_image(&ho_Image, hv_SampleKeys, hv_DLSample);
      //
      get_anomaly_result(&ho_AnomalyImage, &ho_AnomalyRegion, hv_DLResult, hv_ResultKeys, 
          hv_AnomalyClassificationThreshold, hv_AnomalyRegionThreshold, &hv_AnomalyScore, 
          &hv_AnomalyClassID, &hv_AnomalyClassThresholdDisplay, &hv_AnomalyRegionThresholdDisplay);
      //
      //Read in input image.
      GetDictObject(&ho_Image, hv_DLSample, "image");
      //Add the anomaly image to the input image.
      add_colormap_to_image(ho_AnomalyImage, ho_Image, &ho_AnomalyImage, hv_HeatmapColorScheme);
      //
      //
      if (0 != hv_WindowHandle)
          DispObj(ho_AnomalyImage, *hv_WindowHandle);
      else
      {
          //Get or open next window.
          GetImageSize(ho_Image, &hv_WidthImage, &hv_HeightImage);
          get_next_window(hv_Font, hv_FontSize, hv_ShowBottomDesc, hv_WidthImage, hv_HeightImage,
              0, hv_ScaleWindows, hv_ThresholdWidth, hv_PrevWindowCoordinates, hv_WindowHandleDict,
              HTuple(hv_KeysForDisplay[hv_KeyIndex]), &hv_CurrentWindowHandle, &hv_WindowImageRatio,
              &hv_PrevWindowCoordinates);

          if (HDevWindowStack::IsOpen())
              DispObj(ho_AnomalyImage, HDevWindowStack::GetActive());
      }
      if (0 != hv_ShowBottomDesc)
      {
          hv_Text = "Anomaly image " + hv_ImageIDStringBraces;
          if (0 != hv_WindowHandle)
              DispText(*hv_WindowHandle, hv_Text, "window", "bottom", "left",
                  "white", "box", "false");
        else if (HDevWindowStack::IsOpen())
          DispText(HDevWindowStack::GetActive(),hv_Text, "window", "bottom", "left", 
              "white", "box", "false");
      }
      //Display the legend.
      if (0 != hv_ShowLegend)
      {
        hv_Text[hv_Text.TupleLength()] = "";
        if (0 != (int(hv_AnomalyClassID==1)))
        {
          hv_Text[hv_Text.TupleLength()] = hv_AnomalyClassID+" : 'nok'";
        }
        else if (0 != (int(hv_AnomalyClassID==0)))
        {
          hv_Text[hv_Text.TupleLength()] = hv_AnomalyClassID+" : 'ok'";
        }
        else
        {
          hv_Text[hv_Text.TupleLength()] = "No classification result found";
        }
        hv_Text[hv_Text.TupleLength()] = "";
        hv_Text[hv_Text.TupleLength()] = "Anomaly score: "+(hv_AnomalyScore.TupleString(".2f"));
        //Get or open next child window
        get_child_window(hv_HeightImage, hv_Font, hv_FontSize, hv_Text, hv_PrevWindowCoordinates, 
            hv_WindowHandleDict, HTuple(hv_KeysForDisplay[hv_KeyIndex]), &hv_WindowImageRatio, 
            &hv_PrevWindowCoordinates);
        if (0 != hv_WindowHandle)
            DispText(*hv_WindowHandle, hv_Text, "window", "top", "left",
                "white", "box", "false");
        else if (HDevWindowStack::IsOpen())
          DispText(HDevWindowStack::GetActive(),hv_Text, "window", "top", "left", 
              "white", "box", "false");
      }
    }
    else if (0 != (int(HTuple(hv_KeysForDisplay[hv_KeyIndex])==HTuple("classification_ground_truth"))))
    {
      //
      //Ground truth classification image and class label.
      get_image(&ho_Image, hv_SampleKeys, hv_DLSample);
      //
      get_classification_ground_truth(hv_SampleKeys, hv_DLSample, &hv_ClassificationLabelIDGroundTruth);
      //
      //Get or open next window.
      GetImageSize(ho_Image, &hv_WidthImage, &hv_HeightImage);
      get_next_window(hv_Font, hv_FontSize, hv_ShowBottomDesc, hv_WidthImage, hv_HeightImage, 
          0, hv_ScaleWindows, hv_ThresholdWidth, hv_PrevWindowCoordinates, hv_WindowHandleDict, 
          HTuple(hv_KeysForDisplay[hv_KeyIndex]), &hv_CurrentWindowHandle, &hv_WindowImageRatio, 
          &hv_PrevWindowCoordinates);
      //
      //Visualization.
      if (HDevWindowStack::IsOpen())
        DispObj(ho_Image, HDevWindowStack::GetActive());
      //
      if (0 != hv_ShowClassificationIDs)
      {
        hv_Text = "GT label ID: "+hv_ClassificationLabelIDGroundTruth;
        if (0 != hv_WindowHandle)
            DispText(*hv_WindowHandle, hv_Text, "window", "top", "left",
                "white", "box", "false");
        else if (HDevWindowStack::IsOpen())
          DispText(HDevWindowStack::GetActive(),hv_Text, "window", "top", "left", 
              "white", "box", "false");
      }
      //
      if (0 != hv_ShowBottomDesc)
      {
        hv_Text = "Ground truth classification "+hv_ImageIDStringBraces;
        if (0 != hv_WindowHandle)
            DispText(*hv_WindowHandle, hv_Text, "window", "bottom", "left",
                "white", "box", "false");
        else if (HDevWindowStack::IsOpen())
          DispText(HDevWindowStack::GetActive(),hv_Text, "window", "bottom", "left", 
              "white", "box", "false");
      }
      //
      //Display the legend.
      if (0 != hv_ShowLegend)
      {
        hv_Text = "Ground truth class ID "+hv_ImageIDStringBraces;
        hv_Text = hv_Text.TupleConcat(HTuple(hv_ClassesLegend[hv_ClassificationLabelIDGroundTruth]));
        //
        //Get or open next child window
        get_child_window(hv_HeightImage, hv_Font, hv_FontSize, hv_Text, hv_PrevWindowCoordinates, 
            hv_WindowHandleDict, HTuple(hv_KeysForDisplay[hv_KeyIndex]), &hv_WindowImageRatio, 
            &hv_PrevWindowCoordinates);
        if (0 != hv_WindowHandle)
            DispText(*hv_WindowHandle, hv_Text, "window", "top", "left",
                HTuple("white").TupleConcat(HTuple(hv_Colors[hv_ClassificationLabelIDGroundTruth])),
                "box", "false");
        else if (HDevWindowStack::IsOpen())
            DispText(HDevWindowStack::GetActive(),hv_Text, "window", "top", "left", 
              HTuple("white").TupleConcat(HTuple(hv_Colors[hv_ClassificationLabelIDGroundTruth])), 
              "box", "false");
      }
    }
    else if (0 != (int(HTuple(hv_KeysForDisplay[hv_KeyIndex])==HTuple("classification_result"))))
    {
      //
      //Ground truth classification image.
      get_image(&ho_Image, hv_SampleKeys, hv_DLSample);
      //
      get_classification_result(hv_ResultKeys, hv_DLResult, &hv_ClassificationLabelIDResult);
      //
      //Get or open next window.
      GetImageSize(ho_Image, &hv_WidthImage, &hv_HeightImage);
      get_next_window(hv_Font, hv_FontSize, hv_ShowBottomDesc, hv_WidthImage, hv_HeightImage, 
          0, hv_ScaleWindows, hv_ThresholdWidth, hv_PrevWindowCoordinates, hv_WindowHandleDict, 
          HTuple(hv_KeysForDisplay[hv_KeyIndex]), &hv_CurrentWindowHandle, &hv_WindowImageRatio, 
          &hv_PrevWindowCoordinates);
      //
      //Visualization.
      if (0 != hv_WindowHandle)
          DispObj(ho_Image, *hv_WindowHandle);
      if (HDevWindowStack::IsOpen())
          DispObj(ho_Image, HDevWindowStack::GetActive());
      //
      //Display the class IDs.
      if (0 != hv_ShowClassificationIDs)
      {
        GetDictTuple(hv_WindowHandleDict, "meta_information", &hv_MetaInfo);
        GetDictTuple(hv_MetaInfo, "classification_result_margin_bottom", &hv_MarginBottom);
        GetDictTuple(hv_MetaInfo, "classification_result_window_coordinates", &hv_WindowCoordinates);
        hv_CurrentWindowHeight = HTuple(hv_WindowCoordinates[3])-HTuple(hv_WindowCoordinates[0]);
        GetFontExtents(hv_CurrentWindowHandle, &hv__, &hv__, &hv__, &hv_MaxHeight);
        hv_Text = "Result class ID: "+hv_ClassificationLabelIDResult;
        if (0 != hv_ShowBottomDesc)
        {
            if (0 != hv_WindowHandle)
                DispText(*hv_WindowHandle, hv_Text, "window", hv_CurrentWindowHeight - ((hv_MarginBottom + hv_MaxHeight) + 10),
                    "left", "white", "box", "false");
            else if (HDevWindowStack::IsOpen())
                DispText(HDevWindowStack::GetActive(),hv_Text, "window", hv_CurrentWindowHeight-((hv_MarginBottom+hv_MaxHeight)+10), 
                  "left", "white", "box", "false");
        }
        else
        {
            if (0 != hv_WindowHandle)
                DispText(*hv_WindowHandle, hv_Text, "window", "bottom", "left",
                    "white", "box", "false");
            else if (HDevWindowStack::IsOpen())
                DispText(HDevWindowStack::GetActive(),hv_Text, "window", "bottom", "left", 
                    "white", "box", "false");
        }
      }
      //
      if (0 != hv_ShowBottomDesc)
      {
        hv_Text = "Result classification "+hv_ImageIDStringBraces;
        if (0 != hv_WindowHandle)
            DispText(*hv_WindowHandle, hv_Text, "window", "bottom", "left",
                "white", "box", "false");
        else if (HDevWindowStack::IsOpen())
            DispText(HDevWindowStack::GetActive(),hv_Text, "window", "bottom", "left", 
                "white", "box", "false");
      }
      //
      //Display the legend.
      if (0 != hv_ShowLegend)
      {
        hv_Text = "Result class ID "+hv_ImageIDStringBraces;
        if (0 != (int(hv_ClassificationLabelIDResult==HTuple())))
        {
          hv_Text[hv_Text.TupleLength()] = "No classification result is given!";
        }
        else
        {
          hv_Text = hv_Text.TupleConcat(HTuple(hv_ClassesLegend[hv_ClassificationLabelIDResult]));
        }
        //
        //Get or open next child window
        get_child_window(hv_HeightImage, hv_Font, hv_FontSize, hv_Text, hv_PrevWindowCoordinates, 
            hv_WindowHandleDict, HTuple(hv_KeysForDisplay[hv_KeyIndex]), &hv_WindowImageRatio, 
            &hv_PrevWindowCoordinates);
        if (0 != hv_WindowHandle)
            DispText(*hv_WindowHandle, hv_Text, "window", "top", "left",
                HTuple("white").TupleConcat(HTuple(hv_Colors[hv_ClassificationLabelIDResult])),
                "box", "false");
        else if (HDevWindowStack::IsOpen())
          DispText(HDevWindowStack::GetActive(),hv_Text, "window", "top", "left", 
              HTuple("white").TupleConcat(HTuple(hv_Colors[hv_ClassificationLabelIDResult])), 
              "box", "false");
      }
    }
    else if (0 != (int(HTuple(hv_KeysForDisplay[hv_KeyIndex])==HTuple("classification_both"))))
    {
      //
      //Ground truth and result classification image.
      get_image(&ho_Image, hv_SampleKeys, hv_DLSample);
      //
      get_classification_ground_truth(hv_SampleKeys, hv_DLSample, &hv_ClassificationLabelIDGroundTruth);
      get_classification_result(hv_ResultKeys, hv_DLResult, &hv_ClassificationLabelIDResult);
      //
      //Get or open next window.
      GetImageSize(ho_Image, &hv_WidthImage, &hv_HeightImage);
      get_next_window(hv_Font, hv_FontSize, hv_ShowBottomDesc, hv_WidthImage, hv_HeightImage, 
          0, hv_ScaleWindows, hv_ThresholdWidth, hv_PrevWindowCoordinates, hv_WindowHandleDict, 
          HTuple(hv_KeysForDisplay[hv_KeyIndex]), &hv_CurrentWindowHandle, &hv_WindowImageRatio, 
          &hv_PrevWindowCoordinates);
      //
      //Visualization.
      //
      //Get prediction color.
      hv_PredictionColor = "white";
      if (0 != (int(hv_ClassificationLabelIDGroundTruth==hv_ClassificationLabelIDResult)))
      {
        hv_PredictionText = "Correct";
        hv_PredictionColor = "green";
      }
      else
      {
        hv_PredictionText = "Wrong";
        hv_PredictionColor = "red";
      }
      //
      //Generate prediction color frame and show image.
      if (0 != hv_ShowClassificationColorFrame)
      {
        //Create a frame with line width 7 that is completely displayed in the window.
        hv_BoarderOffset = 7/2.;
        GetDictTuple(hv_WindowHandleDict, "meta_information", &hv_MetaInfo);
        GetDictTuple(hv_MetaInfo, "classification_both_window_image_ratio_height", 
            &hv_WindowImageRatioHeight);
        GetDictTuple(hv_MetaInfo, "classification_both_window_image_ratio_width", 
            &hv_WindowImageRatioWidth);
        hv_BoarderOffsetRow = hv_BoarderOffset/hv_WindowImageRatioHeight;
        hv_BoarderOffsetCol = hv_BoarderOffset/hv_WindowImageRatioWidth;
        GenContourPolygonXld(&ho_PredictionColorFrame, ((((hv_BoarderOffsetRow-0.5).TupleConcat(hv_BoarderOffsetRow-0.5)).TupleConcat((hv_HeightImage+0.5)-hv_BoarderOffsetRow)).TupleConcat((hv_HeightImage+0.5)-hv_BoarderOffsetRow)).TupleConcat(hv_BoarderOffsetRow-0.5), 
            ((((hv_BoarderOffsetCol-0.5).TupleConcat((hv_WidthImage+0.5)-hv_BoarderOffsetCol)).TupleConcat((hv_WidthImage+0.5)-hv_BoarderOffsetCol)).TupleConcat(hv_BoarderOffsetCol-0.5)).TupleConcat(hv_BoarderOffsetCol-0.5));
        if (0 != hv_WindowHandle)
        {
            SetLineWidth(*hv_WindowHandle, 7);
            SetColor(*hv_WindowHandle, hv_PredictionColor);
            DispObj(ho_Image, *hv_WindowHandle);
            DispObj(ho_PredictionColorFrame, *hv_WindowHandle);
        }
        else {
            if (HDevWindowStack::IsOpen())
                SetLineWidth(HDevWindowStack::GetActive(), 7);
            if (HDevWindowStack::IsOpen())
                SetColor(HDevWindowStack::GetActive(), hv_PredictionColor);
            if (HDevWindowStack::IsOpen())
                DispObj(ho_Image, HDevWindowStack::GetActive());
            if (HDevWindowStack::IsOpen())
                DispObj(ho_PredictionColorFrame, HDevWindowStack::GetActive());
        }
      }
      else
      {
          if (0 != hv_WindowHandle)
              DispObj(ho_Image, *hv_WindowHandle);
          else if (HDevWindowStack::IsOpen())
            DispObj(ho_Image, HDevWindowStack::GetActive());
      }
      //
      if (0 != hv_ShowClassificationIDs)
      {
        GetDictTuple(hv_WindowHandleDict, "meta_information", &hv_MetaInfo);
        GetDictTuple(hv_MetaInfo, HTuple(hv_KeysForDisplay[hv_KeyIndex])+"_margin_bottom", 
            &hv_MarginBottom);
        GetDictTuple(hv_MetaInfo, HTuple(hv_KeysForDisplay[hv_KeyIndex])+"_window_coordinates", 
            &hv_WindowCoordinates);
        hv_CurrentWindowHeight = HTuple(hv_WindowCoordinates[3])-HTuple(hv_WindowCoordinates[0]);
        GetFontExtents(hv_CurrentWindowHandle, &hv__, &hv__, &hv__, &hv_MaxHeight);
        hv_Text = "GT label ID: "+hv_ClassificationLabelIDGroundTruth;
        if (HDevWindowStack::IsOpen())
          DispText(HDevWindowStack::GetActive(),hv_Text, "window", "top", "left", 
              "white", "box", "false");
        hv_Text = "Result class ID: "+hv_ClassificationLabelIDResult;
        if (0 != hv_ShowBottomDesc)
        {
            if (0 != hv_WindowHandle)
                DispText(*hv_WindowHandle, hv_Text, "window", hv_CurrentWindowHeight - ((hv_MarginBottom + hv_MaxHeight) + 10),
                    "left", "white", "box", "false");
            else if (HDevWindowStack::IsOpen())
                DispText(HDevWindowStack::GetActive(),hv_Text, "window", hv_CurrentWindowHeight-((hv_MarginBottom+hv_MaxHeight)+10), 
                    "left", "white", "box", "false");
        }
        else
        {
            if (0 != hv_WindowHandle)
                DispText(*hv_WindowHandle, hv_Text, "window", "bottom", "left",
                    "white", "box", "false");
            else if (HDevWindowStack::IsOpen())
                DispText(HDevWindowStack::GetActive(),hv_Text, "window", "bottom", "left", 
                    "white", "box", "false");
        }
      }
      //
      if (0 != hv_ShowBottomDesc)
      {
        hv_Text = "Result/Ground truth classification "+hv_ImageIDStringBraces;
        if (0 != hv_WindowHandle)
            DispText(*hv_WindowHandle, hv_Text, "window", "bottom", "left",
                "white", "box", "false");
        else if (HDevWindowStack::IsOpen())
            DispText(HDevWindowStack::GetActive(),hv_Text, "window", "bottom", "left", 
              "white", "box", "false");
      }
      //
      //Display the legend.
      if (0 != hv_ShowLegend)
      {
        hv_Text = "Ground truth class ID "+hv_ImageIDStringBraces;
        hv_Text[hv_Text.TupleLength()] = HTuple(hv_ClassesLegend[hv_ClassificationLabelIDGroundTruth]);
        hv_Text[hv_Text.TupleLength()] = "";
        hv_Text[hv_Text.TupleLength()] = "";
        hv_Text[hv_Text.TupleLength()] = "Result class ID";
        if (0 != (int(hv_ClassificationLabelIDResult==HTuple())))
        {
          hv_Text[hv_Text.TupleLength()] = "No classification result is given!";
        }
        else
        {
          hv_Text[hv_Text.TupleLength()] = HTuple(hv_ClassesLegend[hv_ClassificationLabelIDResult]);
          hv_Text[hv_Text.TupleLength()] = "";
          hv_Text[hv_Text.TupleLength()] = "";
          hv_Text[hv_Text.TupleLength()] = "Prediction ";
          hv_Text[hv_Text.TupleLength()] = hv_PredictionText;
        }
        //
        //Get or open next child window.
        get_child_window(hv_HeightImage, hv_Font, hv_FontSize, hv_Text, hv_PrevWindowCoordinates, 
            hv_WindowHandleDict, HTuple(hv_KeysForDisplay[hv_KeyIndex]), &hv_WindowImageRatio, 
            &hv_PrevWindowCoordinates);
        TupleGenConst(hv_Text.TupleLength(), "white", &hv_LineColors);
        hv_LineColors[1] = HTuple(hv_Colors[hv_ClassificationLabelIDGroundTruth]);
        if (0 != (int(hv_ClassificationLabelIDResult!=HTuple())))
        {
          hv_LineColors[5] = HTuple(hv_Colors[hv_ClassificationLabelIDResult]);
          hv_LineColors[9] = hv_PredictionColor;
        }
        if (0 != hv_WindowHandle)
            DispText(*hv_WindowHandle, hv_Text, "window", "top", "left",
                hv_LineColors, "box", "false");
        else if (HDevWindowStack::IsOpen())
            DispText(HDevWindowStack::GetActive(),hv_Text, "window", "top", "left", 
                hv_LineColors, "box", "false");
      }
      //
    }
    else if (0 != (HTuple(int(HTuple(hv_KeysForDisplay[hv_KeyIndex])==HTuple("heatmap_grad_cam"))).TupleOr(int(HTuple(hv_KeysForDisplay[hv_KeyIndex])==HTuple("heatmap_confidence_based")))))
    {
      //
      //Display the heatmap image (method 'heatmap_grad_cam' or 'heatmap_confidence_based')
      //in the selected color scheme.
      //Retrieve heatmap image, inferred image, and inference results.
      hv_SelectedHeatmapMethod = HTuple(hv_KeysForDisplay[hv_KeyIndex]);
      if (0 != (HTuple(int((hv_ResultKeys.TupleFind("heatmap_grad_cam"))!=-1)).TupleOr(int((hv_ResultKeys.TupleFind("heatmap_confidence_based"))!=-1))))
      {
        if (0 != (int(hv_SelectedHeatmapMethod==HTuple("heatmap_grad_cam"))))
        {
          GetDictTuple(hv_DLResult, "heatmap_grad_cam", &hv_DictHeatmap);
          hv_MethodName = "Grad-CAM";
        }
        else
        {
          GetDictTuple(hv_DLResult, "heatmap_confidence_based", &hv_DictHeatmap);
          hv_MethodName = "Confidence based";
        }
        GetDictParam(hv_DictHeatmap, "keys", HTuple(), &hv_HeatmapKeys);
        hv_HeatmapImageName = hv_HeatmapKeys.TupleRegexpSelect("heatmap_image_class_[0-9]*");
        hv_TargetClassID = hv_HeatmapImageName.TupleRegexpMatch("heatmap_image_class_([0-9]+)$");
        GetDictObject(&ho_ImageHeatmap, hv_DictHeatmap, hv_HeatmapImageName);
      }
      else
      {
        throw HException("Heatmap image could not be found in DLResult.");
      }
      //
      if (0 != (int(hv_SelectedHeatmapMethod==HTuple("heatmap_grad_cam"))))
      {
        //Read in input image.
        GetDictObject(&ho_Image, hv_DLSample, "image");
        //Add the heatmap to the input image.
        add_colormap_to_image(ho_ImageHeatmap, ho_Image, &ho_ImageHeatmap, hv_HeatmapColorScheme);
      }
      //
      //Get or open next window.
      GetImageSize(ho_ImageHeatmap, &hv_WidthImage, &hv_HeightImage);
      get_next_window(hv_Font, hv_FontSize, hv_ShowBottomDesc, hv_WidthImage, hv_HeightImage, 
          0, hv_ScaleWindows, hv_ThresholdWidth, hv_PrevWindowCoordinates, hv_WindowHandleDict, 
          HTuple(hv_KeysForDisplay[hv_KeyIndex]), &hv_CurrentWindowHandle, &hv_WindowImageRatio, 
          &hv_PrevWindowCoordinates);
      //
      if (0 != hv_WindowHandle)
          DispObj(ho_ImageHeatmap, *hv_WindowHandle);
      else if (HDevWindowStack::IsOpen())
          DispObj(ho_ImageHeatmap, HDevWindowStack::GetActive());
      if (0 != hv_ShowBottomDesc)
      {
        hv_Text = "Classification heatmap "+hv_ImageIDStringBraces;
        if (0 != hv_WindowHandle)
            DispText(*hv_WindowHandle, hv_Text, "window", "bottom", "left",
                "white", "box", "false");
        else if (HDevWindowStack::IsOpen())
            DispText(HDevWindowStack::GetActive(),hv_Text, "window", "bottom", "left", 
              "white", "box", "false");
      }
      //
      //Display the legend.
      if (0 != hv_ShowLegend)
      {
        GetDictTuple(hv_DLResult, "classification_class_names", &hv_ClassNames);
        GetDictTuple(hv_DLResult, "classification_class_ids", &hv_ClassIDs);
        GetDictTuple(hv_DLResult, "classification_confidences", &hv_Confidences);
        if (0 != (int(hv_SelectedHeatmapMethod==HTuple("heatmap_confidence_based"))))
        {
          GetDictTuple(hv_DictHeatmap, "classification_heatmap_maxdeviation", &hv_MaxDeviation);
        }
        hv_ClassificationLabelNameResult = ((const HTuple&)hv_ClassNames)[0];
        hv_ClassificationLabelIDResult = ((const HTuple&)hv_ClassIDs)[0];
        hv_TargetClassConfidence = HTuple(hv_Confidences[hv_ClassIDs.TupleFind(hv_TargetClassID.TupleNumber())]);
        hv_Text = "--------- ";
        hv_Text[hv_Text.TupleLength()] = "Image ";
        hv_Text[hv_Text.TupleLength()] = "--------- ";
        hv_Text[hv_Text.TupleLength()] = "";
        if (0 != (int((hv_SampleKeys.TupleFind("image_label_id"))!=-1)))
        {
          GetDictTuple(hv_DLSample, "image_label_id", &hv_ClassificationLabelIDGroundTruth);
          GetDictTuple(hv_DLDatasetInfo, "class_names", &hv_ClassificationLabelNamesGroundTruth);
          //Get prediction color.
          if (0 != (int(hv_ClassificationLabelIDGroundTruth==hv_ClassificationLabelIDResult)))
          {
            hv_PredictionColor = "green";
          }
          else
          {
            hv_PredictionColor = "red";
          }
          hv_Text[hv_Text.TupleLength()] = "Ground truth class: ";
          hv_Text[hv_Text.TupleLength()] = HTuple(hv_ClassificationLabelNamesGroundTruth[hv_ClassificationLabelIDGroundTruth]);
          hv_Text[hv_Text.TupleLength()] = "";
        }
        hv_Text[hv_Text.TupleLength()] = "Predicted class: ";
        hv_Text[hv_Text.TupleLength()] = hv_ClassificationLabelNameResult;
        hv_Text[hv_Text.TupleLength()] = "";
        hv_Text[hv_Text.TupleLength()] = "Confidence: "+(HTuple(hv_Confidences[0]).TupleString(".2f"));
        hv_Text[hv_Text.TupleLength()] = "";
        hv_Text[hv_Text.TupleLength()] = "--------- ";
        hv_Text[hv_Text.TupleLength()] = "Heatmap ";
        hv_Text[hv_Text.TupleLength()] = "--------- ";
        hv_Text[hv_Text.TupleLength()] = "";
        hv_Text[hv_Text.TupleLength()] = "Method: "+hv_MethodName;
        hv_Text[hv_Text.TupleLength()] = "Target class: "+hv_TargetClassID;
        hv_Text[hv_Text.TupleLength()] = "";
        hv_Text[hv_Text.TupleLength()] = "Target class confidence: "+(hv_TargetClassConfidence.TupleString(".2f"));
        if (0 != (int(hv_SelectedHeatmapMethod==HTuple("heatmap_confidence_based"))))
        {
          hv_Text[hv_Text.TupleLength()] = "Maximum deviation: "+(hv_MaxDeviation.TupleString(".2f"));
        }
        //
        //Get or open next child window
        get_child_window(hv_HeightImage, hv_Font, hv_FontSize, hv_Text, hv_PrevWindowCoordinates, 
            hv_WindowHandleDict, HTuple(hv_KeysForDisplay[hv_KeyIndex]), &hv_WindowImageRatio, 
            &hv_PrevWindowCoordinates);
        TupleGenConst(hv_Text.TupleLength(), "white", &hv_LineColors);
        if (0 != (int((hv_SampleKeys.TupleFind("image_label_id"))!=-1)))
        {
          hv_LineColors[8] = hv_PredictionColor;
        }
        if (0 != hv_WindowHandle)
            DispText(*hv_WindowHandle, hv_Text, "window", "top", "left",
                hv_LineColors, "box", "false");
        else if (HDevWindowStack::IsOpen())
          DispText(HDevWindowStack::GetActive(),hv_Text, "window", "top", "left", 
              hv_LineColors, "box", "false");
      }
      //
    }
    else if (0 != (int(HTuple(hv_KeysForDisplay[hv_KeyIndex])==HTuple("segmentation_weight"))))
    {
      //
      //Weight image.
      get_weight_image(&ho_ImageWeight, hv_SampleKeys, hv_DLSample);
      //
      //Get or open next window.
      GetImageSize(ho_ImageWeight, &hv_WidthImage, &hv_HeightImage);
      get_next_window(hv_Font, hv_FontSize, hv_ShowBottomDesc, hv_WidthImage, hv_HeightImage, 
          0, hv_ScaleWindows, hv_ThresholdWidth, hv_PrevWindowCoordinates, hv_WindowHandleDict, 
          HTuple(hv_KeysForDisplay[hv_KeyIndex]), &hv_CurrentWindowHandle, &hv_WindowImageRatio, 
          &hv_PrevWindowCoordinates);
      //
      if (0 != hv_WindowHandle)
          DispObj(ho_ImageWeight, *hv_WindowHandle);
      else if (HDevWindowStack::IsOpen())
        DispObj(ho_ImageWeight, HDevWindowStack::GetActive());
      if (0 != hv_ShowBottomDesc)
      {
          if (0 != hv_WindowHandle)
              DispText(*hv_WindowHandle, "Weight image " + hv_ImageIDStringBraces,
                  "window", "bottom", "left", "white", "box", "false");
          else if (HDevWindowStack::IsOpen())
            DispText(HDevWindowStack::GetActive(),"Weight image "+hv_ImageIDStringBraces, 
              "window", "bottom", "left", "white", "box", "false");
      }
    }
    else if (0 != (int(HTuple(hv_KeysForDisplay[hv_KeyIndex])==HTuple("segmentation_confidence"))))
    {
      //
      //Segmentation confidences.
      get_confidence_image(&ho_ImageConfidence, hv_ResultKeys, hv_DLResult);
      //
      //Get or open next window.
      GetImageSize(ho_ImageConfidence, &hv_WidthImage, &hv_HeightImage);
      get_next_window(hv_Font, hv_FontSize, hv_ShowBottomDesc, hv_WidthImage, hv_HeightImage, 
          0, hv_ScaleWindows, hv_ThresholdWidth, hv_PrevWindowCoordinates, hv_WindowHandleDict, 
          HTuple(hv_KeysForDisplay[hv_KeyIndex]), &hv_CurrentWindowHandle, &hv_WindowImageRatio, 
          &hv_PrevWindowCoordinates);
      //
      if (0 != hv_WindowHandle)
          DispObj(ho_ImageConfidence, *hv_WindowHandle);
      if (HDevWindowStack::IsOpen())
        DispObj(ho_ImageConfidence, HDevWindowStack::GetActive());
      if (0 != hv_ShowBottomDesc)
      {
          if (0 != hv_WindowHandle)
              DispText(*hv_WindowHandle, "Confidence image " + hv_ImageIDStringBraces,
              "window", "bottom", "left", "white", "box", "false");
          else if (HDevWindowStack::IsOpen())
             DispText(HDevWindowStack::GetActive(),"Confidence image "+hv_ImageIDStringBraces, 
               "window", "bottom", "left", "white", "box", "false");
      }
    }
    else if (0 != (int(HTuple(hv_KeysForDisplay[hv_KeyIndex])==HTuple("bbox_ground_truth"))))
    {
      //
      //Sample bounding boxes on image.
      get_image(&ho_Image, hv_SampleKeys, hv_DLSample);
      //
      //Get or open next window.
      GetImageSize(ho_Image, &hv_WidthImage, &hv_HeightImage);
      get_next_window(hv_Font, hv_FontSize, hv_ShowBottomDesc, hv_WidthImage, hv_HeightImage, 
          0, hv_ScaleWindows, hv_ThresholdWidth, hv_PrevWindowCoordinates, hv_WindowHandleDict, 
          HTuple(hv_KeysForDisplay[hv_KeyIndex]), &hv_CurrentWindowHandle, &hv_WindowImageRatio, 
          &hv_PrevWindowCoordinates);
      //
      //Visualization.
      if (0 != hv_WindowHandle)
          DispObj(ho_Image, *hv_WindowHandle);
      else if (HDevWindowStack::IsOpen())
        DispObj(ho_Image, HDevWindowStack::GetActive());
      //
      dev_display_ground_truth_detection(hv_DLSample, hv_SampleKeys, hv_LineWidth, 
          hv_ClassIDs, hv_Colors, hv_BboxLabelColor, hv_WindowImageRatio, hv_BboxTextColor, 
          hv_ShowLabels, hv_ShowDirection, hv_CurrentWindowHandle, &hv_BboxIDs);
      hv_Text = "Ground truth "+hv_ImageIDStringBraces;
      if (0 != hv_ShowBottomDesc)
      {
          if (0 != hv_WindowHandle)
              DispText(*hv_WindowHandle, hv_Text, "window", "bottom", "left",
                  "white", "box", "false");
          else if (HDevWindowStack::IsOpen())
              DispText(HDevWindowStack::GetActive(),hv_Text, "window", "bottom", "left", 
                  "white", "box", "false");
      }
      //
      //Display the legend.
      if (0 != hv_ShowLegend)
      {
        hv_BboxColors = "white";
        if (0 != (hv_BboxIDs.TupleLength()))
        {
          hv_BboxIDsUniq = (hv_BboxIDs.TupleSort()).TupleUniq();
          hv_Text = hv_Text.TupleConcat(HTuple(hv_ClassesLegend[hv_BboxIDsUniq]));
          hv_BboxColors = hv_BboxColors.TupleConcat(HTuple(hv_Colors[hv_BboxIDsUniq]));
        }
        else
        {
          hv_Text = hv_Text.TupleConcat("No ground truth bounding boxes present.");
        }
        //
        //Get or open next child window.
        get_child_window(hv_HeightImage, hv_Font, hv_FontSize, hv_Text, hv_PrevWindowCoordinates, 
            hv_WindowHandleDict, HTuple(hv_KeysForDisplay[hv_KeyIndex]), &hv_WindowImageRatio, 
            &hv_PrevWindowCoordinates);
        if (0 != hv_WindowHandle)
            DispText(*hv_WindowHandle, hv_Text, "window", "top", "left",
                hv_BboxColors, "box", "false");
        else if (HDevWindowStack::IsOpen())
          DispText(HDevWindowStack::GetActive(),hv_Text, "window", "top", "left", 
              hv_BboxColors, "box", "false");
      }
    }
    else if (0 != (int(HTuple(hv_KeysForDisplay[hv_KeyIndex])==HTuple("bbox_result"))))
    {
      //
      //Result bounding boxes on image.
      get_image(&ho_Image, hv_SampleKeys, hv_DLSample);
      //
      //Get or open next window.
      GetImageSize(ho_Image, &hv_WidthImage, &hv_HeightImage);
      get_next_window(hv_Font, hv_FontSize, hv_ShowBottomDesc, hv_WidthImage, hv_HeightImage, 
          0, hv_ScaleWindows, hv_ThresholdWidth, hv_PrevWindowCoordinates, hv_WindowHandleDict, 
          HTuple(hv_KeysForDisplay[hv_KeyIndex]), &hv_CurrentWindowHandle, &hv_WindowImageRatio, 
          &hv_PrevWindowCoordinates);
      //
      //Visualization.
      if (0 != hv_WindowHandle)
          DispObj(ho_Image, *hv_WindowHandle);
      else if (HDevWindowStack::IsOpen())
        DispObj(ho_Image, HDevWindowStack::GetActive());
      //
      if (0 != (int((hv_ResultKeys.TupleFind("bbox_confidence"))!=-1)))
      {
        GetDictTuple(hv_DLResult, "bbox_confidence", &hv_BboxConfidences);
      }
      else
      {
        throw HException("Result bounding box data could not be found in DLResult.");
      }
      if (0 != hv_BboxDisplayConfidence)
      {
        hv_TextConf = (" ("+(hv_BboxConfidences.TupleString(".2f")))+")";
      }
      else
      {
        hv_TextConf = HTuple(hv_BboxConfidences.TupleLength(),"");
      }
      dev_display_result_detection(hv_DLResult, hv_ResultKeys, hv_LineWidth, hv_ClassIDs, 
          hv_TextConf, hv_Colors, hv_BboxLabelColor, hv_WindowImageRatio, "top", 
          hv_BboxTextColor, hv_ShowLabels, hv_ShowDirection, hv_CurrentWindowHandle, 
          &hv_BboxClassIndex);
      hv_Text = "Result bounding boxes "+hv_ImageIDStringBraces;
      if (0 != hv_ShowBottomDesc)
      {
          if (0 != hv_WindowHandle)
              DispText(*hv_WindowHandle, hv_Text, "window", "bottom", "left",
                  "white", "box", "false");
        else if (HDevWindowStack::IsOpen())
          DispText(HDevWindowStack::GetActive(),hv_Text, "window", "bottom", "left", 
              "white", "box", "false");
      }
      //
      //Display the legend.
      if (0 != hv_ShowLegend)
      {
        hv_BboxColorsResults = "white";
        if (0 != (int((hv_BboxClassIndex.TupleLength())>0)))
        {
          hv_BboxClassIndexUniq = (hv_BboxClassIndex.TupleSort()).TupleUniq();
          hv_Text = hv_Text.TupleConcat(HTuple(hv_ClassesLegend[hv_BboxClassIndexUniq]));
          hv_BboxColorsResults = hv_BboxColorsResults.TupleConcat(HTuple(hv_Colors[hv_BboxClassIndexUniq]));
        }
        else
        {
          hv_Text = hv_Text.TupleConcat("No result bounding boxes present.");
        }
        //
        //Get or open next child window.
        get_child_window(hv_HeightImage, hv_Font, hv_FontSize, hv_Text, hv_PrevWindowCoordinates, 
            hv_WindowHandleDict, HTuple(hv_KeysForDisplay[hv_KeyIndex]), &hv_WindowImageRatio, 
            &hv_PrevWindowCoordinates);
        if (0 != hv_WindowHandle)
            DispText(*hv_WindowHandle, hv_Text, "window", "top", "left",
                hv_BboxColorsResults, "box", "false");
        else if (HDevWindowStack::IsOpen())
          DispText(HDevWindowStack::GetActive(),hv_Text, "window", "top", "left", 
              hv_BboxColorsResults, "box", "false");
      }
    }
    else if (0 != (int(HTuple(hv_KeysForDisplay[hv_KeyIndex])==HTuple("bbox_both"))))
    {
      //
      //Ground truth and result bounding boxes on image.
      get_image(&ho_Image, hv_SampleKeys, hv_DLSample);
      //
      //Get or open next window.
      GetImageSize(ho_Image, &hv_WidthImage, &hv_HeightImage);
      get_next_window(hv_Font, hv_FontSize, hv_ShowBottomDesc, hv_WidthImage, hv_HeightImage, 
          0, hv_ScaleWindows, hv_ThresholdWidth, hv_PrevWindowCoordinates, hv_WindowHandleDict, 
          HTuple(hv_KeysForDisplay[hv_KeyIndex]), &hv_CurrentWindowHandle, &hv_WindowImageRatio, 
          &hv_PrevWindowCoordinates);
      //
      if (0 != hv_WindowHandle)
          DispObj(ho_Image, *hv_WindowHandle);
      else if (HDevWindowStack::IsOpen())
        DispObj(ho_Image, HDevWindowStack::GetActive());
      //
      //Visualization.
      dev_display_ground_truth_detection(hv_DLSample, hv_SampleKeys, hv_LineWidth, 
          hv_ClassIDs, hv_Colors, hv_BboxLabelColor, hv_WindowImageRatio, hv_BboxTextColor, 
          hv_ShowLabels, hv_ShowDirection, hv_CurrentWindowHandle, &hv_BboxLabelIndex);
      if (0 != (int((hv_ResultKeys.TupleFind("bbox_confidence"))!=-1)))
      {
        GetDictTuple(hv_DLResult, "bbox_confidence", &hv_BboxConfidences);
      }
      else
      {
        throw HException("Result bounding box data could not be found in DLResult.");
      }
      if (0 != hv_BboxDisplayConfidence)
      {
        hv_TextConf = (" ("+(hv_BboxConfidences.TupleString(".2f")))+")";
      }
      else
      {
        hv_TextConf = HTuple(hv_BboxConfidences.TupleLength(),"");
      }
      dev_display_result_detection(hv_DLResult, hv_ResultKeys, hv_LineWidth, hv_ClassIDs, 
          hv_TextConf, hv_Colors, hv_BboxLabelColor, hv_WindowImageRatio, "bottom", 
          hv_BboxTextColor, hv_ShowLabels, hv_ShowDirection, hv_CurrentWindowHandle, 
          &hv_BboxClassIndex);
      hv_Text = "Ground truth and result bounding boxes "+hv_ImageIDStringBraces;
      if (0 != hv_ShowBottomDesc)
      {
          if (0 != hv_WindowHandle)
              DispText(*hv_WindowHandle, hv_Text, "window", "bottom", "left",
                  "white", "box", "false");
        else if (HDevWindowStack::IsOpen())
          DispText(HDevWindowStack::GetActive(),hv_Text, "window", "bottom", "left", 
              "white", "box", "false");
      }
      hv_Text = "Ground truth and";
      hv_Text[hv_Text.TupleLength()] = "result bounding boxes "+hv_ImageIDStringBraces;
      //
      //Display the legend.
      if (0 != hv_ShowLegend)
      {
        hv_BboxColorsBoth.Clear();
        hv_BboxColorsBoth[0] = "white";
        hv_BboxColorsBoth[1] = "white";
        if (0 != (int(((hv_BboxClassIndex.TupleLength())+(hv_BboxLabelIndex.TupleLength()))>0)))
        {
          hv_BboxClassLabelIndexUniq = ((hv_BboxClassIndex.TupleConcat(hv_BboxLabelIndex)).TupleSort()).TupleUniq();
          hv_Text = hv_Text.TupleConcat(HTuple(hv_ClassesLegend[hv_BboxClassLabelIndexUniq]));
          hv_BboxColorsBoth = hv_BboxColorsBoth.TupleConcat(HTuple(hv_Colors[hv_BboxClassLabelIndexUniq]));
        }
        else
        {
          hv_Text = hv_Text.TupleConcat("No ground truth nor result bounding boxes present.");
        }
        //
        //Get or open next child window.
        get_child_window(hv_HeightImage, hv_Font, hv_FontSize, hv_Text, hv_PrevWindowCoordinates, 
            hv_WindowHandleDict, HTuple(hv_KeysForDisplay[hv_KeyIndex]), &hv_WindowImageRatio, 
            &hv_PrevWindowCoordinates);
        if (0 != hv_WindowHandle)
            DispText(*hv_WindowHandle, hv_Text, "window", "top", "left",
                hv_BboxColorsBoth, "box", "false");
        else if (HDevWindowStack::IsOpen())
          DispText(HDevWindowStack::GetActive(),hv_Text, "window", "top", "left", 
              hv_BboxColorsBoth, "box", "false");
      }
    }
    else if (0 != (int(HTuple(hv_KeysForDisplay[hv_KeyIndex])==HTuple("segmentation_image_ground_truth"))))
    {
      //
      //Ground truth segmentation image.
      get_image(&ho_Image, hv_SampleKeys, hv_DLSample);
      get_segmentation_image_ground_truth(&ho_SegmentationImagGroundTruth, hv_SampleKeys, 
          hv_DLSample);
      //
      //Get or open next window.
      GetImageSize(ho_Image, &hv_WidthImage, &hv_HeightImage);
      get_next_window(hv_Font, hv_FontSize, hv_ShowBottomDesc, hv_WidthImage, hv_HeightImage, 
          0, hv_ScaleWindows, hv_ThresholdWidth, hv_PrevWindowCoordinates, hv_WindowHandleDict, 
          HTuple(hv_KeysForDisplay[hv_KeyIndex]), &hv_CurrentWindowHandle, &hv_WindowImageRatio, 
          &hv_PrevWindowCoordinates);
      //
      //Visualization.
      if (0 != hv_WindowHandle)
      {
          DispObj(ho_Image, *hv_WindowHandle);
          //Display segmentation regions.
          hv_ColorsSegmentation = hv_Colors + hv_SegTransparency;
          GetDraw(hv_CurrentWindowHandle, &hv_DrawMode);
          SetDraw(*hv_WindowHandle, hv_SegDraw);
          GetLineWidth(hv_CurrentWindowHandle, &hv_Width);
          SetLineWidth(*hv_WindowHandle, hv_LineWidth);
          dev_display_segmentation_regions(ho_SegmentationImagGroundTruth, hv_ClassIDs,
              hv_ColorsSegmentation, hv_SegExcludeClassIDs, &hv_ImageClassIDs);
          SetDraw(*hv_WindowHandle, hv_DrawMode);
          SetLineWidth(*hv_WindowHandle, hv_Width.TupleInt());
          hv_Text = "Ground truth segmentation " + hv_ImageIDStringBraces;
          if (0 != hv_ShowBottomDesc)
          {
              DispText(*hv_WindowHandle, hv_Text, "window", "bottom", "left",
                      "white", "box", "false");
          }
          //
          //Display the legend.
          if (0 != hv_ShowLegend)
          {
              hv_ImageClassIDsUniq = (hv_ImageClassIDs.TupleSort()).TupleUniq();
              //Get Indices according to image class IDs.
              TupleGenConst(hv_ImageClassIDsUniq.TupleLength(), 0, &hv_ImageClassIDsIndices);
              {
                  HTuple end_val900 = (hv_ImageClassIDsUniq.TupleLength()) - 1;
                  HTuple step_val900 = 1;
                  for (hv_ImageClassIDsIndex = 0; hv_ImageClassIDsIndex.Continue(end_val900, step_val900); hv_ImageClassIDsIndex += step_val900)
                  {
                      hv_ImageClassIDsIndices[hv_ImageClassIDsIndex] = hv_ClassIDs.TupleFindFirst(HTuple(hv_ImageClassIDsUniq[hv_ImageClassIDsIndex]));
                  }
              }
              hv_Text = hv_Text.TupleConcat(HTuple(hv_ClassesLegend[hv_ImageClassIDsIndices]));
              //
              //Get or open next child window
              get_child_window(hv_HeightImage, hv_Font, hv_FontSize, hv_Text, hv_PrevWindowCoordinates,
                  hv_WindowHandleDict, HTuple(hv_KeysForDisplay[hv_KeyIndex]), &hv_WindowImageRatio,
                  &hv_PrevWindowCoordinates);
              DispText(*hv_WindowHandle, hv_Text, "window", "top", "left",
                      HTuple("white").TupleConcat(HTuple(hv_Colors[hv_ImageClassIDsIndices])),
                      "box", "false");
          }
      }
      else
      {
          if (HDevWindowStack::IsOpen())
              DispObj(ho_Image, HDevWindowStack::GetActive());
          //
          //Display segmentation regions.
          hv_ColorsSegmentation = hv_Colors + hv_SegTransparency;
          GetDraw(hv_CurrentWindowHandle, &hv_DrawMode);
          if (HDevWindowStack::IsOpen())
              SetDraw(HDevWindowStack::GetActive(), hv_SegDraw);
          GetLineWidth(hv_CurrentWindowHandle, &hv_Width);
          if (HDevWindowStack::IsOpen())
              SetLineWidth(HDevWindowStack::GetActive(), hv_LineWidth);
          dev_display_segmentation_regions(ho_SegmentationImagGroundTruth, hv_ClassIDs,
              hv_ColorsSegmentation, hv_SegExcludeClassIDs, &hv_ImageClassIDs);
          if (HDevWindowStack::IsOpen())
              SetDraw(HDevWindowStack::GetActive(), hv_DrawMode);
          if (HDevWindowStack::IsOpen())
              SetLineWidth(HDevWindowStack::GetActive(), hv_Width.TupleInt());
          hv_Text = "Ground truth segmentation " + hv_ImageIDStringBraces;
          if (0 != hv_ShowBottomDesc)
          {
              if (HDevWindowStack::IsOpen())
                  DispText(HDevWindowStack::GetActive(), hv_Text, "window", "bottom", "left",
                      "white", "box", "false");
          }
          //
          //Display the legend.
          if (0 != hv_ShowLegend)
          {
              hv_ImageClassIDsUniq = (hv_ImageClassIDs.TupleSort()).TupleUniq();
              //Get Indices according to image class IDs.
              TupleGenConst(hv_ImageClassIDsUniq.TupleLength(), 0, &hv_ImageClassIDsIndices);
              {
                  HTuple end_val900 = (hv_ImageClassIDsUniq.TupleLength()) - 1;
                  HTuple step_val900 = 1;
                  for (hv_ImageClassIDsIndex = 0; hv_ImageClassIDsIndex.Continue(end_val900, step_val900); hv_ImageClassIDsIndex += step_val900)
                  {
                      hv_ImageClassIDsIndices[hv_ImageClassIDsIndex] = hv_ClassIDs.TupleFindFirst(HTuple(hv_ImageClassIDsUniq[hv_ImageClassIDsIndex]));
                  }
              }
              hv_Text = hv_Text.TupleConcat(HTuple(hv_ClassesLegend[hv_ImageClassIDsIndices]));
              //
              //Get or open next child window
              get_child_window(hv_HeightImage, hv_Font, hv_FontSize, hv_Text, hv_PrevWindowCoordinates,
                  hv_WindowHandleDict, HTuple(hv_KeysForDisplay[hv_KeyIndex]), &hv_WindowImageRatio,
                  &hv_PrevWindowCoordinates);
              if (HDevWindowStack::IsOpen())
                  DispText(HDevWindowStack::GetActive(), hv_Text, "window", "top", "left",
                      HTuple("white").TupleConcat(HTuple(hv_Colors[hv_ImageClassIDsIndices])),
                      "box", "false");
          }
      }
    }
    else if (0 != (int(HTuple(hv_KeysForDisplay[hv_KeyIndex])==HTuple("segmentation_image_result"))))
    {
      //
      //Result segmentation on image.
      get_image(&ho_Image, hv_SampleKeys, hv_DLSample);
      get_segmentation_image_result(&ho_SegmentationImageResult, hv_ResultKeys, hv_DLResult);
      //
      //Get or open next window.
      GetImageSize(ho_Image, &hv_WidthImage, &hv_HeightImage);
      get_next_window(hv_Font, hv_FontSize, hv_ShowBottomDesc, hv_WidthImage, hv_HeightImage, 
          0, hv_ScaleWindows, hv_ThresholdWidth, hv_PrevWindowCoordinates, hv_WindowHandleDict, 
          HTuple(hv_KeysForDisplay[hv_KeyIndex]), &hv_CurrentWindowHandle, &hv_WindowImageRatio, 
          &hv_PrevWindowCoordinates);
      //
      //Visualization.
      if (0 != hv_WindowHandle)
      {
         DispObj(ho_Image, *hv_WindowHandle);
          //
          //Display result segmentation regions.
          hv_ColorsResults = hv_Colors + hv_SegTransparency;
          GetDraw(hv_CurrentWindowHandle, &hv_DrawMode);
          SetDraw(*hv_WindowHandle, hv_SegDraw);
          GetLineWidth(hv_CurrentWindowHandle, &hv_Width);
          SetLineWidth(*hv_WindowHandle, hv_LineWidth);
          dev_display_segmentation_regions(ho_SegmentationImageResult, hv_ClassIDs, hv_ColorsResults,
              hv_SegExcludeClassIDs, &hv_ImageClassIDs);
          SetDraw(*hv_WindowHandle, hv_DrawMode);
          SetLineWidth(*hv_WindowHandle, hv_Width.TupleInt());
          hv_Text = "Result segmentation " + hv_ImageIDStringBraces;
          if (0 != hv_ShowBottomDesc)
          {
              DispText(*hv_WindowHandle, hv_Text, "window", "bottom", "left",
                      "white", "box", "false");
          }
          //
          //Display the legend.
          if (0 != hv_ShowLegend)
          {
              hv_ImageClassIDsUniq = (hv_ImageClassIDs.TupleSort()).TupleUniq();
              //Get Indices according to image class IDs.
              TupleGenConst(hv_ImageClassIDsUniq.TupleLength(), 0, &hv_ImageClassIDsIndices);
              {
                  HTuple end_val941 = (hv_ImageClassIDsUniq.TupleLength()) - 1;
                  HTuple step_val941 = 1;
                  for (hv_ImageClassIDsIndex = 0; hv_ImageClassIDsIndex.Continue(end_val941, step_val941); hv_ImageClassIDsIndex += step_val941)
                  {
                      hv_ImageClassIDsIndices[hv_ImageClassIDsIndex] = hv_ClassIDs.TupleFindFirst(HTuple(hv_ImageClassIDsUniq[hv_ImageClassIDsIndex]));
                  }
              }
              hv_Text = hv_Text.TupleConcat(HTuple(hv_ClassesLegend[hv_ImageClassIDsIndices]));
              //
              //Get or open next child window.
              get_child_window(hv_HeightImage, hv_Font, hv_FontSize, hv_Text, hv_PrevWindowCoordinates,
                  hv_WindowHandleDict, HTuple(hv_KeysForDisplay[hv_KeyIndex]), &hv_WindowImageRatio,
                  &hv_PrevWindowCoordinates);
              DispText(*hv_WindowHandle, hv_Text, "window", "top", "left",
                      HTuple("white").TupleConcat(HTuple(hv_Colors[hv_ImageClassIDsIndices])),
                      "box", "false");
          }
      }
      else
      {
          if (HDevWindowStack::IsOpen())
              DispObj(ho_Image, HDevWindowStack::GetActive());
          //
          //Display result segmentation regions.
          hv_ColorsResults = hv_Colors + hv_SegTransparency;
          GetDraw(hv_CurrentWindowHandle, &hv_DrawMode);
          if (HDevWindowStack::IsOpen())
              SetDraw(HDevWindowStack::GetActive(), hv_SegDraw);
          GetLineWidth(hv_CurrentWindowHandle, &hv_Width);
          if (HDevWindowStack::IsOpen())
              SetLineWidth(HDevWindowStack::GetActive(), hv_LineWidth);
          dev_display_segmentation_regions(ho_SegmentationImageResult, hv_ClassIDs, hv_ColorsResults,
              hv_SegExcludeClassIDs, &hv_ImageClassIDs);
          if (HDevWindowStack::IsOpen())
              SetDraw(HDevWindowStack::GetActive(), hv_DrawMode);
          if (HDevWindowStack::IsOpen())
              SetLineWidth(HDevWindowStack::GetActive(), hv_Width.TupleInt());
          hv_Text = "Result segmentation " + hv_ImageIDStringBraces;
          if (0 != hv_ShowBottomDesc)
          {
              if (HDevWindowStack::IsOpen())
                  DispText(HDevWindowStack::GetActive(), hv_Text, "window", "bottom", "left",
                      "white", "box", "false");
          }
          //
          //Display the legend.
          if (0 != hv_ShowLegend)
          {
              hv_ImageClassIDsUniq = (hv_ImageClassIDs.TupleSort()).TupleUniq();
              //Get Indices according to image class IDs.
              TupleGenConst(hv_ImageClassIDsUniq.TupleLength(), 0, &hv_ImageClassIDsIndices);
              {
                  HTuple end_val941 = (hv_ImageClassIDsUniq.TupleLength()) - 1;
                  HTuple step_val941 = 1;
                  for (hv_ImageClassIDsIndex = 0; hv_ImageClassIDsIndex.Continue(end_val941, step_val941); hv_ImageClassIDsIndex += step_val941)
                  {
                      hv_ImageClassIDsIndices[hv_ImageClassIDsIndex] = hv_ClassIDs.TupleFindFirst(HTuple(hv_ImageClassIDsUniq[hv_ImageClassIDsIndex]));
                  }
              }
              hv_Text = hv_Text.TupleConcat(HTuple(hv_ClassesLegend[hv_ImageClassIDsIndices]));
              //
              //Get or open next child window.
              get_child_window(hv_HeightImage, hv_Font, hv_FontSize, hv_Text, hv_PrevWindowCoordinates,
                  hv_WindowHandleDict, HTuple(hv_KeysForDisplay[hv_KeyIndex]), &hv_WindowImageRatio,
                  &hv_PrevWindowCoordinates);
              if (HDevWindowStack::IsOpen())
                  DispText(HDevWindowStack::GetActive(), hv_Text, "window", "top", "left",
                      HTuple("white").TupleConcat(HTuple(hv_Colors[hv_ImageClassIDsIndices])),
                      "box", "false");
          }
      }
    }
    else if (0 != (int(HTuple(hv_KeysForDisplay[hv_KeyIndex])==HTuple("segmentation_image_both"))))
    {
      //
      //Ground truth and result segmentation on image.
      get_image(&ho_Image, hv_SampleKeys, hv_DLSample);
      get_segmentation_image_ground_truth(&ho_SegmentationImagGroundTruth, hv_SampleKeys, 
          hv_DLSample);
      get_segmentation_image_result(&ho_SegmentationImageResult, hv_ResultKeys, hv_DLResult);
      //
      //Get or open next window.
      GetImageSize(ho_Image, &hv_WidthImage, &hv_HeightImage);
      get_next_window(hv_Font, hv_FontSize, hv_ShowBottomDesc, hv_WidthImage, hv_HeightImage, 
          0, hv_ScaleWindows, hv_ThresholdWidth, hv_PrevWindowCoordinates, hv_WindowHandleDict, 
          HTuple(hv_KeysForDisplay[hv_KeyIndex]), &hv_CurrentWindowHandle, &hv_WindowImageRatio, 
          &hv_PrevWindowCoordinates);
      //
      //Visualization.
      if (0 != hv_WindowHandle)
      {

          DispObj(ho_Image, *hv_WindowHandle);
          //
          //Display regions.
          hv_ColorsResults = hv_Colors + hv_SegTransparency;
          SetDraw(*hv_WindowHandle, "margin");
          SetLineWidth(*hv_WindowHandle, 2);
          dev_display_segmentation_regions(ho_SegmentationImagGroundTruth, hv_ClassIDs,
              hv_ColorsResults, hv_SegExcludeClassIDs, &hv_GroundTruthIDs);
          SetLineWidth(*hv_WindowHandle, 6);
          dev_display_segmentation_regions(ho_SegmentationImageResult, hv_ClassIDs, hv_ColorsResults,
              hv_SegExcludeClassIDs, &hv_ResultIDs);
          SetDraw(*hv_WindowHandle, "fill");
          hv_Text = "Ground truth and result segmentation " + hv_ImageIDStringBraces;
          if (0 != hv_ShowBottomDesc)
          {
              DispText(*hv_WindowHandle, hv_Text, "window", "bottom", "left",
                      "white", "box", "false");
          }
          //
          //Display the legend.
          if (0 != hv_ShowLegend)
          {
              hv_ImageClassIDsUniq = ((hv_GroundTruthIDs.TupleConcat(hv_ResultIDs)).TupleSort()).TupleUniq();
              //Get Indices according to image class IDs.
              TupleGenConst(hv_ImageClassIDsUniq.TupleLength(), 0, &hv_ImageClassIDsIndices);
              {
                  HTuple end_val982 = (hv_ImageClassIDsUniq.TupleLength()) - 1;
                  HTuple step_val982 = 1;
                  for (hv_ImageClassIDsIndex = 0; hv_ImageClassIDsIndex.Continue(end_val982, step_val982); hv_ImageClassIDsIndex += step_val982)
                  {
                      hv_ImageClassIDsIndices[hv_ImageClassIDsIndex] = hv_ClassIDs.TupleFindFirst(HTuple(hv_ImageClassIDsUniq[hv_ImageClassIDsIndex]));
                  }
              }
              hv_Text = hv_Text.TupleConcat(HTuple(hv_ClassesLegend[hv_ImageClassIDsIndices]));
              hv_Text[(hv_Text.TupleLength()) + 1] = HTuple("- thicker line: result, thinner lines: ground truth");
              hv_Text[hv_Text.TupleLength()] = "  (you may have to zoom in for a more detailed view)";
              hv_StringSegExcludeClassIDs = "";
              {
                  HTuple end_val989 = (hv_SegExcludeClassIDs.TupleLength()) - 1;
                  HTuple step_val989 = 1;
                  for (hv_StringIndex = 0; hv_StringIndex.Continue(end_val989, step_val989); hv_StringIndex += step_val989)
                  {
                      if (0 != (int(hv_StringIndex == ((hv_SegExcludeClassIDs.TupleLength()) - 1))))
                      {
                          hv_StringSegExcludeClassIDs += HTuple(hv_SegExcludeClassIDs[hv_StringIndex]);
                      }
                      else
                      {
                          hv_StringSegExcludeClassIDs = (hv_StringSegExcludeClassIDs + HTuple(hv_SegExcludeClassIDs[hv_StringIndex])) + HTuple(", ");
                      }
                  }
              }
              if (0 != (int(hv_SegExcludeClassIDs != HTuple())))
              {
                  hv_Text[hv_Text.TupleLength()] = ("- (excluded classID(s) " + hv_StringSegExcludeClassIDs) + " from visualization)";
              }
              //
              get_child_window(hv_HeightImage, hv_Font, hv_FontSize, hv_Text, hv_PrevWindowCoordinates,
                  hv_WindowHandleDict, HTuple(hv_KeysForDisplay[hv_KeyIndex]), &hv_WindowImageRatio,
                  &hv_PrevWindowCoordinates);
              DispText(*hv_WindowHandle, hv_Text, "window", "top", "left",
                      (HTuple("white").TupleConcat(HTuple(hv_Colors[hv_ImageClassIDsIndices]))).TupleConcat(((HTuple("white").Append("white")).Append("white"))),
                      "box", "false");
          }
      }
      else
      {
          if (HDevWindowStack::IsOpen())
              DispObj(ho_Image, HDevWindowStack::GetActive());
          //
          //Display regions.
          hv_ColorsResults = hv_Colors + hv_SegTransparency;
          if (HDevWindowStack::IsOpen())
              SetDraw(HDevWindowStack::GetActive(), "margin");
          if (HDevWindowStack::IsOpen())
              SetLineWidth(HDevWindowStack::GetActive(), 2);
          dev_display_segmentation_regions(ho_SegmentationImagGroundTruth, hv_ClassIDs,
              hv_ColorsResults, hv_SegExcludeClassIDs, &hv_GroundTruthIDs);
          if (HDevWindowStack::IsOpen())
              SetLineWidth(HDevWindowStack::GetActive(), 6);
          dev_display_segmentation_regions(ho_SegmentationImageResult, hv_ClassIDs, hv_ColorsResults,
              hv_SegExcludeClassIDs, &hv_ResultIDs);
          if (HDevWindowStack::IsOpen())
              SetDraw(HDevWindowStack::GetActive(), "fill");
          hv_Text = "Ground truth and result segmentation " + hv_ImageIDStringBraces;
          if (0 != hv_ShowBottomDesc)
          {
              if (HDevWindowStack::IsOpen())
                  DispText(HDevWindowStack::GetActive(), hv_Text, "window", "bottom", "left",
                      "white", "box", "false");
          }
          //
          //Display the legend.
          if (0 != hv_ShowLegend)
          {
              hv_ImageClassIDsUniq = ((hv_GroundTruthIDs.TupleConcat(hv_ResultIDs)).TupleSort()).TupleUniq();
              //Get Indices according to image class IDs.
              TupleGenConst(hv_ImageClassIDsUniq.TupleLength(), 0, &hv_ImageClassIDsIndices);
              {
                  HTuple end_val982 = (hv_ImageClassIDsUniq.TupleLength()) - 1;
                  HTuple step_val982 = 1;
                  for (hv_ImageClassIDsIndex = 0; hv_ImageClassIDsIndex.Continue(end_val982, step_val982); hv_ImageClassIDsIndex += step_val982)
                  {
                      hv_ImageClassIDsIndices[hv_ImageClassIDsIndex] = hv_ClassIDs.TupleFindFirst(HTuple(hv_ImageClassIDsUniq[hv_ImageClassIDsIndex]));
                  }
              }
              hv_Text = hv_Text.TupleConcat(HTuple(hv_ClassesLegend[hv_ImageClassIDsIndices]));
              hv_Text[(hv_Text.TupleLength()) + 1] = HTuple("- thicker line: result, thinner lines: ground truth");
              hv_Text[hv_Text.TupleLength()] = "  (you may have to zoom in for a more detailed view)";
              hv_StringSegExcludeClassIDs = "";
              {
                  HTuple end_val989 = (hv_SegExcludeClassIDs.TupleLength()) - 1;
                  HTuple step_val989 = 1;
                  for (hv_StringIndex = 0; hv_StringIndex.Continue(end_val989, step_val989); hv_StringIndex += step_val989)
                  {
                      if (0 != (int(hv_StringIndex == ((hv_SegExcludeClassIDs.TupleLength()) - 1))))
                      {
                          hv_StringSegExcludeClassIDs += HTuple(hv_SegExcludeClassIDs[hv_StringIndex]);
                      }
                      else
                      {
                          hv_StringSegExcludeClassIDs = (hv_StringSegExcludeClassIDs + HTuple(hv_SegExcludeClassIDs[hv_StringIndex])) + HTuple(", ");
                      }
                  }
              }
              if (0 != (int(hv_SegExcludeClassIDs != HTuple())))
              {
                  hv_Text[hv_Text.TupleLength()] = ("- (excluded classID(s) " + hv_StringSegExcludeClassIDs) + " from visualization)";
              }
              //
              get_child_window(hv_HeightImage, hv_Font, hv_FontSize, hv_Text, hv_PrevWindowCoordinates,
                  hv_WindowHandleDict, HTuple(hv_KeysForDisplay[hv_KeyIndex]), &hv_WindowImageRatio,
                  &hv_PrevWindowCoordinates);
              if (HDevWindowStack::IsOpen())
                  DispText(HDevWindowStack::GetActive(), hv_Text, "window", "top", "left",
                      (HTuple("white").TupleConcat(HTuple(hv_Colors[hv_ImageClassIDsIndices]))).TupleConcat(((HTuple("white").Append("white")).Append("white"))),
                      "box", "false");
          }
      }
    }
    else if (0 != (int(HTuple(hv_KeysForDisplay[hv_KeyIndex])==HTuple("segmentation_image_diff"))))
    {
      //
      //Difference of ground truth and result segmentation on image.
      get_image(&ho_Image, hv_SampleKeys, hv_DLSample);
      get_segmentation_image_ground_truth(&ho_SegmentationImagGroundTruth, hv_SampleKeys, 
          hv_DLSample);
      get_segmentation_image_result(&ho_SegmentationImageResult, hv_ResultKeys, hv_DLResult);
      //
      //Get or open next window.
      GetImageSize(ho_Image, &hv_WidthImage, &hv_HeightImage);
      get_next_window(hv_Font, hv_FontSize, hv_ShowBottomDesc, hv_WidthImage, hv_HeightImage, 
          0, hv_ScaleWindows, hv_ThresholdWidth, hv_PrevWindowCoordinates, hv_WindowHandleDict, 
          HTuple(hv_KeysForDisplay[hv_KeyIndex]), &hv_CurrentWindowHandle, &hv_WindowImageRatio, 
          &hv_PrevWindowCoordinates);
      //
      //Visualization.
      if (0 != hv_WindowHandle)
      {
          DispObj(ho_Image, *hv_WindowHandle);
          AbsDiffImage(ho_SegmentationImagGroundTruth, ho_SegmentationImageResult, &ho_ImageAbsDiff,
              1);
          MinMaxGray(ho_SegmentationImageResult, ho_ImageAbsDiff, 0, &hv_Min, &hv_Max,
              &hv_Range);
          if (0 != (int(hv_Min != hv_Max)))
          {
              Threshold(ho_ImageAbsDiff, &ho_DiffRegion, 0.00001, hv_Max);
              SetColor(*hv_WindowHandle, "#ff0000" + hv_SegTransparency);
              DispObj(ho_DiffRegion, *hv_WindowHandle);
          }
          else
          {
              DispText(*hv_WindowHandle, "No difference found.", "window",
                      "top", "left", "black", HTuple(), HTuple());
          }
          if (0 != hv_ShowBottomDesc)
          {
              hv_Text = "Difference of ground truth and result segmentation " + hv_ImageIDStringBraces;
              DispText(*hv_WindowHandle, hv_Text, "window", "bottom", "left",
                      "white", "box", "false");
          }
      }
      else
      {
          if (HDevWindowStack::IsOpen())
              DispObj(ho_Image, HDevWindowStack::GetActive());
          AbsDiffImage(ho_SegmentationImagGroundTruth, ho_SegmentationImageResult, &ho_ImageAbsDiff,
              1);
          MinMaxGray(ho_SegmentationImageResult, ho_ImageAbsDiff, 0, &hv_Min, &hv_Max,
              &hv_Range);
          if (0 != (int(hv_Min != hv_Max)))
          {
              Threshold(ho_ImageAbsDiff, &ho_DiffRegion, 0.00001, hv_Max);
              if (HDevWindowStack::IsOpen())
                  SetColor(HDevWindowStack::GetActive(), "#ff0000" + hv_SegTransparency);
              if (HDevWindowStack::IsOpen())
                  DispObj(ho_DiffRegion, HDevWindowStack::GetActive());
          }
          else
          {
              if (HDevWindowStack::IsOpen())
                  DispText(HDevWindowStack::GetActive(), "No difference found.", "window",
                      "top", "left", "black", HTuple(), HTuple());
          }
          if (0 != hv_ShowBottomDesc)
          {
              hv_Text = "Difference of ground truth and result segmentation " + hv_ImageIDStringBraces;
              if (HDevWindowStack::IsOpen())
                  DispText(HDevWindowStack::GetActive(), hv_Text, "window", "bottom", "left",
                      "white", "box", "false");
          }
      }
    }
    else if (0 != (int(HTuple(hv_KeysForDisplay[hv_KeyIndex])==HTuple("segmentation_weight_map"))))
    {
      //
      //Weight map on image.
      get_image(&ho_Image, hv_SampleKeys, hv_DLSample);
      get_weight_image(&ho_ImageWeight, hv_SampleKeys, hv_DLSample);
      //
      if (0 != (int(hv_SegMaxWeight==0)))
      {
        //Calculate SegMaxWeight if not given in GenParam.
        MinMaxGray(ho_ImageWeight, ho_ImageWeight, 0, &hv_MinWeight, &hv_SegMaxWeight, 
            &hv_Range);
      }
      //
      //Get or open next window.
      GetImageSize(ho_Image, &hv_WidthImage, &hv_HeightImage);
      get_next_window(hv_Font, hv_FontSize, hv_ShowBottomDesc, hv_WidthImage, hv_HeightImage, 
          hv_MapColorBarWidth, hv_ScaleWindows, hv_ThresholdWidth, hv_PrevWindowCoordinates, 
          hv_WindowHandleDict, HTuple(hv_KeysForDisplay[hv_KeyIndex]), &hv_CurrentWindowHandle, 
          &hv_WindowImageRatio, &hv_PrevWindowCoordinates);
      //
      //Visualization.
      if (0 != hv_WindowHandle)
      {
          DispObj(ho_Image, *hv_WindowHandle);
          dev_display_weight_regions(ho_ImageWeight, hv_MapTransparency, hv_SegMaxWeight,
              &hv_WeightsColors);
          dev_display_map_color_bar(hv_WidthImage, hv_HeightImage, hv_MapColorBarWidth,
              hv_WeightsColors, hv_SegMaxWeight, hv_WindowImageRatio, hv_CurrentWindowHandle);
          if (0 != hv_ShowBottomDesc)
          {
              DispText(*hv_WindowHandle, "Weight map " + hv_ImageIDStringBraces,
                      "window", "bottom", "left", "white", "box", "false");
          }
      }
      else
      {
          if (HDevWindowStack::IsOpen())
              DispObj(ho_Image, HDevWindowStack::GetActive());
          dev_display_weight_regions(ho_ImageWeight, hv_MapTransparency, hv_SegMaxWeight,
              &hv_WeightsColors);
          dev_display_map_color_bar(hv_WidthImage, hv_HeightImage, hv_MapColorBarWidth,
              hv_WeightsColors, hv_SegMaxWeight, hv_WindowImageRatio, hv_CurrentWindowHandle);
          if (0 != hv_ShowBottomDesc)
          {
              if (HDevWindowStack::IsOpen())
                  DispText(HDevWindowStack::GetActive(), "Weight map " + hv_ImageIDStringBraces,
                      "window", "bottom", "left", "white", "box", "false");
          }
      }
    }
    else if (0 != (int(HTuple(hv_KeysForDisplay[hv_KeyIndex])==HTuple("segmentation_confidence_map"))))
    {
      //
      //Segmentation confidence map on image.
      get_image(&ho_Image, hv_SampleKeys, hv_DLSample);
      get_confidence_image(&ho_ImageConfidence, hv_ResultKeys, hv_DLResult);
      //
      //Get or open next window.
      GetImageSize(ho_Image, &hv_WidthImage, &hv_HeightImage);
      get_next_window(hv_Font, hv_FontSize, hv_ShowBottomDesc, hv_WidthImage, hv_HeightImage, 
          hv_MapColorBarWidth, hv_ScaleWindows, hv_ThresholdWidth, hv_PrevWindowCoordinates, 
          hv_WindowHandleDict, HTuple(hv_KeysForDisplay[hv_KeyIndex]), &hv_CurrentWindowHandle, 
          &hv_WindowImageRatio, &hv_PrevWindowCoordinates);
      //
      //Visualization.
      if (0 != hv_WindowHandle)
      {
          DispObj(ho_Image, *hv_WindowHandle);
          dev_display_confidence_regions(ho_ImageConfidence, hv_MapTransparency, &hv_ConfidenceColors);
          dev_display_map_color_bar(hv_WidthImage, hv_HeightImage, hv_MapColorBarWidth,
              hv_ConfidenceColors, 1.0, hv_WindowImageRatio, hv_CurrentWindowHandle);
          if (0 != hv_ShowBottomDesc)
          {
              DispText(*hv_WindowHandle, "Confidence map " + hv_ImageIDStringBraces,
                      "window", "bottom", "left", "white", "box", "false");
          }
      }
      else
      {
          if (HDevWindowStack::IsOpen())
              DispObj(ho_Image, HDevWindowStack::GetActive());
          dev_display_confidence_regions(ho_ImageConfidence, hv_MapTransparency, &hv_ConfidenceColors);
          dev_display_map_color_bar(hv_WidthImage, hv_HeightImage, hv_MapColorBarWidth,
              hv_ConfidenceColors, 1.0, hv_WindowImageRatio, hv_CurrentWindowHandle);
          if (0 != hv_ShowBottomDesc)
          {
              if (HDevWindowStack::IsOpen())
                  DispText(HDevWindowStack::GetActive(), "Confidence map " + hv_ImageIDStringBraces,
                      "window", "bottom", "left", "white", "box", "false");
          }
      }
    }
    else
    {
      //Reset flush buffer of existing windows before throwing an exception.
      GetDictParam(hv_WindowHandleDict, "keys", HTuple(), &hv_WindowHandleKeys);
      {
      HTuple end_val1071 = (hv_WindowHandleKeys.TupleLength())-1;
      HTuple step_val1071 = 1;
      for (hv_Index=0; hv_Index.Continue(end_val1071, step_val1071); hv_Index += step_val1071)
      {
        //Only consider the WindowHandleKeys that are needed for the current visualization.
        hv_Indices = hv_KeysForDisplay.TupleFind(HTuple(hv_WindowHandleKeys[hv_Index]));
        if (0 != (HTuple(int(hv_Indices!=-1)).TupleAnd(int(hv_Indices!=HTuple()))))
        {
          GetDictTuple(hv_WindowHandleDict, HTuple(hv_WindowHandleKeys[hv_Index]), 
              &hv_WindowHandles);
          {
          HTuple end_val1076 = (hv_WindowHandles.TupleLength())-1;
          HTuple step_val1076 = 1;
          for (hv_WindowIndex=0; hv_WindowIndex.Continue(end_val1076, step_val1076); hv_WindowIndex += step_val1076)
          {
            //Reset values of windows that have been changed temporarily.
            SetWindowParam(HTuple(hv_WindowHandles[hv_WindowIndex]), "flush", HTuple(hv_FlushValues[hv_Index]));
          }
          }
        }
      }
      }
      throw HException("Key for display unknown: "+HTuple(hv_KeysForDisplay[hv_KeyIndex]));
    }
  }
  }
  //
  //Display results.
  if (0 != hv_WindowHandle)
  {
      FlushBuffer(*hv_WindowHandle);
      SetWindowParam(*hv_WindowHandle, "flush", "false");
  }
  else
  {
      GetDictParam(hv_WindowHandleDict, "keys", HTuple(), &hv_WindowHandleKeysNew);
      {
          HTuple end_val1088 = (hv_WindowHandleKeysNew.TupleLength()) - 1;
          HTuple step_val1088 = 1;
          for (hv_Index = 0; hv_Index.Continue(end_val1088, step_val1088); hv_Index += step_val1088)
          {
              //Only consider the WindowHandleKeys that are needed for the current visualization.
              hv_KeyIndex = hv_KeysForDisplay.TupleFind(HTuple(hv_WindowHandleKeysNew[hv_Index]));
              if (0 != (HTuple(int(hv_KeyIndex != -1)).TupleAnd(int(hv_KeyIndex != HTuple()))))
              {
                  GetDictTuple(hv_WindowHandleDict, HTuple(hv_WindowHandleKeysNew[hv_Index]),
                      &hv_WindowHandles);
                  {
                      HTuple end_val1093 = (hv_WindowHandles.TupleLength()) - 1;
                      HTuple step_val1093 = 1;
                      for (hv_WindowIndex = 0; hv_WindowIndex.Continue(end_val1093, step_val1093); hv_WindowIndex += step_val1093)
                      {
                          //Display content of window handle.
                          if (0 != (int((hv_WindowHandleKeys.TupleLength()) == (hv_WindowHandleKeysNew.TupleLength()))))
                          {
                              //Reset values of windows that have been changed temporarily.
                              if (0 != (int(HTuple(hv_FlushValues[hv_WindowIndex]) == HTuple("true"))))
                              {
                                  FlushBuffer(HTuple(hv_WindowHandles[hv_WindowIndex]));
                              }
                              SetWindowParam(HTuple(hv_WindowHandles[hv_WindowIndex]), "flush", HTuple(hv_FlushValues[hv_WindowIndex]));
                          }
                          else
                          {
                              //Per default, 'flush' of new windows should be set to 'true'.
                              FlushBuffer(HTuple(hv_WindowHandles[hv_WindowIndex]));
                              SetWindowParam(HTuple(hv_WindowHandles[hv_WindowIndex]), "flush", "true");
                          }
                      }
                  }
              }
          }
      }
      //
  }
  return;
}

// Chapter: Deep Learning / Model
// Short Description: Visualize for a given number of samples the raw image, ground truth annotation, and inferred results. 
void dev_display_dl_data_tiled (HTuple hv_DLDataset, HTuple hv_DLModelHandle, HTuple hv_NumSamples, 
    HTuple hv_Split, HTuple hv_GenParam, HTuple hv_WindowHandle, HTuple *hv_WindowHandleOut)
{

  // Local iconic variables
  HObject  ho_FinalRows, ho_SampleImages, ho_SampleImage;
  HObject  ho_FirstChannel, ho_ImageScaled, ho_ImageConverted;
  HObject  ho_TiledImageRow, ho_TiledImage, ho_Domain, ho_RegionComplement;
  HObject  ho_Image, ho_ImageCleared, ho_TiledResult;

  // Local control variables
  HTuple  hv_BackGroundColor, hv_SpacingRow, hv_SpacingCol;
  HTuple  hv_WindowRow, hv_WindowCol, hv_WindowWidth, hv_WindowHeight;
  HTuple  hv_IncludeImage, hv_IncludeGroundTruth, hv_IncludeResults;
  HTuple  hv_Keys, hv_Index, hv_Key, hv_Value, hv_ModelType;
  HTuple  hv_Samples, hv_ClassificationLabelExists, hv_DetectionLabelExists;
  HTuple  hv_SegmentationLabelExists, hv_DLSamples, hv_SampleIndices;
  HTuple  hv_DLResult, hv_ClassIDs, hv_BackGroundClass, hv_RGB;
  HTuple  hv_Scale, hv_Shift, hv_KeyExists, hv_DLDatasetPreprocessParam;
  HTuple  hv_NormType, hv_KeyMinExists, hv_KeyMaxExists, hv_ImageRangeMin;
  HTuple  hv_ImageRangeMax, hv_Channels, hv_Width, hv_Height;
  HTuple  hv_Crop, hv_OffsetRow, hv_TiledWidth, hv_TiledHeight;
  HTuple  hv_NumResult, hv_PredictionsCorrect, hv_ImageConfidences;
  HTuple  hv_ResClassIndices, hv_ResClasses, hv_ImageIDs;
  HTuple  hv_ImageLabelIDs, hv_IndexResult, hv_ImageConfidence;
  HTuple  hv_ResClassIndex, hv_ResClass, hv_ImageID, hv_ImageLabelID;
  HTuple  hv_PredictionCorrect, hv_TextImageRows, hv_TextImageColumns;
  HTuple  hv_TextImageWidth, hv_TextImageHeight, hv_TopOffset;
  HTuple  hv_InstanceType, hv_Rect1KeyExists, hv_Rect2KeyExists;
  HTuple  hv_GTLabel, hv_GTCol1, hv_GTRow1, hv_GTCol2, hv_GTRow2;
  HTuple  hv_GTCol, hv_GTRow, hv_GTLength1, hv_GTLength2;
  HTuple  hv_GTPhi, hv_LeftOffset, hv_LabelId, hv_Col1, hv_Row1;
  HTuple  hv_Col2, hv_Row2, hv_Col, hv_Row, hv_Length1, hv_Length2;
  HTuple  hv_Phi, hv_ResConfidence, hv_ResCol1, hv_ResRow1;
  HTuple  hv_ResCol2, hv_ResRow2, hv_ResCol, hv_ResRow, hv_ResLength1;
  HTuple  hv_ResLength2, hv_ResPhi, hv_ClassId, hv_Confidence;
  HTuple  hv_TiledDLSample, hv_TiledDLResult, hv_IgnoreDirection;
  HTuple  hv_WindowHandleDict, hv_Flush, hv_KeyForDisplay;
  HTuple  hv_GTText, hv_PredictionText, hv__, hv_GTWidth;
  HTuple  hv_TextHeight, hv_PredictionWidth, hv_WindowImageRatio;

  //
  //This procedure visualizes samples, their ground truth annotations, and,
  //if not deactivated, the results obtained by applying the model given by DLModelHandle.
  //
  //** Set the default values for all the generic parameters: ***
  //
  hv_BackGroundColor = "#ffffff";
  hv_SpacingRow = 0;
  hv_SpacingCol = 0;
  hv_WindowRow = 0;
  hv_WindowCol = 0;
  hv_WindowWidth = 640;
  hv_WindowHeight = 480;
  //For model of type detection and segmentation, it is displayed:
  //- the plain input images in the first row,
  //- the image and their ground truth annotations in the second row,
  //- the image and their inference results in the third row.
  //
  //For models of type classification, it is displayed:
  //- the input image with a prediction box.
  hv_IncludeImage = 1;
  hv_IncludeGroundTruth = 1;
  hv_IncludeResults = 1;
  //
  if (0 != (int((hv_GenParam.TupleLength())==1)))
  {
    GetDictParam(hv_GenParam, "keys", HTuple(), &hv_Keys);
    {
    HTuple end_val26 = (hv_Keys.TupleLength())-1;
    HTuple step_val26 = 1;
    for (hv_Index=0; hv_Index.Continue(end_val26, step_val26); hv_Index += step_val26)
    {
      hv_Key = HTuple(hv_Keys[hv_Index]);
      GetDictTuple(hv_GenParam, hv_Key, &hv_Value);
      if (0 != (int(hv_Key==HTuple("background_color"))))
      {
        hv_BackGroundColor = hv_Value;
      }
      else if (0 != (int(hv_Key==HTuple("spacing_row"))))
      {
        hv_SpacingRow = hv_Value;
      }
      else if (0 != (int(hv_Key==HTuple("spacing_col"))))
      {
        hv_SpacingCol = hv_Value;
      }
      else if (0 != (int(hv_Key==HTuple("window_width"))))
      {
        hv_WindowWidth = hv_Value;
      }
      else if (0 != (int(hv_Key==HTuple("window_height"))))
      {
        hv_WindowHeight = hv_Value;
      }
      else if (0 != (int(hv_Key==HTuple("window_row"))))
      {
        hv_WindowRow = hv_Value;
      }
      else if (0 != (int(hv_Key==HTuple("window_col"))))
      {
        hv_WindowCol = hv_Value;
      }
      else if (0 != (int(hv_Key==HTuple("display_input"))))
      {
        hv_IncludeImage = hv_Value;
      }
      else if (0 != (int(hv_Key==HTuple("display_ground_truth"))))
      {
        hv_IncludeGroundTruth = hv_Value;
      }
      else if (0 != (int(hv_Key==HTuple("display_result"))))
      {
        hv_IncludeResults = hv_Value;
      }
    }
    }
  }
  //
  //If a model was handed over, get model type.
  if (0 != (int(hv_DLModelHandle!=HTuple())))
  {
    GetDlModelParam(hv_DLModelHandle, "type", &hv_ModelType);
    if (0 != (HTuple(HTuple(int(hv_ModelType!=HTuple("classification"))).TupleAnd(int(hv_ModelType!=HTuple("detection")))).TupleAnd(int(hv_ModelType!=HTuple("segmentation")))))
    {
      throw HException("Invalid model type");
    }
  }
  else
  {
    //
    //If no model was handed over, results are not displayed.
    hv_IncludeResults = 0;
    //Conclude the model type from the existing keys.
    GetDictTuple(hv_DLDataset, "samples", &hv_Samples);
    GetDictParam(HTuple(hv_Samples[0]), "key_exists", "image_label_id", &hv_ClassificationLabelExists);
    GetDictParam(HTuple(hv_Samples[0]), "key_exists", "bbox_label_id", &hv_DetectionLabelExists);
    GetDictParam(HTuple(hv_Samples[0]), "key_exists", "segmentation_file_name", &hv_SegmentationLabelExists);
    if (0 != hv_ClassificationLabelExists)
    {
      hv_ModelType = "classification";
    }
    else if (0 != hv_DetectionLabelExists)
    {
      hv_ModelType = "detection";
    }
    else if (0 != hv_SegmentationLabelExists)
    {
      hv_ModelType = "segmentation";
    }
    else
    {
      throw HException("Cannot conclude model type from DLDataset.");
    }
  }
  //
  //** Generate results for a random subset of NumSamples-many samples: ***
  //
  //Select random samples.
  GetDictTuple(hv_DLDataset, "samples", &hv_DLSamples);
  find_dl_samples(hv_DLSamples, "split", hv_Split, "or", &hv_SampleIndices);
  tuple_shuffle(hv_SampleIndices, &hv_SampleIndices);
  //
  //Make sure that NumSamples is not bigger than the found number of SampleIndices
  hv_NumSamples = hv_NumSamples.TupleMin2(hv_SampleIndices.TupleLength());
  //
  //Calculate the result for the selected samples.
  hv_NumSamples = hv_NumSamples.TupleMin2(hv_SampleIndices.TupleLength());
  hv_SampleIndices = hv_SampleIndices.TupleSelectRange(0,hv_NumSamples-1);
  //
  if (0 != hv_IncludeResults)
  {
    read_dl_samples(hv_DLDataset, hv_SampleIndices, &hv_DLSamples);
    ApplyDlModel(hv_DLModelHandle, hv_DLSamples, HTuple(), &hv_DLResult);
    hv_DLResult = hv_DLResult.TupleSelectRange(0,hv_NumSamples-1);
    hv_DLSamples = hv_DLSamples.TupleSelectRange(0,hv_NumSamples-1);
  }
  else
  {
    read_dl_samples(hv_DLDataset, hv_SampleIndices, &hv_DLSamples);
  }
  //
  //** Generate the tiled image and the tiled result data: ***
  //
  GetDictTuple(hv_DLDataset, "class_ids", &hv_ClassIDs);
  //For segmentation models, we need a class ID that is not present in class_ids, to
  //mark regions without region visualization.
  hv_BackGroundClass = (hv_ClassIDs.TupleMax())+1;
  hv_RGB.Clear();
  hv_RGB.Append(("0x"+(hv_BackGroundColor.TupleSubstr(1,2))).TupleNumber());
  hv_RGB.Append(("0x"+(hv_BackGroundColor.TupleSubstr(3,4))).TupleNumber());
  hv_RGB.Append(("0x"+(hv_BackGroundColor.TupleSubstr(5,6))).TupleNumber());
  //
  GenEmptyObj(&ho_FinalRows);
  //
  //Generate the tiled sample image.
  //For the visualization it is better to use byte images.
  //The problem is that if you don't convert to bytes the range used to display is dynamic.
  //That means there can be strong fluctuations between several images, especially if there are "outliers" in the images.
  //By default expected range of the input image is [-127,128], but this is not the case for the normalization type 'constant_values'.
  hv_Scale = 1.0;
  hv_Shift = 127.0;
  GetDictParam(hv_DLDataset, "key_exists", "preprocess_param", &hv_KeyExists);
  if (0 != hv_KeyExists)
  {
    GetDictTuple(hv_DLDataset, "preprocess_param", &hv_DLDatasetPreprocessParam);
    GetDictParam(hv_DLDatasetPreprocessParam, "key_exists", "normalization_type", 
        &hv_KeyExists);
    if (0 != hv_KeyExists)
    {
      GetDictTuple(hv_DLDatasetPreprocessParam, "normalization_type", &hv_NormType);
      if (0 != (int(hv_NormType==HTuple("constant_values"))))
      {
        GetDictParam(hv_DLDatasetPreprocessParam, "key_exists", "image_range_min", 
            &hv_KeyMinExists);
        GetDictParam(hv_DLDatasetPreprocessParam, "key_exists", "image_range_min", 
            &hv_KeyMaxExists);
        if (0 != (hv_KeyMinExists.TupleAnd(hv_KeyMaxExists)))
        {
          GetDictTuple(hv_DLDatasetPreprocessParam, "image_range_min", &hv_ImageRangeMin);
          GetDictTuple(hv_DLDatasetPreprocessParam, "image_range_max", &hv_ImageRangeMax);
        }
        else
        {
          hv_ImageRangeMin = -2.0;
          hv_ImageRangeMax = 2.0;
        }
        if (0 != (int((hv_ImageRangeMax-hv_ImageRangeMin)==0)))
        {
          hv_Scale = 1.0;
        }
        else
        {
          hv_Scale = 255.0/(hv_ImageRangeMax-hv_ImageRangeMin);
        }
        hv_Shift = (-hv_Scale)*hv_ImageRangeMin;
      }
    }
  }
  GenEmptyObj(&ho_SampleImages);
  {
  HTuple end_val145 = hv_NumSamples-1;
  HTuple step_val145 = 1;
  for (hv_Index=0; hv_Index.Continue(end_val145, step_val145); hv_Index += step_val145)
  {
    GetDictObject(&ho_SampleImage, HTuple(hv_DLSamples[hv_Index]), "image");
    CountChannels(ho_SampleImage, &hv_Channels);
    if (0 != (int(hv_Channels!=3)))
    {
      //For any number of channels, that is not three, just use the first channel.
      AccessChannel(ho_SampleImage, &ho_FirstChannel, 1);
      Compose3(ho_FirstChannel, ho_FirstChannel, ho_FirstChannel, &ho_SampleImage
          );
    }
    //Convert the images back to byte.
    ScaleImage(ho_SampleImage, &ho_ImageScaled, hv_Scale, hv_Shift);
    ConvertImageType(ho_ImageScaled, &ho_ImageConverted, "byte");
    //Add the image to the tuple SampleImages.
    ConcatObj(ho_SampleImages, ho_ImageConverted, &ho_SampleImages);
  }
  }
  //Tile the images in a row.
  GetImageSize(ho_SampleImages, &hv_Width, &hv_Height);
  TupleGenConst(hv_Width.TupleLength(), -1, &hv_Crop);
  TupleGenConst(hv_Width.TupleLength(), 0, &hv_OffsetRow);
  if (0 != (HTuple(int(hv_ModelType==HTuple("detection"))).TupleOr(int(hv_ModelType==HTuple("segmentation")))))
  {
    TileImagesOffset(ho_SampleImages, &ho_TiledImageRow, hv_OffsetRow, (HTuple(0).TupleConcat((hv_Width+hv_SpacingCol).TupleCumul())).TupleSelectRange(0,(hv_Width.TupleLength())-1), 
        hv_Crop, hv_Crop, hv_Crop, hv_Crop, HTuple(((hv_Width+hv_SpacingCol).TupleCumul())[(hv_Width.TupleLength())-1])-hv_SpacingCol, 
        hv_Height.TupleMax());
  }
  //
  //Generate the columns of images for the tiled output.
  if (0 != hv_IncludeImage)
  {
    if (0 != (int(hv_ModelType==HTuple("classification"))))
    {
      throw HException("The key 'display_input' cannot be set for model type classification.");
      return;
    }
    ConcatObj(ho_FinalRows, ho_TiledImageRow, &ho_FinalRows);
  }
  if (0 != hv_IncludeGroundTruth)
  {
    if (0 != (int(hv_ModelType==HTuple("classification"))))
    {
      throw HException("The key 'display_ground_truth' cannot be set for model type classification.");
      return;
    }
    ConcatObj(ho_FinalRows, ho_TiledImageRow, &ho_FinalRows);
  }
  if (0 != (HTuple(int(hv_ModelType==HTuple("detection"))).TupleOr(int(hv_ModelType==HTuple("segmentation")))))
  {
    if (0 != hv_IncludeResults)
    {
      ConcatObj(ho_FinalRows, ho_TiledImageRow, &ho_FinalRows);
    }
    GetImageSize(ho_FinalRows, &hv_TiledWidth, &hv_TiledHeight);
    TileImagesOffset(ho_FinalRows, &ho_TiledImage, (HTuple(0).TupleConcat((hv_TiledHeight+hv_SpacingRow).TupleCumul())).TupleSelectRange(0,(hv_TiledHeight.TupleLength())-1), 
        HTuple(hv_TiledHeight.TupleLength(),0), HTuple(hv_TiledHeight.TupleLength(),-1), 
        HTuple(hv_TiledHeight.TupleLength(),-1), HTuple(hv_TiledHeight.TupleLength(),-1), 
        HTuple(hv_TiledHeight.TupleLength(),-1), hv_TiledWidth.TupleMax(), HTuple(((hv_TiledHeight+hv_SpacingRow).TupleCumul())[(hv_TiledHeight.TupleLength())-1])-hv_SpacingRow);
    //Fill up with the background color.
    GetDomain(ho_TiledImage, &ho_Domain);
    FullDomain(ho_TiledImage, &ho_TiledImage);
    Complement(ho_Domain, &ho_RegionComplement);
    OverpaintRegion(ho_TiledImage, ho_RegionComplement, hv_RGB, "fill");
  }
  //
  //Generate the tiled data (classification prediction / segmentation regions / detection bounding boxes).
  if (0 != (int(hv_ModelType==HTuple("classification"))))
  {
    //
    //For classification results the image is shown within a colored frame.
    //
    GenEmptyObj(&ho_FinalRows);
    //
    if (0 != hv_IncludeResults)
    {
      TupleLength(hv_DLResult, &hv_NumResult);
      hv_PredictionsCorrect = HTuple();
      hv_ImageConfidences = HTuple();
      hv_ResClassIndices = HTuple();
      hv_ResClasses = HTuple();
      hv_ImageIDs = HTuple();
      hv_ImageLabelIDs = HTuple();
      //
      {
      HTuple end_val211 = hv_NumResult-1;
      HTuple step_val211 = 1;
      for (hv_IndexResult=0; hv_IndexResult.Continue(end_val211, step_val211); hv_IndexResult += step_val211)
      {
        //
        //Get labels and classes.
        GetDictTuple(HTuple(hv_DLResult[hv_IndexResult]), "classification_confidences", 
            &hv_ImageConfidence);
        GetDictTuple(HTuple(hv_DLResult[hv_IndexResult]), "classification_class_ids", 
            &hv_ResClassIndex);
        GetDictTuple(HTuple(hv_DLResult[hv_IndexResult]), "classification_class_names", 
            &hv_ResClass);
        GetDictTuple(HTuple(hv_DLSamples[hv_IndexResult]), "image_id", &hv_ImageID);
        GetDictTuple(HTuple(hv_DLSamples[hv_IndexResult]), "image_label_id", &hv_ImageLabelID);
        //
        //Check whether the actual sample prediction is false or correct.
        hv_PredictionCorrect = int(HTuple(hv_ResClassIndex[0])==hv_ImageLabelID);
        hv_PredictionsCorrect = hv_PredictionsCorrect.TupleConcat(hv_PredictionCorrect);
        hv_ImageConfidences = hv_ImageConfidences.TupleConcat(HTuple(hv_ImageConfidence[0]));
        hv_ResClassIndices = hv_ResClassIndices.TupleConcat(HTuple(hv_ResClassIndex[0]));
        hv_ResClasses = hv_ResClasses.TupleConcat(HTuple(hv_ResClass[0]));
        hv_ImageIDs = hv_ImageIDs.TupleConcat(hv_ImageID);
        hv_ImageLabelIDs = hv_ImageLabelIDs.TupleConcat(hv_ImageLabelID);
      }
      }
      //
      //Tile the sample images.
      gen_tiled_classification_image_result(&ho_TiledImageRow, hv_DLSamples, hv_SpacingCol, 
          hv_PredictionsCorrect, hv_ResClasses, &hv_TextImageRows, &hv_TextImageColumns, 
          &hv_TextImageWidth, &hv_TextImageHeight);
      ConcatObj(ho_FinalRows, ho_TiledImageRow, &ho_FinalRows);
    }
    GetImageSize(ho_FinalRows, &hv_TiledWidth, &hv_TiledHeight);
    ho_TiledImage = ho_TiledImageRow;
    //
  }
  else if (0 != (int(hv_ModelType==HTuple("detection"))))
  {
    //
    //For detection results the detected boxes must be moved
    //to the positions of the respective image.
    hv_TopOffset = 0;
    if (0 != hv_IncludeImage)
    {
      hv_TopOffset = (hv_TopOffset+hv_SpacingRow)+HTuple(hv_TiledHeight[0]);
    }
    //Check the detection type.
    if (0 != (int((hv_DLModelHandle.TupleLength())>0)))
    {
      GetDlModelParam(hv_DLModelHandle, "instance_type", &hv_InstanceType);
    }
    else
    {
      GetDictParam(HTuple(hv_DLSamples[0]), "key_exists", "bbox_row1", &hv_Rect1KeyExists);
      if (0 != hv_Rect1KeyExists)
      {
        hv_InstanceType = "rectangle1";
      }
      else
      {
        GetDictParam(HTuple(hv_DLSamples[0]), "key_exists", "bbox_phi", &hv_Rect2KeyExists);
        if (0 != hv_Rect2KeyExists)
        {
          hv_InstanceType = "rectangle2";
        }
        else
        {
          throw HException("Not suitable sample keys!");
        }
      }
    }
    //Move the ground truth labels.
    hv_GTLabel = HTuple();
    if (0 != (int(hv_InstanceType==HTuple("rectangle1"))))
    {
      hv_GTCol1 = HTuple();
      hv_GTRow1 = HTuple();
      hv_GTCol2 = HTuple();
      hv_GTRow2 = HTuple();
    }
    else
    {
      hv_GTCol = HTuple();
      hv_GTRow = HTuple();
      hv_GTLength1 = HTuple();
      hv_GTLength2 = HTuple();
      hv_GTPhi = HTuple();
    }
    if (0 != hv_IncludeGroundTruth)
    {
      hv_LeftOffset = 0;
      {
      HTuple end_val277 = hv_NumSamples-1;
      HTuple step_val277 = 1;
      for (hv_Index=0; hv_Index.Continue(end_val277, step_val277); hv_Index += step_val277)
      {
        GetDictTuple(HTuple(hv_DLSamples[hv_Index]), "bbox_label_id", &hv_LabelId);
        hv_GTLabel = hv_GTLabel.TupleConcat(hv_LabelId);
        if (0 != (int(hv_InstanceType==HTuple("rectangle1"))))
        {
          GetDictTuple(HTuple(hv_DLSamples[hv_Index]), "bbox_col1", &hv_Col1);
          hv_GTCol1 = hv_GTCol1.TupleConcat(hv_Col1+hv_LeftOffset);
          GetDictTuple(HTuple(hv_DLSamples[hv_Index]), "bbox_row1", &hv_Row1);
          hv_GTRow1 = hv_GTRow1.TupleConcat(hv_Row1+hv_TopOffset);
          GetDictTuple(HTuple(hv_DLSamples[hv_Index]), "bbox_col2", &hv_Col2);
          hv_GTCol2 = hv_GTCol2.TupleConcat(hv_Col2+hv_LeftOffset);
          GetDictTuple(HTuple(hv_DLSamples[hv_Index]), "bbox_row2", &hv_Row2);
          hv_GTRow2 = hv_GTRow2.TupleConcat(hv_Row2+hv_TopOffset);
        }
        else
        {
          GetDictTuple(HTuple(hv_DLSamples[hv_Index]), "bbox_col", &hv_Col);
          hv_GTCol = hv_GTCol.TupleConcat(hv_Col+hv_LeftOffset);
          GetDictTuple(HTuple(hv_DLSamples[hv_Index]), "bbox_row", &hv_Row);
          hv_GTRow = hv_GTRow.TupleConcat(hv_Row+hv_TopOffset);
          GetDictTuple(HTuple(hv_DLSamples[hv_Index]), "bbox_length1", &hv_Length1);
          hv_GTLength1 = hv_GTLength1.TupleConcat(hv_Length1);
          GetDictTuple(HTuple(hv_DLSamples[hv_Index]), "bbox_length2", &hv_Length2);
          hv_GTLength2 = hv_GTLength2.TupleConcat(hv_Length2);
          GetDictTuple(HTuple(hv_DLSamples[hv_Index]), "bbox_phi", &hv_Phi);
          hv_GTPhi = hv_GTPhi.TupleConcat(hv_Phi);
        }
        hv_LeftOffset = (hv_LeftOffset+hv_SpacingCol)+HTuple(hv_Width[hv_Index]);
      }
      }
      hv_TopOffset = (hv_TopOffset+hv_SpacingRow)+HTuple(hv_TiledHeight[0]);
    }
    hv_ResClass = HTuple();
    hv_ResConfidence = HTuple();
    if (0 != (int(hv_InstanceType==HTuple("rectangle1"))))
    {
      hv_ResCol1 = HTuple();
      hv_ResRow1 = HTuple();
      hv_ResCol2 = HTuple();
      hv_ResRow2 = HTuple();
    }
    else
    {
      hv_ResCol = HTuple();
      hv_ResRow = HTuple();
      hv_ResLength1 = HTuple();
      hv_ResLength2 = HTuple();
      hv_ResPhi = HTuple();
    }
    //Move the result classes.
    if (0 != hv_IncludeResults)
    {
      hv_LeftOffset = 0;
      {
      HTuple end_val322 = hv_NumSamples-1;
      HTuple step_val322 = 1;
      for (hv_Index=0; hv_Index.Continue(end_val322, step_val322); hv_Index += step_val322)
      {
        GetDictTuple(HTuple(hv_DLResult[hv_Index]), "bbox_class_id", &hv_ClassId);
        hv_ResClass = hv_ResClass.TupleConcat(hv_ClassId);
        GetDictTuple(HTuple(hv_DLResult[hv_Index]), "bbox_confidence", &hv_Confidence);
        hv_ResConfidence = hv_ResConfidence.TupleConcat(hv_Confidence);
        if (0 != (int(hv_InstanceType==HTuple("rectangle1"))))
        {
          GetDictTuple(HTuple(hv_DLResult[hv_Index]), "bbox_col1", &hv_Col1);
          hv_ResCol1 = hv_ResCol1.TupleConcat(hv_Col1+hv_LeftOffset);
          GetDictTuple(HTuple(hv_DLResult[hv_Index]), "bbox_row1", &hv_Row1);
          hv_ResRow1 = hv_ResRow1.TupleConcat(hv_Row1+hv_TopOffset);
          GetDictTuple(HTuple(hv_DLResult[hv_Index]), "bbox_col2", &hv_Col2);
          hv_ResCol2 = hv_ResCol2.TupleConcat(hv_Col2+hv_LeftOffset);
          GetDictTuple(HTuple(hv_DLResult[hv_Index]), "bbox_row2", &hv_Row2);
          hv_ResRow2 = hv_ResRow2.TupleConcat(hv_Row2+hv_TopOffset);
        }
        else
        {
          GetDictTuple(HTuple(hv_DLResult[hv_Index]), "bbox_col", &hv_Col);
          hv_ResCol = hv_ResCol.TupleConcat(hv_Col+hv_LeftOffset);
          GetDictTuple(HTuple(hv_DLResult[hv_Index]), "bbox_row", &hv_Row);
          hv_ResRow = hv_ResRow.TupleConcat(hv_Row+hv_TopOffset);
          GetDictTuple(HTuple(hv_DLResult[hv_Index]), "bbox_length1", &hv_Length1);
          hv_ResLength1 = hv_ResLength1.TupleConcat(hv_Length1);
          GetDictTuple(HTuple(hv_DLResult[hv_Index]), "bbox_length2", &hv_Length2);
          hv_ResLength2 = hv_ResLength2.TupleConcat(hv_Length2);
          GetDictTuple(HTuple(hv_DLResult[hv_Index]), "bbox_phi", &hv_Phi);
          hv_ResPhi = hv_ResPhi.TupleConcat(hv_Phi);
        }
        hv_LeftOffset = (hv_LeftOffset+hv_SpacingCol)+HTuple(hv_Width[hv_Index]);
      }
      }
    }
    //
  }
  else if (0 != (int(hv_ModelType==HTuple("segmentation"))))
  {
    //
    //The tiled segmentation result image is constructed in the same way as the tiled sample image.
    GenEmptyObj(&ho_FinalRows);
    if (0 != hv_IncludeImage)
    {
      GenImageConst(&ho_Image, "real", HTuple(hv_TiledWidth[0]), HTuple(hv_TiledHeight[0]));
      //For the top (image) row, set the (virtual) background class as segmentation result.
      GenImageProto(ho_Image, &ho_ImageCleared, hv_BackGroundClass);
      ConcatObj(ho_FinalRows, ho_ImageCleared, &ho_FinalRows);
    }
    //
    if (0 != hv_IncludeGroundTruth)
    {
      gen_tiled_segmentation_image(&ho_TiledImageRow, hv_DLSamples, hv_SpacingCol, 
          hv_Width, hv_Height);
      ConcatObj(ho_FinalRows, ho_TiledImageRow, &ho_FinalRows);
    }
    //
    if (0 != hv_IncludeResults)
    {
      gen_tiled_segmentation_image(&ho_TiledImageRow, hv_DLResult, hv_SpacingCol, 
          hv_Width, hv_Height);
      ConcatObj(ho_FinalRows, ho_TiledImageRow, &ho_FinalRows);
    }
    GetImageSize(ho_FinalRows, &hv_TiledWidth, &hv_TiledHeight);
    TileImagesOffset(ho_FinalRows, &ho_TiledResult, (HTuple(0).TupleConcat((hv_TiledHeight+hv_SpacingRow).TupleCumul())).TupleSelectRange(0,(hv_TiledHeight.TupleLength())-1), 
        HTuple(hv_TiledHeight.TupleLength(),0), HTuple(hv_TiledHeight.TupleLength(),-1), 
        HTuple(hv_TiledHeight.TupleLength(),-1), HTuple(hv_TiledHeight.TupleLength(),-1), 
        HTuple(hv_TiledHeight.TupleLength(),-1), hv_TiledWidth.TupleMax(), HTuple(((hv_TiledHeight+hv_SpacingRow).TupleCumul())[(hv_TiledHeight.TupleLength())-1])-hv_SpacingRow);
    //Fill up with the background color.
    GetDomain(ho_TiledResult, &ho_Domain);
    FullDomain(ho_TiledResult, &ho_TiledResult);
    Complement(ho_Domain, &ho_RegionComplement);
    OverpaintRegion(ho_TiledResult, ho_RegionComplement, hv_BackGroundClass, "fill");
  }
  //
  //** Generate a TiledDLSample and a TiledDLResult to display them with dev_display_dl_data: ***
  //
  //Generate a tiled DLSample.
  if (0 != (int(hv_ModelType==HTuple("classification"))))
  {
    gen_dl_samples_from_images(ho_TiledImage, &hv_TiledDLSample);
    SetDictTuple(hv_TiledDLSample, "image_id", hv_ImageID);
    SetDictTuple(hv_TiledDLSample, "image_label_id", hv_ImageLabelID);
  }
  else if (0 != (int(hv_ModelType==HTuple("detection"))))
  {
    gen_dl_samples_from_images(ho_TiledImage, &hv_TiledDLSample);
    SetDictTuple(hv_TiledDLSample, "bbox_label_id", hv_GTLabel);
    if (0 != (int(hv_InstanceType==HTuple("rectangle1"))))
    {
      SetDictTuple(hv_TiledDLSample, "bbox_col1", hv_GTCol1);
      SetDictTuple(hv_TiledDLSample, "bbox_row1", hv_GTRow1);
      SetDictTuple(hv_TiledDLSample, "bbox_col2", hv_GTCol2);
      SetDictTuple(hv_TiledDLSample, "bbox_row2", hv_GTRow2);
    }
    else
    {
      SetDictTuple(hv_TiledDLSample, "bbox_col", hv_GTCol);
      SetDictTuple(hv_TiledDLSample, "bbox_row", hv_GTRow);
      SetDictTuple(hv_TiledDLSample, "bbox_length1", hv_GTLength1);
      SetDictTuple(hv_TiledDLSample, "bbox_length2", hv_GTLength2);
      SetDictTuple(hv_TiledDLSample, "bbox_phi", hv_GTPhi);
    }
  }
  else if (0 != (int(hv_ModelType==HTuple("segmentation"))))
  {
    gen_dl_samples_from_images(ho_TiledImage, &hv_TiledDLSample);
  }
  //
  //Generate a tiled DLResult.
  CreateDict(&hv_TiledDLResult);
  if (0 != (int(hv_ModelType==HTuple("classification"))))
  {
    SetDictTuple(hv_TiledDLResult, "image_class_id", HTuple(hv_ResClassIndices[0]));
    SetDictTuple(hv_TiledDLResult, "image_confidence", hv_ImageConfidences);
  }
  else if (0 != (int(hv_ModelType==HTuple("segmentation"))))
  {
    SetDictObject(ho_TiledResult, hv_TiledDLResult, "segmentation_image");
  }
  else if (0 != (int(hv_ModelType==HTuple("detection"))))
  {
    SetDictTuple(hv_TiledDLResult, "bbox_class_id", hv_ResClass);
    SetDictTuple(hv_TiledDLResult, "bbox_confidence", hv_ResConfidence);
    if (0 != (int(hv_InstanceType==HTuple("rectangle1"))))
    {
      SetDictTuple(hv_TiledDLResult, "bbox_col1", hv_ResCol1);
      SetDictTuple(hv_TiledDLResult, "bbox_row1", hv_ResRow1);
      SetDictTuple(hv_TiledDLResult, "bbox_col2", hv_ResCol2);
      SetDictTuple(hv_TiledDLResult, "bbox_row2", hv_ResRow2);
    }
    else
    {
      SetDictTuple(hv_TiledDLResult, "bbox_col", hv_ResCol);
      SetDictTuple(hv_TiledDLResult, "bbox_row", hv_ResRow);
      SetDictTuple(hv_TiledDLResult, "bbox_length1", hv_ResLength1);
      SetDictTuple(hv_TiledDLResult, "bbox_length2", hv_ResLength2);
      SetDictTuple(hv_TiledDLResult, "bbox_phi", hv_ResPhi);
    }
  }
  //
  //Set display parameters.
  CreateDict(&hv_GenParam);
  //Pinch off all text display from dev_display_dl_data for a clutterless visualization.
  SetDictTuple(hv_GenParam, "display_bottom_desc", 0);
  SetDictTuple(hv_GenParam, "display_legend", 0);
  SetDictTuple(hv_GenParam, "display_labels", 0);
  if (0 != (HTuple(int(hv_ModelType==HTuple("detection"))).TupleAnd(int((hv_DLModelHandle.TupleLength())>0))))
  {
    //Don't display directions if 'ignore_direction' is set to 'true'.
    GetDlModelParam(hv_DLModelHandle, "ignore_direction", &hv_IgnoreDirection);
    if (0 != (int(hv_IgnoreDirection==HTuple("true"))))
    {
      SetDictTuple(hv_GenParam, "display_direction", 0);
    }
  }
  //
  //Initialize the window for displaying the tiled comparison of input and result.
  CreateDict(&hv_WindowHandleDict);
  if (0 != (int((hv_WindowHandle.TupleLength())==0)))
  {
    GetImageSize(ho_TiledImage, &hv_Width, &hv_Height);
    dev_open_window_fit_size(hv_WindowRow, hv_WindowCol, hv_Width, hv_Height, hv_WindowWidth, 
        hv_WindowHeight,0, &hv_WindowHandle);
  }
  GetWindowParam(hv_WindowHandle, "flush", &hv_Flush);
  if (0 != (int(hv_Flush==HTuple("true"))))
  {
    SetWindowParam(hv_WindowHandle, "flush", "false");
  }
  SetWindowParam(hv_WindowHandle, "background_color", hv_BackGroundColor);
  HDevWindowStack::SetActive(hv_WindowHandle);
  if (HDevWindowStack::IsOpen())
    ClearWindow(HDevWindowStack::GetActive());
  (*hv_WindowHandleOut) = hv_WindowHandle;
  //
  if (0 != (int(hv_ModelType==HTuple("classification"))))
  {
    hv_KeyForDisplay = "image";
  }
  else if (0 != (int(hv_ModelType==HTuple("segmentation"))))
  {
    hv_KeyForDisplay = "segmentation_image_result";
  }
  else if (0 != (int(hv_ModelType==HTuple("detection"))))
  {
    hv_KeyForDisplay = "bbox_both";
  }
  //
  SetDictTuple(hv_WindowHandleDict, hv_KeyForDisplay, hv_WindowHandle);
  //
  //Display the tiled image.
  dev_display_dl_data(hv_TiledDLSample, hv_TiledDLResult, hv_DLDataset, hv_KeyForDisplay, 
      hv_GenParam, hv_WindowHandleDict);
  //
  //For classification, display additional text.
  if (0 != (int(hv_ModelType==HTuple("classification"))))
  {
    GetImageSize(ho_TiledImage, &hv_Width, &hv_Height);
    {
    HTuple end_val476 = hv_NumSamples-1;
    HTuple step_val476 = 1;
    for (hv_Index=0; hv_Index.Continue(end_val476, step_val476); hv_Index += step_val476)
    {
      hv_GTText = "Ground truth: "+HTuple(hv_ImageLabelIDs[hv_Index]);
      hv_PredictionText = "Prediction: "+HTuple(hv_ResClassIndices[hv_Index]);
      GetStringExtents(hv_WindowHandle, hv_GTText, &hv__, &hv__, &hv_GTWidth, &hv_TextHeight);
      GetStringExtents(hv_WindowHandle, hv_PredictionText, &hv__, &hv__, &hv_PredictionWidth, 
          &hv__);
      //Get ratio between image and window coordinate system.
      GetWindowExtents(hv_WindowHandle, &hv__, &hv__, &hv_WindowWidth, &hv_WindowHeight);
      //Adapt widths of texts to plot it correctly in window coordinate system.
      if (0 != (int((hv_Height/(hv_Width.TupleReal()))>(hv_WindowHeight/(hv_WindowWidth.TupleReal())))))
      {
        hv_WindowImageRatio = 1.0/(hv_WindowHeight/(HTuple(hv_Height[0]).TupleReal()));
      }
      else
      {
        hv_WindowImageRatio = 1.0/(hv_WindowWidth/(HTuple(hv_Width[0]).TupleReal()));
      }
      if (0 != (int(hv_TextImageWidth<(((hv_GTWidth+hv_PredictionWidth)*hv_WindowImageRatio)+30))))
      {
        //Make shorter text.
        hv_GTText = "GT: "+HTuple(hv_ImageLabelIDs[hv_Index]);
        hv_PredictionText = "Pred.: "+HTuple(hv_ResClassIndices[hv_Index]);
        GetStringExtents(hv_WindowHandle, hv_GTText, &hv__, &hv__, &hv_GTWidth, &hv_TextHeight);
        GetStringExtents(hv_WindowHandle, hv_PredictionText, &hv__, &hv__, &hv_PredictionWidth, 
            &hv__);
      }
      if (HDevWindowStack::IsOpen())
        DispText(HDevWindowStack::GetActive(),hv_GTText, "image", (HTuple(hv_TextImageRows[hv_Index])+(hv_TextImageHeight/2))-hv_TextHeight, 
            HTuple(hv_TextImageColumns[hv_Index])+10, "white", "box", "false");
      if (HDevWindowStack::IsOpen())
        DispText(HDevWindowStack::GetActive(),hv_PredictionText, "image", (HTuple(hv_TextImageRows[hv_Index])+(hv_TextImageHeight/2))-hv_TextHeight, 
            ((HTuple(hv_TextImageColumns[hv_Index])+hv_TextImageWidth)-(hv_PredictionWidth*hv_WindowImageRatio))-10, 
            "white", "box", "false");
    }
    }
  }
  //
  if (0 != (int(hv_Flush==HTuple("true"))))
  {
    //Only flush the window, if 'flush' is 'true'. Otherwise the caller
    //(who set flush to 'false' on purpose) is responsible for flushing.
    FlushBuffer(hv_WindowHandle);
  }
  SetWindowParam(hv_WindowHandle, "flush", hv_Flush);
  //
  return;
}

// Chapter: Deep Learning / Anomaly Detection
// Short Description: Display the ground truth anomaly regions of the given DLSample. 
void dev_display_ground_truth_anomaly_regions (HTuple hv_SampleKeys, HTuple hv_DLSample, 
    HTuple hv_CurrentWindowHandle, HTuple hv_LineWidth, HTuple hv_AnomalyRegionLabelColor, 
    HTuple hv_AnomalyColorTransparency, HTuple *hv_AnomalyRegionExists)
{

  // Local iconic variables
  HObject  ho_AnomalyImage, ho_AnomalyRegion;

  // Local control variables
  HTuple  hv_Red, hv_Green, hv_Blue, hv_Alpha, hv_InitialColor;
  HTuple  hv_IndexColor, hv_Color_RGBA, hv_Area;

  //
  //This procedure visualizes the ground truth anomalies
  //if there is an anomaly_ground_truth in DLSample.
  //
  //Get current set color.
  GetRgba(hv_CurrentWindowHandle, &hv_Red, &hv_Green, &hv_Blue, &hv_Alpha);
  hv_InitialColor = HTuple();
  {
  HTuple end_val7 = (hv_Red.TupleLength())-1;
  HTuple step_val7 = 1;
  for (hv_IndexColor=0; hv_IndexColor.Continue(end_val7, step_val7); hv_IndexColor += step_val7)
  {
    hv_Color_RGBA = ((("#"+(HTuple(hv_Red[hv_IndexColor]).TupleString("2x")))+(HTuple(hv_Green[hv_IndexColor]).TupleString("2x")))+(HTuple(hv_Blue[hv_IndexColor]).TupleString("2x")))+(HTuple(hv_Alpha[hv_IndexColor]).TupleString("2x"));
    TupleRegexpReplace(hv_Color_RGBA, (HTuple(" ").Append("replace_all")), "0", &hv_Color_RGBA);
    hv_InitialColor = hv_InitialColor.TupleConcat(hv_Color_RGBA);
  }
  }
  //
  if (0 != (int((hv_SampleKeys.TupleFind("anomaly_ground_truth"))!=-1)))
  {
    GetDictObject(&ho_AnomalyImage, hv_DLSample, "anomaly_ground_truth");
    Threshold(ho_AnomalyImage, &ho_AnomalyRegion, 1, 255);
    //Get non-empty regions.
    RegionFeatures(ho_AnomalyRegion, "area", &hv_Area);
    if (0 != (int(hv_Area>0)))
    {
      if (HDevWindowStack::IsOpen())
        SetColor(HDevWindowStack::GetActive(),hv_AnomalyRegionLabelColor+hv_AnomalyColorTransparency);
      //Display the anomaly region.
      if (HDevWindowStack::IsOpen())
        SetDraw(HDevWindowStack::GetActive(),"fill");
      if (HDevWindowStack::IsOpen())
        DispObj(ho_AnomalyRegion, HDevWindowStack::GetActive());
    }
    (*hv_AnomalyRegionExists) = "true";
  }
  else
  {
    (*hv_AnomalyRegionExists) = "false";
  }
  //
  //Reset colors.
  if (HDevWindowStack::IsOpen())
    SetColor(HDevWindowStack::GetActive(),hv_InitialColor);
  //
  return;
}

// Chapter: Graphics / Output
// Short Description: Display the ground truth bounding boxes of DLSample. 
void dev_display_ground_truth_detection (HTuple hv_DLSample, HTuple hv_SampleKeys, 
    HTuple hv_LineWidthBbox, HTuple hv_ClassIDs, HTuple hv_BboxColors, HTuple hv_BboxLabelColor, 
    HTuple hv_WindowImageRatio, HTuple hv_TextColor, HTuple hv_ShowLabels, HTuple hv_ShowDirection, 
    HTuple hv_WindowHandle, HTuple *hv_BboxIDs)
{

  // Local iconic variables
  HObject  ho_BboxRectangle, ho_OrientationArrows;
  HObject  ho_RectangleSelected, ho_ArrowSelected;

  // Local control variables
  HTuple  hv_InstanceType, hv_BboxRow1, hv_BboxCol1;
  HTuple  hv_BboxRow2, hv_BboxCol2, hv_BboxLabels, hv_BboxRow;
  HTuple  hv_BboxCol, hv_BboxLength1, hv_BboxLength2, hv_BboxPhi;
  HTuple  hv_Text, hv_Ascent, hv_Descent, hv__, hv_TextOffset;
  HTuple  hv_LabelRow, hv_LabelCol, hv_HeadSize, hv_ContourStyle;
  HTuple  hv_Style, hv_IndexBbox, hv_ClassID, hv_TextColorClasses;

  //
  //This procedure displays the ground truth bounding boxes of DLSample.
  //
  hv_InstanceType = "rectangle1";
  if (0 != (int((hv_SampleKeys.TupleFind("bbox_row1"))!=-1)))
  {
    GetDictTuple(hv_DLSample, "bbox_row1", &hv_BboxRow1);
    GetDictTuple(hv_DLSample, "bbox_col1", &hv_BboxCol1);
    GetDictTuple(hv_DLSample, "bbox_row2", &hv_BboxRow2);
    GetDictTuple(hv_DLSample, "bbox_col2", &hv_BboxCol2);
    GetDictTuple(hv_DLSample, "bbox_label_id", &hv_BboxLabels);
  }
  else if (0 != (int((hv_SampleKeys.TupleFind("bbox_phi"))!=-1)))
  {
    GetDictTuple(hv_DLSample, "bbox_row", &hv_BboxRow);
    GetDictTuple(hv_DLSample, "bbox_col", &hv_BboxCol);
    GetDictTuple(hv_DLSample, "bbox_length1", &hv_BboxLength1);
    GetDictTuple(hv_DLSample, "bbox_length2", &hv_BboxLength2);
    GetDictTuple(hv_DLSample, "bbox_phi", &hv_BboxPhi);
    GetDictTuple(hv_DLSample, "bbox_label_id", &hv_BboxLabels);
    hv_InstanceType = "rectangle2";
  }
  else
  {
    throw HException("Ground truth bounding box data could not be found in DLSample.");
  }
  if (0 != (int((hv_BboxLabels.TupleLength())>0)))
  {
    //
    //Get text and text size for correct positioning of label IDs.
    if (0 != hv_ShowLabels)
    {
      hv_Text = hv_BboxLabels;
      GetStringExtents(hv_WindowHandle, hv_Text, &hv_Ascent, &hv_Descent, &hv__, 
          &hv__);
      hv_TextOffset = (hv_Ascent+hv_Descent)/hv_WindowImageRatio;
    }
    //
    //Generate bounding box XLDs.
    if (0 != (int(hv_InstanceType==HTuple("rectangle1"))))
    {
      TupleGenConst(hv_BboxRow1.TupleLength(), 0.0, &hv_BboxPhi);
      GenRectangle2ContourXld(&ho_BboxRectangle, 0.5*(hv_BboxRow1+hv_BboxRow2), 0.5*(hv_BboxCol1+hv_BboxCol2), 
          hv_BboxPhi, 0.5*(hv_BboxCol2-hv_BboxCol1), 0.5*(hv_BboxRow2-hv_BboxRow1));
      if (0 != hv_ShowLabels)
      {
        hv_LabelRow = hv_BboxRow1;
        hv_LabelCol = hv_BboxCol1;
      }
    }
    else
    {
      GenRectangle2ContourXld(&ho_BboxRectangle, hv_BboxRow, hv_BboxCol, hv_BboxPhi, 
          hv_BboxLength1, hv_BboxLength2);
      if (0 != hv_ShowLabels)
      {
        hv_LabelRow = hv_BboxRow-hv_TextOffset;
        hv_LabelCol = hv_BboxCol;
      }
      if (0 != hv_ShowDirection)
      {
        hv_HeadSize = 20.0;
        gen_arrow_contour_xld(&ho_OrientationArrows, hv_BboxRow, hv_BboxCol, hv_BboxRow-((hv_BboxLength1+hv_HeadSize)*(hv_BboxPhi.TupleSin())), 
            hv_BboxCol+((hv_BboxLength1+hv_HeadSize)*(hv_BboxPhi.TupleCos())), hv_HeadSize, 
            hv_HeadSize);
      }
    }
    //
    //Collect the ClassIDs of the bounding boxes.
    TupleGenConst(hv_BboxLabels.TupleLength(), 0, &(*hv_BboxIDs));
    //
    //Draw the bounding boxes.
    GetContourStyle(hv_WindowHandle, &hv_ContourStyle);
    if (HDevWindowStack::IsOpen())
      SetContourStyle(HDevWindowStack::GetActive(),"stroke_and_fill");
    GetLineStyle(hv_WindowHandle, &hv_Style);
    if (HDevWindowStack::IsOpen())
      SetLineWidth(HDevWindowStack::GetActive(),hv_LineWidthBbox);
    {
    HTuple end_val58 = (hv_BboxLabels.TupleLength())-1;
    HTuple step_val58 = 1;
    for (hv_IndexBbox=0; hv_IndexBbox.Continue(end_val58, step_val58); hv_IndexBbox += step_val58)
    {
      SelectObj(ho_BboxRectangle, &ho_RectangleSelected, hv_IndexBbox+1);
      hv_ClassID = hv_ClassIDs.TupleFind(HTuple(hv_BboxLabels[hv_IndexBbox]));
      (*hv_BboxIDs)[hv_IndexBbox] = hv_ClassID;
      if (HDevWindowStack::IsOpen())
        SetColor(HDevWindowStack::GetActive(),HTuple(hv_BboxColors[hv_ClassID])+"60");
      if (HDevWindowStack::IsOpen())
        DispObj(ho_RectangleSelected, HDevWindowStack::GetActive());
      if (0 != (HTuple(int(hv_InstanceType==HTuple("rectangle2"))).TupleAnd(hv_ShowDirection)))
      {
        SelectObj(ho_OrientationArrows, &ho_ArrowSelected, hv_IndexBbox+1);
        if (HDevWindowStack::IsOpen())
          SetColor(HDevWindowStack::GetActive(),HTuple(hv_BboxColors[hv_ClassID])+"FF");
        if (HDevWindowStack::IsOpen())
          DispObj(ho_ArrowSelected, HDevWindowStack::GetActive());
        if (HDevWindowStack::IsOpen())
          SetColor(HDevWindowStack::GetActive(),HTuple(hv_BboxColors[hv_ClassID])+"60");
      }
    }
    }
    //
    //Write text to the bounding boxes.
    if (0 != hv_ShowLabels)
    {
      //For better visibility the text is displayed after all bounding boxes are drawn.
      //Select text color.
      if (0 != (int(hv_TextColor==HTuple(""))))
      {
        hv_TextColorClasses = HTuple(hv_BboxColors[(*hv_BboxIDs)]);
      }
      else
      {
        TupleGenConst((*hv_BboxIDs).TupleLength(), hv_TextColor, &hv_TextColorClasses);
      }
      //Display text.
      if (HDevWindowStack::IsOpen())
        DispText(HDevWindowStack::GetActive(),hv_BboxLabels, "image", hv_LabelRow, 
            hv_LabelCol, hv_TextColorClasses, ((HTuple("box_color").Append("shadow")).Append("border_radius")), 
            hv_BboxLabelColor.TupleConcat((HTuple("false").Append(0))));
    }
    //
    if (HDevWindowStack::IsOpen())
      SetContourStyle(HDevWindowStack::GetActive(),hv_ContourStyle);
    SetLineStyle(hv_WindowHandle, hv_Style);
  }
  else
  {
    //Do nothing if there are no ground truth bounding boxes.
    (*hv_BboxIDs) = HTuple();
  }
  //
  return;
}

// Chapter: Deep Learning / Model
// Short Description: Initialize the visualization of the training progress. This includes setting default values for visualization parameters. 
void dev_display_init_train_dl_model (HTuple hv_DLModelHandle, HTuple hv_TrainParam, 
    HTuple *hv_DisplayData)
{

  // Local iconic variables

  // Local control variables
  HTuple  hv_ModelType, hv_DisplayParam, hv_DisplayEnabled;
  HTuple  hv_UpdateImagesIntervalEpochs, hv_NumImagesPreview;
  HTuple  hv_ChangePlotIntervalSeconds, hv_SelectedPercentageTrainSamples;
  HTuple  hv_XAxisLabel, hv_DisplayParamNames, hv_DisplayDefaultValues;
  HTuple  hv_Index, hv_KeyExists, hv_DisplayIntervalSeconds;
  HTuple  hv_Time, hv_StatusModelParamNames, hv_WindowTextWidth;
  HTuple  hv_WindowTextHeight, hv_WindowImagesWidth, hv_WindowImagesHeight;
  HTuple  hv_WindowBGColor, hv_WindowHandleText, hv_WindowImagesRow;
  HTuple  hv_WindowImagesCol, hv_TiledParam, hv_Exception;
  HTuple  hv_TiledParamNames, hv_TiledDefaultValues;

  //
  //This procedure initializes the visualization of the training progress.
  //This includes setting default values for visualization parameters.
  //
  //Get the actual model type.
  if (0 != (int(hv_DLModelHandle!=HTuple())))
  {
    GetDlModelParam(hv_DLModelHandle, "type", &hv_ModelType);
  }
  else
  {
    GetDictTuple(hv_TrainParam, "type", &hv_ModelType);
  }
  //
  //Initialize display data.
  CreateDict(&(*hv_DisplayData));
  GetDictTuple(hv_TrainParam, "display_param", &hv_DisplayParam);
  SetDictTuple((*hv_DisplayData), "display_param", hv_DisplayParam);
  GetDictTuple(hv_DisplayParam, "enabled", &hv_DisplayEnabled);
  SetDictTuple((*hv_DisplayData), "enabled", hv_DisplayEnabled);
  //
  //Set default values if not set by the user.
  //
  //Default interval (in epochs) for the preview update
  //depending on the model type.
  if (0 != (int(hv_ModelType==HTuple("classification"))))
  {
    hv_UpdateImagesIntervalEpochs = 4;
  }
  else
  {
    hv_UpdateImagesIntervalEpochs = 0.5;
  }
  //
  //Default number of images to display in the images preview.
  if (0 != (int(hv_ModelType==HTuple("classification"))))
  {
    hv_NumImagesPreview = 6;
  }
  else if (0 != (int(hv_ModelType==HTuple("detection"))))
  {
    hv_NumImagesPreview = 2;
  }
  else if (0 != (int(hv_ModelType==HTuple("segmentation"))))
  {
    hv_NumImagesPreview = 4;
  }
  //
  //Default interval (in seconds) to switch between plots.
  hv_ChangePlotIntervalSeconds = 10;
  //
  //Default percentage of images that is used for evaluation on training set.
  //If set to zero no evaluation on training set is done.
  hv_SelectedPercentageTrainSamples = 0;
  //
  //Default x_axis_label: Show 'iterations' or 'epochs' as x-values in plots.
  hv_XAxisLabel = "epochs";
  //
  //Collect all display params and overwrite if user values are given.
  //Note, some parameters are also used for the evaluation.
  //Thus, write its value even for DisplayEnabled = false.
  hv_DisplayParamNames.Clear();
  hv_DisplayParamNames[0] = "change_plot_interval_seconds";
  hv_DisplayParamNames[1] = "num_images";
  hv_DisplayParamNames[2] = "selected_percentage_train_samples";
  hv_DisplayParamNames[3] = "update_images_interval_epochs";
  hv_DisplayParamNames[4] = "x_axis_label";
  hv_DisplayDefaultValues.Clear();
  hv_DisplayDefaultValues.Append(hv_ChangePlotIntervalSeconds);
  hv_DisplayDefaultValues.Append(hv_NumImagesPreview);
  hv_DisplayDefaultValues.Append(hv_SelectedPercentageTrainSamples);
  hv_DisplayDefaultValues.Append(hv_UpdateImagesIntervalEpochs);
  hv_DisplayDefaultValues.Append(hv_XAxisLabel);
  {
  HTuple end_val52 = (hv_DisplayParamNames.TupleLength())-1;
  HTuple step_val52 = 1;
  for (hv_Index=0; hv_Index.Continue(end_val52, step_val52); hv_Index += step_val52)
  {
    GetDictParam(hv_DisplayParam, "key_exists", HTuple(hv_DisplayParamNames[hv_Index]), 
        &hv_KeyExists);
    if (0 != (hv_KeyExists.TupleNot()))
    {
      SetDictTuple(hv_DisplayParam, HTuple(hv_DisplayParamNames[hv_Index]), HTuple(hv_DisplayDefaultValues[hv_Index]));
    }
  }
  }
  //
  //Set last_update, which is needed to determine when updates are needed.
  GetDictTuple(hv_TrainParam, "update_interval_seconds", &hv_DisplayIntervalSeconds);
  CountSeconds(&hv_Time);
  SetDictTuple((*hv_DisplayData), "last_update", hv_Time-(hv_DisplayIntervalSeconds*2));
  //
  //Some entries in DisplayParam are also needed in case of disabled display.
  //They are all set yet.
  if (0 != (hv_DisplayEnabled.TupleNot()))
  {
    return;
  }
  //
  //Separate handling for parameters that are specified by tuples.
  //
  //These model parameters are displayed in the text window if available.
  hv_StatusModelParamNames.Clear();
  hv_StatusModelParamNames[0] = "learning_rate";
  hv_StatusModelParamNames[1] = "batch_size";
  hv_StatusModelParamNames[2] = "batch_size_multiplier";
  hv_StatusModelParamNames[3] = "momentum";
  hv_StatusModelParamNames[4] = "weight_prior";
  hv_StatusModelParamNames[5] = "image_dimensions";
  GetDictParam(hv_DisplayParam, "key_exists", "status_model_params", &hv_KeyExists);
  if (0 != (hv_KeyExists.TupleNot()))
  {
    SetDictTuple(hv_DisplayParam, "status_model_params", hv_StatusModelParamNames);
  }
  //
  //
  //Setup and open text window.
  hv_WindowTextWidth = 700;
  hv_WindowTextHeight = 750;
  hv_WindowImagesWidth = 1200-hv_WindowTextWidth;
  hv_WindowImagesHeight = hv_WindowTextHeight;
  hv_WindowBGColor = "light gray";
  //
  SetWindowAttr("background_color",hv_WindowBGColor);
  OpenWindow(0,0,hv_WindowTextWidth,hv_WindowTextHeight,0,"visible","",&hv_WindowHandleText);
  HDevWindowStack::Push(hv_WindowHandleText);
  set_display_font(hv_WindowHandleText, 16, "mono", "true", "false");
  SetDictTuple((*hv_DisplayData), "window_text", hv_WindowHandleText);
  SetDictTuple((*hv_DisplayData), "window_text_width", hv_WindowTextWidth);
  SetDictTuple((*hv_DisplayData), "window_text_height", hv_WindowTextHeight);
  //
  //Configure images window, which is opened later by another procedure.
  hv_WindowImagesRow = 0;
  hv_WindowImagesCol = hv_WindowTextWidth+10;
  SetDictTuple((*hv_DisplayData), "window_images", HTuple());
  //
  //Set user specified parameters used for the display of tiled images.
  try
  {
    GetDictTuple(hv_DisplayParam, "tiled_param", &hv_TiledParam);
  }
  // catch (Exception) 
  catch (HException &HDevExpDefaultException)
  {
    HDevExpDefaultException.ToHTuple(&hv_Exception);
    CreateDict(&hv_TiledParam);
    SetDictTuple(hv_DisplayParam, "tiled_param", hv_TiledParam);
  }
  //
  //Only set values if they are not already given.
  hv_TiledParamNames.Clear();
  hv_TiledParamNames[0] = "window_row";
  hv_TiledParamNames[1] = "window_col";
  hv_TiledParamNames[2] = "window_width";
  hv_TiledParamNames[3] = "window_height";
  hv_TiledDefaultValues.Clear();
  hv_TiledDefaultValues.Append(hv_WindowImagesRow);
  hv_TiledDefaultValues.Append(hv_WindowImagesCol);
  hv_TiledDefaultValues.Append(hv_WindowImagesWidth);
  hv_TiledDefaultValues.Append(hv_WindowImagesHeight);
  {
  HTuple end_val109 = (hv_TiledParamNames.TupleLength())-1;
  HTuple step_val109 = 1;
  for (hv_Index=0; hv_Index.Continue(end_val109, step_val109); hv_Index += step_val109)
  {
    GetDictParam(hv_TiledParam, "key_exists", HTuple(hv_TiledParamNames[hv_Index]), 
        &hv_KeyExists);
    if (0 != (hv_KeyExists.TupleNot()))
    {
      SetDictTuple(hv_TiledParam, HTuple(hv_TiledParamNames[hv_Index]), HTuple(hv_TiledDefaultValues[hv_Index]));
    }
  }
  }
  //
  //Set specific display parameters for all available model types.
  if (0 != (int(hv_ModelType==HTuple("classification"))))
  {
    SetDictTuple(hv_TiledParam, "display_input", 0);
    SetDictTuple(hv_TiledParam, "display_ground_truth", 0);
    SetDictTuple(hv_TiledParam, "display_legend", 0);
  }
  //
  //
  //Start with loss plot since usually no evaluation is available in the beginning.
  SetDictTuple((*hv_DisplayData), "last_change_plot", hv_Time);
  SetDictTuple((*hv_DisplayData), "plot_eval", 0);
  HDevWindowStack::SetActive(hv_WindowHandleText);
  //
  return;
}

// Chapter: Graphics / Output
// Short Description: Display a color bar next to an image. 
void dev_display_map_color_bar (HTuple hv_ImageWidth, HTuple hv_ImageHeight, HTuple hv_MapColorBarWidth, 
    HTuple hv_Colors, HTuple hv_MaxValue, HTuple hv_WindowImageRatio, HTuple hv_WindowHandle)
{

  // Local iconic variables
  HObject  ho_Rectangle;

  // Local control variables
  HTuple  hv_ClipRegion, hv_ColorIndex, hv_RectHeight;
  HTuple  hv_DrawMode, hv_Row, hv_Row1, hv_Column1, hv_Row2;
  HTuple  hv_Column2, hv__, hv_TextHeight, hv_Index, hv_Text;

  //
  //This procedure displays a color bar next to the image
  //specified with ImageWidth and ImageHeight.
  //
  GetSystem("clip_region", &hv_ClipRegion);
  SetSystem("clip_region", "false");
  //
  //Display the color bar.
  hv_ColorIndex = 0;
  hv_RectHeight = (1.0*hv_ImageHeight)/(hv_Colors.TupleLength());
  //Set draw mode to fill
  GetDraw(hv_WindowHandle, &hv_DrawMode);
  if (HDevWindowStack::IsOpen())
    SetDraw(HDevWindowStack::GetActive(),"fill");
  {
  HTuple end_val13 = 0;
  HTuple step_val13 = -hv_RectHeight;
  for (hv_Row=hv_ImageHeight-1; hv_Row.Continue(end_val13, step_val13); hv_Row += step_val13)
  {
    //The color bar consists of multiple rectangle1.
    hv_Row1 = hv_Row-hv_RectHeight;
    hv_Column1 = hv_ImageWidth+(20/hv_WindowImageRatio);
    hv_Row2 = hv_Row;
    hv_Column2 = (hv_ImageWidth+20)+(hv_MapColorBarWidth/hv_WindowImageRatio);
    GenRectangle1(&ho_Rectangle, hv_Row1, hv_Column1, hv_Row2, hv_Column2);
    if (HDevWindowStack::IsOpen())
      SetColor(HDevWindowStack::GetActive(),HTuple(hv_Colors[hv_ColorIndex]));
    if (HDevWindowStack::IsOpen())
      DispObj(ho_Rectangle, HDevWindowStack::GetActive());
    hv_ColorIndex += 1;
  }
  }
  //
  //Display labels for color bar.
  GetStringExtents(hv_WindowHandle, "0123456789", &hv__, &hv__, &hv__, &hv_TextHeight);
  for (hv_Index=0; hv_Index<=1; hv_Index+=0.2)
  {
    hv_Text = (hv_MaxValue-(hv_Index*hv_MaxValue)).TupleString(".1f");
    if (HDevWindowStack::IsOpen())
      DispText(HDevWindowStack::GetActive(),hv_Text, "image", hv_Index*(hv_ImageHeight-(2*(hv_TextHeight/hv_WindowImageRatio))), 
          hv_ImageWidth+(40/hv_WindowImageRatio), "black", "box", "false");
  }
  //
  SetSystem("clip_region", hv_ClipRegion);
  if (HDevWindowStack::IsOpen())
    SetDraw(HDevWindowStack::GetActive(),hv_DrawMode);
  return;
}

// Chapter: Graphics / Output
// Short Description: Display a matrix inside a window 
void dev_display_matrix (HTuple hv_Matrix, HTuple hv_ColumnNames, HTuple hv_RowNames, 
    HTuple hv_WindowHandle, HTuple hv_GenParam)
{

  // Local iconic variables

  // Local control variables
  HTuple  hv_NumRows, hv_NumColumns, hv_DisplayColumnWidth;
  HTuple  hv_DisplayColor, hv_IdxsNoDisplay, hv_ValueColors;
  HTuple  hv_NumberFormat, hv_TitleExists, hv_Title, hv_SubtitleExists;
  HTuple  hv_Subtitle, hv_AxisTitlesExists, hv_AxisTitles;
  HTuple  hv_AxisColor, hv_GenParamKeys, hv_GenParamIndex;
  HTuple  hv_ValueTmp, hv_ColStringWidths, hv_AllNames, hv_MaxStringWidth;
  HTuple  hv_StringIndex, hv_String, hv_Ascent, hv_Descent;
  HTuple  hv_StringWidth, hv_StringHeight, hv_RowSeparator;
  HTuple  hv_RowStart, hv_TitleHeight, hv_TitleWidth, hv_SubtitleHeight;
  HTuple  hv_SubtitleWidth, hv_AxisTitleHeight, hv_TitleAscent;
  HTuple  hv_TitleDescent, hv_TitleRowStart, hv_SubtitleAscent;
  HTuple  hv_SubtitleDescent, hv_SubtitleRowStart, hv_AxisTitleAscent;
  HTuple  hv_AxisTitleDescent, hv_AxisTitleWidth, hv_AxisTitleRowStart;
  HTuple  hv_LabelRowStart, hv_RowDistance, hv_RowEnd, hv_ColumnStart;
  HTuple  hv_ColumnOffset, hv_ColumnEnd, hv_Width, hv_Height;
  HTuple  hv_WidthLimit, hv_HeightLimit, hv_TextRow, hv_TextColumn;
  HTuple  hv_RowStartTmp, hv_ColumnDistance, hv_Index, hv_MatrixText;
  HTuple  hv_IdxsDisplay;

  //
  //Display a matrix in a window.
  //
  //GenParam (all are optional):
  //  - display_column_width: Either 'equal' or 'minimal'. For 'equal', all columns have the same width
  //                          that equals the width of the longest row or column-name.
  //  - display_color:        If true, the values will be displayed with the colors that are given by
  //                          'value_colors'.
  //                          Else, all values are displayed in white.
  //  - idxs_no_display:      Indices of the matrix that should not be displayed. The indices should be
  //                          given in row-major order, according to get_full_matrix. If not given or if
  //                          set to an empty tuple, all matrix values are displayed.
  //  - axis_titles:          Titles for the matrix x- and y-axis that are displayed at the top-right
  //                          and bottom-left of the matrix, respectively. Should be a tuple of length
  //                          two or an empty tuple if no axis titles should be displayed.
  //  - value_colors:         Tuple containing the hex color-string for each value of the matrix. If a
  //                          single value is given, this color is used for all values, else it must have
  //                          the length of the number of values in the matrix. Is ignored if
  //                          display_color is false.
  //  - number_format:        Format of the numbers when they are converted to strings for display.
  //                          Default: 2.0f
  //  - title:                Title of the displayed matrix, if given shown in the top-left.
  //  - subtitle:             Subtitle of the displayed matrix, if given shown beyond the title.
  //  - row_col_names_color:  Color of column and row names. Default: 'light gray'
  //
  //
  //Get the matrix dimensions.
  GetSizeMatrix(hv_Matrix, &hv_NumRows, &hv_NumColumns);
  //Set defaults parameters.
  hv_DisplayColumnWidth = "equal";
  hv_DisplayColor = 0;
  hv_IdxsNoDisplay = HTuple();
  hv_ValueColors = HTuple(hv_NumRows*hv_NumColumns,"white");
  hv_NumberFormat = "2.0f";
  hv_TitleExists = 0;
  hv_Title = HTuple();
  hv_SubtitleExists = 0;
  hv_Subtitle = HTuple();
  hv_AxisTitlesExists = 0;
  hv_AxisTitles = HTuple();
  hv_AxisColor = "light gray";
  //Get parameters and overwrite
  //
  hv_GenParamKeys = HTuple();
  if (0 != (int(hv_GenParam!=HTuple())))
  {
    GetDictParam(hv_GenParam, "keys", HTuple(), &hv_GenParamKeys);
  }
  {
  HTuple end_val47 = (hv_GenParamKeys.TupleLength())-1;
  HTuple step_val47 = 1;
  for (hv_GenParamIndex=0; hv_GenParamIndex.Continue(end_val47, step_val47); hv_GenParamIndex += step_val47)
  {
    //DisplayColumnWidth.
    if (0 != (int(HTuple(hv_GenParamKeys[hv_GenParamIndex])==HTuple("display_column_width"))))
    {
      GetDictTuple(hv_GenParam, "display_column_width", &hv_ValueTmp);
      if (0 != (HTuple(int((hv_ValueTmp.TupleLength())!=1)).TupleOr(int(((HTuple("equal").Append("minimal")).TupleFind(hv_ValueTmp))==-1))))
      {
        throw HException("Invalid value for 'display_column_width'");
      }
      else
      {
        hv_DisplayColumnWidth = hv_ValueTmp;
      }
      //DisplayColor.
    }
    else if (0 != (int(HTuple(hv_GenParamKeys[hv_GenParamIndex])==HTuple("display_color"))))
    {
      GetDictTuple(hv_GenParam, "display_color", &hv_ValueTmp);
      if (0 != (HTuple(int((hv_ValueTmp.TupleLength())!=1)).TupleOr(int(((HTuple(1).Append(0)).TupleFind(hv_ValueTmp))==-1))))
      {
        throw HException("Invalid value for 'display_color'");
      }
      else
      {
        hv_DisplayColor = hv_ValueTmp;
      }
      //ValueIdxsNoDisplay.
    }
    else if (0 != (int(HTuple(hv_GenParamKeys[hv_GenParamIndex])==HTuple("idxs_no_display"))))
    {
      GetDictTuple(hv_GenParam, "idxs_no_display", &hv_ValueTmp);
      if (0 != (int((hv_ValueTmp.TupleLength())>0)))
      {
        if (0 != (HTuple(HTuple(HTuple(int((hv_ValueTmp.TupleMin())<0)).TupleOr(int((hv_ValueTmp.TupleMax())>((hv_NumRows*hv_NumColumns)-1)))).TupleOr((hv_ValueTmp.TupleIsInt()).TupleNot())).TupleOr(int((hv_ValueTmp.TupleLength())>(hv_NumRows*hv_NumColumns)))))
        {
          throw HException("Invalid value for 'idxs_no_display'");
        }
        hv_IdxsNoDisplay = hv_ValueTmp;
      }
      //ValueColors.
    }
    else if (0 != (int(HTuple(hv_GenParamKeys[hv_GenParamIndex])==HTuple("value_colors"))))
    {
      if (0 != hv_DisplayColor)
      {
        GetDictTuple(hv_GenParam, "value_colors", &hv_ValueTmp);
        if (0 != (int((hv_ValueTmp.TupleLength())>0)))
        {
          if (0 != (HTuple(HTuple(int((hv_ValueTmp.TupleLength())!=(hv_NumColumns*hv_NumRows))).TupleAnd(int((hv_ValueTmp.TupleLength())!=1))).TupleOr((hv_ValueTmp.TupleIsString()).TupleNot())))
          {
            throw HException("Invalid value for 'value_colors'");
          }
          if (0 != (int((hv_ValueTmp.TupleLength())==1)))
          {
            hv_ValueTmp = HTuple(hv_NumRows*hv_NumColumns,hv_ValueTmp);
          }
          hv_ValueColors = hv_ValueTmp;
        }
      }
      //NumberFormat.
    }
    else if (0 != (int(HTuple(hv_GenParamKeys[hv_GenParamIndex])==HTuple("number_format"))))
    {
      GetDictTuple(hv_GenParam, "number_format", &hv_ValueTmp);
      if (0 != (HTuple(int((hv_ValueTmp.TupleLength())!=1)).TupleOr((hv_ValueTmp.TupleIsString()).TupleNot())))
      {
        throw HException("Invalid value for 'number_format'");
      }
      else
      {
        hv_NumberFormat = hv_ValueTmp;
      }
      //Title.
    }
    else if (0 != (int(HTuple(hv_GenParamKeys[hv_GenParamIndex])==HTuple("title"))))
    {
      GetDictTuple(hv_GenParam, "title", &hv_ValueTmp);
      if (0 != (HTuple(int((hv_ValueTmp.TupleLength())!=1)).TupleOr((hv_ValueTmp.TupleIsString()).TupleNot())))
      {
        throw HException("Invalid value for 'title'");
      }
      else
      {
        hv_TitleExists = 1;
        hv_Title = hv_ValueTmp;
      }
      //Subtitle.
    }
    else if (0 != (int(HTuple(hv_GenParamKeys[hv_GenParamIndex])==HTuple("subtitle"))))
    {
      GetDictTuple(hv_GenParam, "subtitle", &hv_ValueTmp);
      if (0 != (HTuple(int((hv_ValueTmp.TupleLength())!=1)).TupleOr((hv_ValueTmp.TupleIsString()).TupleNot())))
      {
        throw HException("Invalid value for 'subtitle'");
      }
      else
      {
        hv_SubtitleExists = 1;
        hv_Subtitle = hv_ValueTmp;
      }
      //AxisTitles.
    }
    else if (0 != (int(HTuple(hv_GenParamKeys[hv_GenParamIndex])==HTuple("axis_titles"))))
    {
      GetDictTuple(hv_GenParam, "axis_titles", &hv_ValueTmp);
      if (0 != (int((hv_ValueTmp.TupleLength())>0)))
      {
        if (0 != (HTuple(int((hv_ValueTmp.TupleLength())>2)).TupleOr(int((hv_ValueTmp.TupleIsStringElem())!=HTuple(hv_ValueTmp.TupleLength(),1)))))
        {
          throw HException("Invalid value for 'axis_titles'");
        }
        hv_AxisTitlesExists = 1;
        hv_AxisTitles = hv_ValueTmp;
      }
      //Color for row and column names
    }
    else if (0 != (int(HTuple(hv_GenParamKeys[hv_GenParamIndex])==HTuple("row_col_names_color"))))
    {
      GetDictTuple(hv_GenParam, "row_col_names_color", &hv_ValueTmp);
      if (0 != (HTuple(int((hv_ValueTmp.TupleLength())!=1)).TupleOr((hv_ValueTmp.TupleIsString()).TupleNot())))
      {
        throw HException("Invalid value for 'row_col_names_color'");
      }
      else
      {
        hv_AxisColor = hv_ValueTmp;
      }
    }
  }
  }
  //
  //Calculate the width of row and column-name-strings.
  hv_ColStringWidths = HTuple();
  hv_AllNames.Clear();
  hv_AllNames.Append(hv_ColumnNames);
  hv_AllNames.Append(hv_RowNames);
  hv_MaxStringWidth = 0;
  {
  HTuple end_val138 = (hv_AllNames.TupleLength())-1;
  HTuple step_val138 = 1;
  for (hv_StringIndex=0; hv_StringIndex.Continue(end_val138, step_val138); hv_StringIndex += step_val138)
  {
    hv_String = HTuple(hv_AllNames[hv_StringIndex]);
    GetStringExtents(hv_WindowHandle, hv_String, &hv_Ascent, &hv_Descent, &hv_StringWidth, 
        &hv_StringHeight);
    if (0 != (int(hv_StringIndex<(hv_ColumnNames.TupleLength()))))
    {
      hv_ColStringWidths = hv_ColStringWidths.TupleConcat(hv_StringWidth);
    }
    hv_MaxStringWidth = hv_MaxStringWidth.TupleMax2(hv_StringWidth);
  }
  }
  //The columns should have a minimum width for 4 characters.
  GetStringExtents(hv_WindowHandle, "test", &hv_Ascent, &hv_Descent, &hv_StringWidth, 
      &hv_StringHeight);
  //Update the maximum string width.
  hv_MaxStringWidth = hv_MaxStringWidth.TupleMax2(hv_StringWidth);
  //
  //Set parameters for nice display, e.g., distances from window border, row- and column-offsets,
  //axis titles and title.
  //Begin
  //Title
  //Subtitle
  //                   AxisLabel
  //         Class 1     Class n
  //Label 1  a(1,1) ...  a(1,n)
  //                ...
  //Labeln m a(m,1) ...  a(m,n)
  //End
  //Thereby entries are vertically separated by RowSeparator
  hv_RowSeparator = 10;
  hv_RowStart = hv_StringHeight+hv_RowSeparator;
  hv_TitleHeight = 0;
  hv_TitleWidth = 0;
  hv_SubtitleHeight = 0;
  hv_SubtitleWidth = 0;
  hv_AxisTitleHeight = 0;
  if (0 != (int((hv_Title.TupleLength())>0)))
  {
    GetStringExtents(hv_WindowHandle, hv_Title, &hv_TitleAscent, &hv_TitleDescent, 
        &hv_TitleWidth, &hv_TitleHeight);
    hv_TitleRowStart = hv_RowStart;
    hv_RowStart = (hv_RowStart+hv_TitleHeight)+hv_RowSeparator;
  }
  if (0 != (int((hv_Subtitle.TupleLength())>0)))
  {
    GetStringExtents(hv_WindowHandle, hv_Subtitle, &hv_SubtitleAscent, &hv_SubtitleDescent, 
        &hv_SubtitleWidth, &hv_SubtitleHeight);
    hv_SubtitleRowStart = hv_RowStart;
    hv_RowStart = (hv_RowStart+hv_SubtitleHeight)+hv_RowSeparator;
  }
  hv_RowStart = (hv_RowStart+hv_StringHeight)+hv_RowSeparator;
  if (0 != (hv_AxisTitles.TupleLength()))
  {
    GetStringExtents(hv_WindowHandle, hv_AxisTitles, &hv_AxisTitleAscent, &hv_AxisTitleDescent, 
        &hv_AxisTitleWidth, &hv_AxisTitleHeight);
    hv_AxisTitleRowStart = hv_RowStart;
    hv_RowStart = (hv_RowStart+hv_AxisTitleHeight)+hv_RowSeparator;
  }
  //Add space for label names
  hv_LabelRowStart = hv_RowStart;
  hv_RowStart = (hv_RowStart+hv_StringHeight)+hv_RowSeparator;
  hv_RowDistance = hv_StringHeight+hv_RowSeparator;
  hv_RowEnd = (hv_StringHeight*2)+(hv_RowSeparator*2);
  hv_ColumnStart = 50+hv_MaxStringWidth;
  hv_ColumnOffset = 30;
  hv_ColumnEnd = hv_ColumnOffset;
  //
  //Adapt the window size to fit the confusion matrix.
  if (0 != (int(hv_DisplayColumnWidth==HTuple("minimal"))))
  {
    //Every column of the confusion matrix is as narrow as possible
    //based to the respective string widths.
    hv_Width = (((hv_ColStringWidths.TupleSum())+(hv_ColumnOffset*hv_NumColumns))+hv_ColumnStart)+hv_ColumnEnd;
  }
  else if (0 != (int(hv_DisplayColumnWidth==HTuple("equal"))))
  {
    //Every column of the confusion matrix should have the same width.
    //based on the maximum string width.
    hv_Width = (((hv_MaxStringWidth+hv_ColumnOffset)*hv_NumColumns)+hv_ColumnStart)+hv_ColumnEnd;
  }
  //Make sure, window is large enough for the title / subtitle:
  hv_Width = hv_Width.TupleMax2((hv_TitleWidth.TupleMax2(hv_SubtitleWidth))+(2*hv_ColumnOffset));
  hv_Height = ((hv_RowDistance*hv_NumRows)+hv_RowStart)+hv_RowEnd;
  HDevWindowStack::SetActive(hv_WindowHandle);
  if (HDevWindowStack::IsOpen())
    ClearWindow(HDevWindowStack::GetActive());
  //
  //Set reasonable limits for graphics window (adapt if necessary).
  hv_WidthLimit.Clear();
  hv_WidthLimit[0] = 450;
  hv_WidthLimit.Append(hv_Width);
  hv_HeightLimit.Clear();
  hv_HeightLimit[0] = 250;
  hv_HeightLimit.Append(hv_Height);
  dev_resize_window_fit_size(0, 0, hv_Width, hv_Height, hv_WidthLimit, hv_HeightLimit);
  //
  //Calculate row and column coordinates for display of values.
  hv_TextRow = HTuple();
  hv_TextColumn = HTuple();
  hv_RowStartTmp = 0;
  hv_ColumnDistance.Clear();
  hv_ColumnDistance[0] = 0;
  hv_ColumnDistance.Append(((hv_ColStringWidths.TupleSelectRange(0,hv_NumColumns-2))+hv_ColumnOffset).TupleCumul());
  if (0 != (int(hv_DisplayColumnWidth==HTuple("equal"))))
  {
    hv_ColumnDistance = HTuple::TupleGenSequence(0,hv_NumColumns-1,1)*(hv_MaxStringWidth+hv_ColumnOffset);
  }
  {
  HTuple end_val224 = hv_NumRows-1;
  HTuple step_val224 = 1;
  for (hv_Index=0; hv_Index.Continue(end_val224, step_val224); hv_Index += step_val224)
  {
    hv_TextRow = hv_TextRow.TupleConcat(HTuple(hv_NumColumns,hv_RowStartTmp));
    hv_RowStartTmp += hv_RowDistance;
    hv_TextColumn = hv_TextColumn.TupleConcat(hv_ColumnDistance);
  }
  }
  //Display the confusion matrix with a margin from the top and left.
  hv_TextColumn += hv_ColumnStart;
  hv_TextRow += hv_RowStart;
  //
  //Display title.
  if (0 != (int((hv_Title.TupleLength())>0)))
  {
    if (HDevWindowStack::IsOpen())
      DispText(HDevWindowStack::GetActive(),hv_Title, "window", hv_TitleRowStart, 
          "left", "white", "box", "false");
  }
  //Display subtitle.
  if (0 != (int((hv_Subtitle.TupleLength())>0)))
  {
    if (HDevWindowStack::IsOpen())
      DispText(HDevWindowStack::GetActive(),hv_Subtitle, "window", hv_SubtitleRowStart, 
          "left", "white", "box", "false");
  }
  //Display axis.
  if (0 != (int((hv_AxisTitles.TupleLength())>0)))
  {
    if (HDevWindowStack::IsOpen())
      DispText(HDevWindowStack::GetActive(),HTuple(hv_AxisTitles[0]), "window", hv_AxisTitleRowStart, 
          "right", "white", "box", "false");
    if (HDevWindowStack::IsOpen())
      DispText(HDevWindowStack::GetActive(),HTuple(hv_AxisTitles[1]), "window", ((hv_TextRow.TupleMax())+hv_StringHeight)+hv_RowSeparator, 
          "left", "white", "box", "false");
  }
  //
  //Display row names.
  if (HDevWindowStack::IsOpen())
    DispText(HDevWindowStack::GetActive(),hv_RowNames, "window", HTuple(hv_TextRow[HTuple::TupleGenSequence(0,hv_NumColumns*(hv_NumRows-1),hv_NumColumns)]), 
        HTuple(hv_NumRows,(HTuple(hv_TextColumn[0])-hv_MaxStringWidth)-hv_ColumnOffset), 
        hv_AxisColor, "box", "false");
  //
  //Display column names.
  if (HDevWindowStack::IsOpen())
    DispText(HDevWindowStack::GetActive(),hv_ColumnNames, "window", HTuple(hv_NumColumns,hv_LabelRowStart), 
        hv_TextColumn.TupleSelectRange(0,hv_NumColumns-1), hv_AxisColor, "box", "false");
  //
  //
  //Align the numbers right.
  GetFullMatrix(hv_Matrix, &hv_MatrixText);
  hv_MatrixText = hv_MatrixText.TupleString(hv_NumberFormat);
  //
  //
  //Handle no-display indices.
  hv_IdxsDisplay = HTuple::TupleGenSequence(0,(hv_MatrixText.TupleLength())-1,1).TupleDifference(hv_IdxsNoDisplay);
  //
  //Display confusion matrix.
  if (HDevWindowStack::IsOpen())
    DispText(HDevWindowStack::GetActive(),HTuple(hv_MatrixText[hv_IdxsDisplay]), 
        "window", HTuple(hv_TextRow[hv_IdxsDisplay]), HTuple(hv_TextColumn[hv_IdxsDisplay]), 
        HTuple(hv_ValueColors[hv_IdxsDisplay]), "box", "false");
  return;
}

// Chapter: Graphics / Output
// Short Description: This procedure draws a pie chart into the active window handle. 
void dev_display_pie_chart (HTuple hv_WindowHandle, HTuple hv_Ratios, HTuple hv_Row, 
    HTuple hv_Column, HTuple hv_Radius, HTuple hv_Colors, HTuple hv_GenParam)
{

  // Local iconic variables
  HObject  ho_Sectors, ho_Arc, ho_Sector;

  // Local control variables
  HTuple  hv_Tolerance, hv_InitialWindowHandle;
  HTuple  hv_NoInitialWindow, hv_Red, hv_Green, hv_Blue, hv_Alpha;
  HTuple  hv_InitialColor, hv_IndexColor, hv_Color_RGBA, hv_Title;
  HTuple  hv_TitleColor, hv_TitleKeyExists, hv_TitleColorKeyExists;
  HTuple  hv_GivenTitle, hv_GivenTitleColor, hv_Exception;
  HTuple  hv_FullCircleAngle, hv_StartAngle, hv_DrawColors;
  HTuple  hv_K, hv_EndAngle, hv_Rows, hv_Columns, hv_HomMat2DIdentity;
  HTuple  hv_HomMat2DRotate, hv_ContourStyle, hv_Ascent, hv_Descent;
  HTuple  hv_Width, hv_Height;

  //
  //Draw a pie-chart into the active window.
  //
  //Check input Ratios.
  if (0 != (int(((hv_Ratios.TupleLessElem(0.0)).TupleSum())>0)))
  {
    throw HException("Values of parameter Ratios must be positive.");
  }
  hv_Tolerance = 0.1;
  if (0 != (int(((1-(hv_Ratios.TupleSum())).TupleAbs())>=hv_Tolerance)))
  {
    throw HException("Sum of Ratios differs too much from 1.");
  }
  //
  //Get current window handle.
  if (HDevWindowStack::IsOpen())
    hv_InitialWindowHandle = HDevWindowStack::GetActive();
  //In case there is no open window, no initial values are to be reset.
  hv_NoInitialWindow = 0;
  if (0 != (int(hv_InitialWindowHandle==-1)))
  {
    hv_NoInitialWindow = 1;
  }
  else
  {
    //Get current set color.
    GetRgba(hv_InitialWindowHandle, &hv_Red, &hv_Green, &hv_Blue, &hv_Alpha);
    hv_InitialColor = HTuple();
    {
    HTuple end_val22 = (hv_Red.TupleLength())-1;
    HTuple step_val22 = 1;
    for (hv_IndexColor=0; hv_IndexColor.Continue(end_val22, step_val22); hv_IndexColor += step_val22)
    {
      hv_Color_RGBA = ((("#"+(HTuple(hv_Red[hv_IndexColor]).TupleString("2x")))+(HTuple(hv_Green[hv_IndexColor]).TupleString("2x")))+(HTuple(hv_Blue[hv_IndexColor]).TupleString("2x")))+(HTuple(hv_Alpha[hv_IndexColor]).TupleString("2x"));
      TupleRegexpReplace(hv_Color_RGBA, (HTuple(" ").Append("replace_all")), "0", 
          &hv_Color_RGBA);
      hv_InitialColor = hv_InitialColor.TupleConcat(hv_Color_RGBA);
    }
    }
  }
  HDevWindowStack::SetActive(hv_WindowHandle);
  //
  //In case no segment colors are specified, generate random colors.
  if (0 != (int((hv_Colors.TupleLength())==0)))
  {
    //Generate random colors.
    get_distinct_colors_dev_display_pie_chart(hv_Ratios.TupleLength(), 1, 0, 220, 
        &hv_Colors);
  }
  //
  //Set default parameters.
  hv_Title = HTuple();
  hv_TitleColor = "white";
  //Get generic parameters.
  if (0 != (int((hv_GenParam.TupleLength())>0)))
  {
    GetDictParam(hv_GenParam, "key_exists", "title", &hv_TitleKeyExists);
    GetDictParam(hv_GenParam, "key_exists", "title_color", &hv_TitleColorKeyExists);
    //
    //Title text.
    if (0 != hv_TitleKeyExists)
    {
      GetDictTuple(hv_GenParam, "title", &hv_GivenTitle);
      if (0 != (HTuple((hv_GivenTitle.TupleIsString()).TupleNot()).TupleOr(int((hv_GivenTitle.TupleStrlen())<1))))
      {
        throw HException("Invalid value for key 'title'");
      }
      hv_Title = hv_GivenTitle;
    }
    //Title color.
    if (0 != hv_TitleColorKeyExists)
    {
      GetDictTuple(hv_GenParam, "title_color", &hv_GivenTitleColor);
      if (0 != ((hv_GivenTitleColor.TupleIsString()).TupleNot()))
      {
        throw HException("Invalid value for key 'title_color'");
      }
      try
      {
        if (HDevWindowStack::IsOpen())
          SetColor(HDevWindowStack::GetActive(),hv_GivenTitleColor);
      }
      // catch (Exception) 
      catch (HException &HDevExpDefaultException)
      {
        HDevExpDefaultException.ToHTuple(&hv_Exception);
        throw HException("Invalid value for key 'title_color'");
      }
      hv_TitleColor = hv_GivenTitleColor;
    }
  }
  hv_FullCircleAngle = 2*3.14159;
  hv_StartAngle = 0;
  //
  //Generate circle-sectors.
  hv_Ratios = hv_Ratios/(hv_Ratios.TupleSum());
  hv_DrawColors = HTuple();
  GenEmptyObj(&ho_Sectors);
  {
  HTuple end_val73 = (hv_Ratios.TupleLength())-1;
  HTuple step_val73 = 1;
  for (hv_K=0; hv_K.Continue(end_val73, step_val73); hv_K += step_val73)
  {
    if (0 != (int(HTuple(hv_Ratios[hv_K])>0)))
    {
      hv_EndAngle = hv_StartAngle+(HTuple(hv_Ratios[hv_K])*hv_FullCircleAngle);
      GenCircleContourXld(&ho_Arc, hv_Row, hv_Column, hv_Radius, hv_StartAngle, hv_EndAngle, 
          "positive", 1);
      GetContourXld(ho_Arc, &hv_Rows, &hv_Columns);
      GenContourPolygonXld(&ho_Sector, (hv_Row.TupleConcat(hv_Rows)).TupleConcat(hv_Row), 
          (hv_Column.TupleConcat(hv_Columns)).TupleConcat(hv_Column));
      hv_StartAngle = hv_EndAngle;
      hv_DrawColors = hv_DrawColors.TupleConcat(HTuple(hv_Colors[hv_K]));
      ConcatObj(ho_Sectors, ho_Sector, &ho_Sectors);
    }
  }
  }
  //Rotate 90 degrees such that the first sector starts at the top.
  HomMat2dIdentity(&hv_HomMat2DIdentity);
  HomMat2dRotate(hv_HomMat2DIdentity, 0.25*hv_FullCircleAngle, hv_Row, hv_Column, 
      &hv_HomMat2DRotate);
  AffineTransContourXld(ho_Sectors, &ho_Sectors, hv_HomMat2DRotate);
  //
  //Display sectors.
  HDevWindowStack::SetActive(hv_WindowHandle);
  GetContourStyle(hv_WindowHandle, &hv_ContourStyle);
  if (HDevWindowStack::IsOpen())
    SetContourStyle(HDevWindowStack::GetActive(),"stroke_and_fill");
  if (HDevWindowStack::IsOpen())
    SetColor(HDevWindowStack::GetActive(),hv_DrawColors);
  if (HDevWindowStack::IsOpen())
    DispObj(ho_Sectors, HDevWindowStack::GetActive());
  //(Optional) title.
  if (0 != (int(hv_Title!=HTuple())))
  {
    GetStringExtents(hv_WindowHandle, hv_Title, &hv_Ascent, &hv_Descent, &hv_Width, 
        &hv_Height);
    if (HDevWindowStack::IsOpen())
      DispText(HDevWindowStack::GetActive(),hv_Title, "image", (((hv_Row-hv_Radius)-hv_Height)-hv_Ascent)-hv_Descent, 
          hv_Column-(hv_Width/2), hv_TitleColor, "box", "false");
  }
  if (HDevWindowStack::IsOpen())
    SetContourStyle(HDevWindowStack::GetActive(),hv_ContourStyle);
  //
  //Reset original window and colors (and close the window in case there was none open).
  if (0 != hv_NoInitialWindow)
  {
    if (HDevWindowStack::IsOpen())
      CloseWindow(HDevWindowStack::Pop());
  }
  else
  {
    HDevWindowStack::SetActive(hv_InitialWindowHandle);
    if (HDevWindowStack::IsOpen())
      SetColor(HDevWindowStack::GetActive(),hv_InitialColor);
  }
  return;
}

// Chapter: Deep Learning / Anomaly Detection
// Short Description: Display the detected anomaly regions. 
void dev_display_result_anomaly_regions (HObject ho_AnomalyRegion, HTuple hv_CurrentWindowHandle, 
    HTuple hv_LineWidth, HTuple hv_AnomalyRegionResultColor, HTuple* hv_WindowHandle)
{

  // Local iconic variables

  // Local control variables
  HTuple  hv_Red, hv_Green, hv_Blue, hv_Alpha, hv_InitialColor;
  HTuple  hv_IndexColor, hv_Color_RGBA, hv_Area;
  
  //
  //This procedure displays the result anomaly regions.
  //
  //Get current set color.
  if (0 != hv_WindowHandle)
  {
      GetRgba(*hv_WindowHandle, &hv_Red, &hv_Green, &hv_Blue, &hv_Alpha);
  }
  else
  {
      HTuple hv_Length;
      TupleLength(hv_CurrentWindowHandle, &hv_Length);
      if (hv_Length.I() > 0)
      {
          GetRgba(hv_CurrentWindowHandle, &hv_Red, &hv_Green, &hv_Blue, &hv_Alpha);
      }
      else
      {
          hv_Red = 255;
          hv_Green = 255;
          hv_Blue = 255;
          hv_Alpha = 255;
      }
  }
  hv_InitialColor = HTuple();
  {
  HTuple end_val6 = (hv_Red.TupleLength())-1;
  HTuple step_val6 = 1;
  for (hv_IndexColor=0; hv_IndexColor.Continue(end_val6, step_val6); hv_IndexColor += step_val6)
  {
    hv_Color_RGBA = ((("#"+(HTuple(hv_Red[hv_IndexColor]).TupleString("2x")))+(HTuple(hv_Green[hv_IndexColor]).TupleString("2x")))+(HTuple(hv_Blue[hv_IndexColor]).TupleString("2x")))+(HTuple(hv_Alpha[hv_IndexColor]).TupleString("2x"));
    TupleRegexpReplace(hv_Color_RGBA, (HTuple(" ").Append("replace_all")), "0", &hv_Color_RGBA);
    hv_InitialColor = hv_InitialColor.TupleConcat(hv_Color_RGBA);
  }
  }
  //
  //Display anomaly regions.
  //Get non-empty regions.
  RegionFeatures(ho_AnomalyRegion, "area", &hv_Area);
  //
  //Display all non-empty class regions in distinct colors.
  if (0 != (int(hv_Area>0)))
  {
      if (0 != hv_WindowHandle)
      {
          SetColor(*hv_WindowHandle, hv_AnomalyRegionResultColor);
          SetLineWidth(*hv_WindowHandle, hv_LineWidth);
          SetDraw(*hv_WindowHandle, "margin");
          DispObj(ho_AnomalyRegion, *hv_WindowHandle);
      }
      else if (HDevWindowStack::IsOpen())
      {
          SetColor(HDevWindowStack::GetActive(), hv_AnomalyRegionResultColor);
          if (HDevWindowStack::IsOpen())
              SetLineWidth(HDevWindowStack::GetActive(), hv_LineWidth);
          if (HDevWindowStack::IsOpen())
              SetDraw(HDevWindowStack::GetActive(), "margin");
          if (HDevWindowStack::IsOpen())
              DispObj(ho_AnomalyRegion, HDevWindowStack::GetActive());
      }
  }
  //
  //Reset colors.
  if (0 != hv_WindowHandle)
  {
    SetColor(*hv_WindowHandle, hv_InitialColor);
  }
  else if (HDevWindowStack::IsOpen())
    SetColor(HDevWindowStack::GetActive(),hv_InitialColor);
  //
  return;
}

// Chapter: Graphics / Output
// Short Description: Display result bounding boxes. 
void dev_display_result_detection (HTuple hv_DLResult, HTuple hv_ResultKeys, HTuple hv_LineWidthBbox, 
    HTuple hv_ClassIDs, HTuple hv_TextConf, HTuple hv_Colors, HTuple hv_BoxLabelColor, 
    HTuple hv_WindowImageRatio, HTuple hv_TextPositionRow, HTuple hv_TextColor, HTuple hv_ShowLabels, 
    HTuple hv_ShowDirection, HTuple hv_WindowHandle, HTuple *hv_BboxClassIndices)
{

  // Local iconic variables
  HObject  ho_BboxRectangle, ho_OrientationArrows;
  HObject  ho_RectangleSelected, ho_ArrowSelected;

  // Local control variables
  HTuple  hv_InstanceType, hv_BboxRow1, hv_BboxCol1;
  HTuple  hv_BboxRow2, hv_BboxCol2, hv_BboxClasses, hv_BboxRow;
  HTuple  hv_BboxCol, hv_BboxLength1, hv_BboxLength2, hv_BboxPhi;
  HTuple  hv_Text, hv_Ascent, hv_Descent, hv__, hv_TextOffset;
  HTuple  hv_LabelRowTop, hv_LabelRowBottom, hv_LabelCol;
  HTuple  hv_HeadSize, hv_ContourStyle, hv_Style, hv_IndexBbox;
  HTuple  hv_ClassID, hv_LineWidth, hv_TextColorClasses, hv_LabelRow;

  //
  //This procedure displays the bounding boxes defined by DLResult.
  //The ClassIDs are necessary to display bounding boxes from the same class
  //always with the same color.
  //
  hv_InstanceType = "rectangle1";
  if (0 != (int((hv_ResultKeys.TupleFind("bbox_row1"))!=-1)))
  {
    GetDictTuple(hv_DLResult, "bbox_row1", &hv_BboxRow1);
    GetDictTuple(hv_DLResult, "bbox_col1", &hv_BboxCol1);
    GetDictTuple(hv_DLResult, "bbox_row2", &hv_BboxRow2);
    GetDictTuple(hv_DLResult, "bbox_col2", &hv_BboxCol2);
    GetDictTuple(hv_DLResult, "bbox_class_id", &hv_BboxClasses);
  }
  else if (0 != (int((hv_ResultKeys.TupleFind("bbox_phi"))!=-1)))
  {
    GetDictTuple(hv_DLResult, "bbox_row", &hv_BboxRow);
    GetDictTuple(hv_DLResult, "bbox_col", &hv_BboxCol);
    GetDictTuple(hv_DLResult, "bbox_length1", &hv_BboxLength1);
    GetDictTuple(hv_DLResult, "bbox_length2", &hv_BboxLength2);
    GetDictTuple(hv_DLResult, "bbox_phi", &hv_BboxPhi);
    GetDictTuple(hv_DLResult, "bbox_class_id", &hv_BboxClasses);
    hv_InstanceType = "rectangle2";
  }
  else
  {
    throw HException("Result bounding box data could not be found in DLResult.");
  }
  //
  if (0 != (int((hv_BboxClasses.TupleLength())>0)))
  {
    //
    //Get text and text size for correct positioning of result class IDs.
    if (0 != hv_ShowLabels)
    {
      hv_Text = hv_BboxClasses+hv_TextConf;
      GetStringExtents(hv_WindowHandle, hv_Text, &hv_Ascent, &hv_Descent, &hv__, 
          &hv__);
      hv_TextOffset = (hv_Ascent+hv_Descent)/hv_WindowImageRatio;
    }
    //
    //Generate bounding box XLDs.
    if (0 != (int(hv_InstanceType==HTuple("rectangle1"))))
    {
      TupleGenConst(hv_BboxRow1.TupleLength(), 0.0, &hv_BboxPhi);
      GenRectangle2ContourXld(&ho_BboxRectangle, 0.5*(hv_BboxRow1+hv_BboxRow2), 0.5*(hv_BboxCol1+hv_BboxCol2), 
          hv_BboxPhi, 0.5*(hv_BboxCol2-hv_BboxCol1), 0.5*(hv_BboxRow2-hv_BboxRow1));
      if (0 != hv_ShowLabels)
      {
        hv_LabelRowTop = hv_BboxRow1;
        hv_LabelRowBottom = hv_BboxRow2-hv_TextOffset;
        hv_LabelCol = hv_BboxCol1;
      }
    }
    else
    {
      GenRectangle2ContourXld(&ho_BboxRectangle, hv_BboxRow, hv_BboxCol, hv_BboxPhi, 
          hv_BboxLength1, hv_BboxLength2);
      if (0 != hv_ShowLabels)
      {
        hv_LabelRowTop = hv_BboxRow-hv_TextOffset;
        hv_LabelRowBottom = hv_BboxRow;
        hv_LabelCol = hv_BboxCol;
      }
      if (0 != hv_ShowDirection)
      {
        hv_HeadSize = 20.0;
        gen_arrow_contour_xld(&ho_OrientationArrows, hv_BboxRow, hv_BboxCol, hv_BboxRow-((hv_BboxLength1+hv_HeadSize)*(hv_BboxPhi.TupleSin())), 
            hv_BboxCol+((hv_BboxLength1+hv_HeadSize)*(hv_BboxPhi.TupleCos())), hv_HeadSize, 
            hv_HeadSize);
      }
    }
    //
    GetContourStyle(hv_WindowHandle, &hv_ContourStyle);
    if (HDevWindowStack::IsOpen())
      SetContourStyle(HDevWindowStack::GetActive(),"stroke");
    GetLineStyle(hv_WindowHandle, &hv_Style);
    if (HDevWindowStack::IsOpen())
      SetLineWidth(HDevWindowStack::GetActive(),hv_LineWidthBbox);
    //
    //Collect ClassIDs of the bounding boxes.
    TupleGenConst(hv_BboxClasses.TupleLength(), 0, &(*hv_BboxClassIndices));
    //
    //Draw bounding boxes.
    {
    HTuple end_val64 = (hv_BboxClasses.TupleLength())-1;
    HTuple step_val64 = 1;
    for (hv_IndexBbox=0; hv_IndexBbox.Continue(end_val64, step_val64); hv_IndexBbox += step_val64)
    {
      SelectObj(ho_BboxRectangle, &ho_RectangleSelected, hv_IndexBbox+1);
      hv_ClassID = hv_ClassIDs.TupleFind(HTuple(hv_BboxClasses[hv_IndexBbox]));
      (*hv_BboxClassIndices)[hv_IndexBbox] = hv_ClassID;
      GetLineWidth(hv_WindowHandle, &hv_LineWidth);
      if (HDevWindowStack::IsOpen())
        SetLineWidth(HDevWindowStack::GetActive(),(hv_LineWidth+2).TupleInt());
      if (HDevWindowStack::IsOpen())
        SetColor(HDevWindowStack::GetActive(),"black");
      if (HDevWindowStack::IsOpen())
        DispObj(ho_RectangleSelected, HDevWindowStack::GetActive());
      if (0 != (HTuple(int(hv_InstanceType==HTuple("rectangle2"))).TupleAnd(hv_ShowDirection)))
      {
        SelectObj(ho_OrientationArrows, &ho_ArrowSelected, hv_IndexBbox+1);
        if (HDevWindowStack::IsOpen())
          DispObj(ho_ArrowSelected, HDevWindowStack::GetActive());
      }
      if (HDevWindowStack::IsOpen())
        SetLineWidth(HDevWindowStack::GetActive(),hv_LineWidth.TupleInt());
      if (HDevWindowStack::IsOpen())
        SetColor(HDevWindowStack::GetActive(),HTuple(hv_Colors[hv_ClassID]));
      if (HDevWindowStack::IsOpen())
        DispObj(ho_RectangleSelected, HDevWindowStack::GetActive());
      if (0 != (HTuple(int(hv_InstanceType==HTuple("rectangle2"))).TupleAnd(hv_ShowDirection)))
      {
        if (HDevWindowStack::IsOpen())
          DispObj(ho_ArrowSelected, HDevWindowStack::GetActive());
      }
    }
    }
    //
    //Draw text of bounding boxes.
    if (0 != hv_ShowLabels)
    {
      //For better visibility the text is displayed after all bounding boxes are drawn.
      //Get text and text size for correct positioning of result class IDs.
      hv_Text = hv_BboxClasses+hv_TextConf;
      //Select text color.
      if (0 != (int(hv_TextColor==HTuple(""))))
      {
        hv_TextColorClasses = HTuple(hv_Colors[(*hv_BboxClassIndices)]);
      }
      else
      {
        TupleGenConst((*hv_BboxClassIndices).TupleLength(), hv_TextColor, &hv_TextColorClasses);
      }
      //Select correct position of the text.
      hv_LabelRow = hv_LabelRowTop;
      if (0 != (int(hv_TextPositionRow==HTuple("bottom"))))
      {
        hv_LabelRow = hv_LabelRowBottom;
      }
      //Display text.
      if (HDevWindowStack::IsOpen())
        DispText(HDevWindowStack::GetActive(),hv_Text, "image", hv_LabelRow, hv_LabelCol, 
            hv_TextColorClasses, ((HTuple("box_color").Append("shadow")).Append("border_radius")), 
            hv_BoxLabelColor.TupleConcat((HTuple("false").Append(0))));
    }
    //
    if (HDevWindowStack::IsOpen())
      SetContourStyle(HDevWindowStack::GetActive(),hv_ContourStyle);
    SetLineStyle(hv_WindowHandle, hv_Style);
  }
  else
  {
    //Do nothing if no results are present.
    (*hv_BboxClassIndices) = HTuple();
  }
  //
  return;
}

// Chapter: Graphics / Output
// Short Description: Display the ground truth/result segmentation as regions. 
void dev_display_segmentation_regions (HObject ho_SegmentationImage, HTuple hv_ClassIDs, 
    HTuple hv_ColorsSegmentation, HTuple hv_ExcludeClassIDs, HTuple *hv_ImageClassIDs)
{

  // Local iconic variables
  HObject  ho_Regions, ho_SelectedRegion;

  // Local control variables
  HTuple  hv_IncludedClassIDs, hv_Area, hv_Index;
  HTuple  hv_ClassID, hv_IndexColor;

  //
  //This procedure displays the ground truth/result segmentation
  //given in SegmentationImage as regions. The ClassIDs are necessary to
  //display ground truth/result segmentations from the same class
  //always with the same color. It is possible to exclude certain ClassIDs
  //from being displayed. The displayed classes are returned in ImageClassIDs.
  //
  //
  //Remove excluded class IDs from the list.
  hv_IncludedClassIDs = hv_ClassIDs.TupleDifference(hv_ExcludeClassIDs);
  //
  //Get a region for each class ID.
  Threshold(ho_SegmentationImage, &ho_Regions, hv_IncludedClassIDs, hv_IncludedClassIDs);
  //
  //Get classes with non-empty regions.
  RegionFeatures(ho_Regions, "area", &hv_Area);
  if (0 != (int((hv_Area.TupleLength())!=(hv_IncludedClassIDs.TupleLength()))))
  {
    throw HException("No equal number of class IDs and segmentation regions.");
  }
  TupleSelectMask(hv_IncludedClassIDs, hv_Area.TupleGreaterElem(0), &(*hv_ImageClassIDs));
  //
  //Display all non-empty class regions in distinct colors.
  {
  HTuple end_val22 = (hv_IncludedClassIDs.TupleLength())-1;
  HTuple step_val22 = 1;
  for (hv_Index=0; hv_Index.Continue(end_val22, step_val22); hv_Index += step_val22)
  {
    if (0 != (int(HTuple(hv_Area[hv_Index])>0)))
    {
      //Use class ID to determine region color.
      hv_ClassID = HTuple(hv_IncludedClassIDs[hv_Index]);
      hv_IndexColor = hv_ClassIDs.TupleFindFirst(hv_ClassID);
      if (HDevWindowStack::IsOpen())
        SetColor(HDevWindowStack::GetActive(),HTuple(hv_ColorsSegmentation[hv_IndexColor]));
      //Display the segmentation region.
      SelectObj(ho_Regions, &ho_SelectedRegion, hv_Index+1);
      if (HDevWindowStack::IsOpen())
        DispObj(ho_SelectedRegion, HDevWindowStack::GetActive());
    }
  }
  }
  return;
}

// Chapter: Deep Learning / Model
// Short Description: Display a legend according to the generic parameters. 
void dev_display_tiled_legend (HTuple hv_WindowImages, HTuple hv_GenParam)
{

  // Local iconic variables

  // Local control variables
  HTuple  hv_DisplayLegend, hv_Exception, hv_LegendText;
  HTuple  hv_Flag, hv_PosTexts, hv_Text;

  //
  //This procedure displays a legend of dev_display_dl_data_tiled
  //according to the generic parameters.
  //
  try
  {
    GetDictTuple(hv_GenParam, "display_legend", &hv_DisplayLegend);
    if (0 != (hv_DisplayLegend.TupleNot()))
    {
      return;
    }
  }
  // catch (Exception) 
  catch (HException &HDevExpDefaultException)
  {
    HDevExpDefaultException.ToHTuple(&hv_Exception);
  }
  //
  hv_LegendText = HTuple();
  try
  {
    GetDictTuple(hv_GenParam, "display_input", &hv_Flag);
    if (0 != hv_Flag)
    {
      hv_LegendText = hv_LegendText.TupleConcat("input");
    }
  }
  // catch (Exception) 
  catch (HException &HDevExpDefaultException)
  {
    HDevExpDefaultException.ToHTuple(&hv_Exception);
    hv_LegendText = hv_LegendText.TupleConcat("input");
  }
  try
  {
    GetDictTuple(hv_GenParam, "display_ground_truth", &hv_Flag);
    if (0 != hv_Flag)
    {
      hv_LegendText = hv_LegendText.TupleConcat("ground truth");
    }
  }
  // catch (Exception) 
  catch (HException &HDevExpDefaultException)
  {
    HDevExpDefaultException.ToHTuple(&hv_Exception);
    hv_LegendText = hv_LegendText.TupleConcat("ground truth");
  }
  try
  {
    GetDictTuple(hv_GenParam, "display_result", &hv_Flag);
    if (0 != hv_Flag)
    {
      hv_LegendText = hv_LegendText.TupleConcat("result");
    }
  }
  // catch (Exception) 
  catch (HException &HDevExpDefaultException)
  {
    HDevExpDefaultException.ToHTuple(&hv_Exception);
    hv_LegendText = hv_LegendText.TupleConcat("result");
  }
  if (0 != (int((hv_LegendText.TupleLength())==3)))
  {
    hv_PosTexts.Clear();
    hv_PosTexts[0] = "Top:    ";
    hv_PosTexts[1] = "Center: ";
    hv_PosTexts[2] = "Bottom: ";
  }
  else if (0 != (int((hv_LegendText.TupleLength())==2)))
  {
    hv_PosTexts.Clear();
    hv_PosTexts[0] = "Top:    ";
    hv_PosTexts[1] = "Bottom: ";
  }
  else
  {
    hv_PosTexts = "";
  }
  HDevWindowStack::SetActive(hv_WindowImages);
  hv_Text = hv_PosTexts+hv_LegendText;
  if (HDevWindowStack::IsOpen())
    DispText(HDevWindowStack::GetActive(),hv_Text, "window", "top", "right", "black", 
        HTuple(), HTuple());
  return;
}

// Chapter: Deep Learning / Anomaly Detection
// Short Description: Display information about the training of an anomaly detection model. 
void dev_display_train_info_anomaly_detection (HTuple hv_TrainParam, HTuple *hv_WindowHandleInfo)
{

  // Local iconic variables

  // Local control variables
  HTuple  hv_DisplayParam, hv_DisplayEnabled, hv_WindowWidth;
  HTuple  hv_WindowHeight, hv_WindowBGColor, hv_TrainParamAnomaly;
  HTuple  hv_DomainRatio, hv_ErrorThreshold, hv_RegularizationNoise;
  HTuple  hv_MaxNumEpochs, hv_TrainInformationLeft, hv_TrainInformationRight;

  //
  //This procedure displays information about the training parameters of an anomaly detection model.
  //
  //Initialize display data.
  GetDictTuple(hv_TrainParam, "display_param", &hv_DisplayParam);
  GetDictTuple(hv_DisplayParam, "enabled", &hv_DisplayEnabled);
  //
  if (0 != (hv_DisplayEnabled.TupleNot()))
  {
    return;
  }
  //
  hv_WindowWidth = 500;
  hv_WindowHeight = 200;
  hv_WindowBGColor = "light gray";
  //
  //Open and setup text window.
  SetWindowAttr("background_color",hv_WindowBGColor);
  OpenWindow(0,0,hv_WindowWidth,hv_WindowHeight,0,"visible","",&(*hv_WindowHandleInfo));
  HDevWindowStack::Push((*hv_WindowHandleInfo));
  set_display_font((*hv_WindowHandleInfo), 16, "mono", "true", "false");
  HDevWindowStack::SetActive((*hv_WindowHandleInfo));
  //
  //Display information.
  GetDictTuple(hv_TrainParam, "anomaly_param", &hv_TrainParamAnomaly);
  GetDictTuple(hv_TrainParamAnomaly, "domain_ratio", &hv_DomainRatio);
  GetDictTuple(hv_TrainParamAnomaly, "error_threshold", &hv_ErrorThreshold);
  GetDictTuple(hv_TrainParamAnomaly, "regularization_noise", &hv_RegularizationNoise);
  GetDictTuple(hv_TrainParamAnomaly, "max_num_epochs", &hv_MaxNumEpochs);
  hv_TrainInformationLeft.Clear();
  hv_TrainInformationLeft[0] = "Training anomaly detection model.";
  hv_TrainInformationLeft[1] = "";
  hv_TrainInformationLeft[2] = "Max. number of epochs:";
  hv_TrainInformationLeft[3] = "Domain ratio:";
  hv_TrainInformationLeft[4] = "Error threshold:";
  hv_TrainInformationLeft[5] = "Regularization noise:";
  hv_TrainInformationLeft[6] = "";
  hv_TrainInformationLeft[7] = "This may take some time...";
  hv_TrainInformationRight.Clear();
  hv_TrainInformationRight[0] = "";
  hv_TrainInformationRight[1] = "";
  hv_TrainInformationRight.Append(hv_MaxNumEpochs);
  hv_TrainInformationRight.Append(hv_DomainRatio.TupleString(".4f"));
  hv_TrainInformationRight.Append(hv_ErrorThreshold.TupleString(".4f"));
  hv_TrainInformationRight.Append(hv_RegularizationNoise.TupleString(".4f"));
  if (HDevWindowStack::IsOpen())
    DispText(HDevWindowStack::GetActive(),hv_TrainInformationLeft, "window", "top", 
        "left", "black", "box", "false");
  if (HDevWindowStack::IsOpen())
    DispText(HDevWindowStack::GetActive(),hv_TrainInformationRight, "window", "top", 
        "right", "black", "box", "false");
  //
  return;
}

// Chapter: Deep Learning / Model
// Short Description: Update the various texts and plots during training. 
void dev_display_update_train_dl_model (HTuple hv_TrainParam, HTuple hv_DisplayData, 
    HTuple hv_TrainInfo, HTuple hv_Epochs, HTuple hv_Loss, HTuple hv_LearningRate, 
    HTuple hv_EvalEpochs, HTuple hv_EvalValues, HTuple hv_EvalValuesTrain)
{

  // Local iconic variables
  HObject  ho_PlotBackground;

  // Local control variables
  HTuple  hv_DisplayEnabled, hv_DisplayParam, hv_WindowText;
  HTuple  hv_WindowHandle, hv_ChangePlotIntervalSeconds, hv_LastChange;
  HTuple  hv_Seconds, hv_PlotEval, hv_PlotFunctionExists;
  HTuple  hv_PlotFunction, hv_TextPlot, hv_PlotTrainEval;
  HTuple  hv_HeadlineText, hv_Indices, hv_Index, hv_PartRow1;
  HTuple  hv_PartColumn1, hv_PartRow2, hv_PartColumn2, hv__;
  HTuple  hv_Width, hv_Height, hv_ClipRegionValue, hv_PlotHeight;
  HTuple  hv_LegendRow, hv_LegendDistanceLeft, hv_LegendDistanceRight;
  HTuple  hv_PlotLearningRateStrategy, hv_RightMargin, hv_ChangeStrategies;
  HTuple  hv_Idx, hv_ChangeStrategy, hv_ChangeStrategyName;
  HTuple  hv_ChangeStrategiesValues, hv_ChangeStrategiesInitial;
  HTuple  hv_StrategyMin, hv_StrategyMax, hv_LogLRMin, hv_LogLRMax;
  HTuple  hv_LRScale, hv_LROffset, hv_LogLROffset, hv_StartYLearningRate;
  HTuple  hv_EndYLearningRate, hv_TicksYLearningRate, hv_LogYLearningRate;
  HTuple  hv_LRColor, hv_LRLineWidth, hv_LRTextLegend, hv_TopMarginPlots;
  HTuple  hv_NumIterationsPerEpoch, hv_Iterations, hv_NumEpochs;
  HTuple  hv_NumIterations, hv_CurrentIteration, hv_XAxisLabel;
  HTuple  hv_ValuesX, hv_TicksX, hv_EvalValuesX, hv_EvalTicksX;
  HTuple  hv_TitleX, hv_EvalIterations, hv_EvalValuesMin;
  HTuple  hv_EvalValuesMax, hv_TicksY, hv_StartY, hv_EndY;
  HTuple  hv_YAxisLabel, hv_BestEvaluationData, hv_BestEvaluationComparisonKeys;
  HTuple  hv_StringExtendsLegendRight, hv_Bullet, hv_Line;
  HTuple  hv_YAxisTitle, hv_Offset, hv_LogMin, hv_LogMax;
  HTuple  hv_Scale, hv_LogOffset, hv_LogY, hv_TextModelParams;
  HTuple  hv_ModelParams, hv_StatusModelParamsLeft, hv_StatusModelParamsRight;
  HTuple  hv_ParName, hv_Tuple, hv_Exception, hv_TupleStr;
  HTuple  hv_StatusEvaluationLeft, hv_StatusEvaluationRight;
  HTuple  hv_BestEvaluationValue, hv_BestEvaluationInfo, hv_BestEvaluationEpoch;
  HTuple  hv_BestTrainEvaluationValue, hv_BestTrainEvaluationInfo;
  HTuple  hv_BestTrainEvaluationEpoch, hv_BestEvaluationComparisonKeysStr;
  HTuple  hv_StatusTrainLeft, hv_StatusTrainRight, hv_EpochReal;
  HTuple  hv_MeanLoss, hv_MeanLossStr, hv_TimeElapsedExists;
  HTuple  hv_StartEpoch, hv_StartTime, hv_SecondsElapsed;
  HTuple  hv_SecondsRemaining, hv_ProgressPercent, hv_ProgressPerSecond;
  HTuple  hv_TimeElapsedString, hv_TimeRemainingString, hv_StatusDeviceLeft;
  HTuple  hv_StatusDeviceRight, hv_DeviceNameExists, hv_DeviceName;
  HTuple  hv_StatusLeft, hv_StatusRight, hv_MaxChars, hv_Str;
  HTuple  hv_IsString, hv_Length, hv_SubStr, hv_Row1, hv_Column1;
  HTuple  hv_Row2, hv_Column2, hv_WindowTextWidth, hv_WindowTextHeight;

  //
  //This procedure updates the various texts and plots.
  //It uses precomputed information (TrainInfo, EvaluationInfos,...).
  //
  GetDictTuple(hv_DisplayData, "enabled", &hv_DisplayEnabled);
  if (0 != (hv_DisplayEnabled.TupleNot()))
  {
    return;
  }
  //
  GetDictTuple(hv_DisplayData, "display_param", &hv_DisplayParam);
  //
  GetDictTuple(hv_DisplayData, "window_text", &hv_WindowText);
  SetWindowParam(hv_WindowText, "flush", "false");
  //Only switch to window if the current window is not the text window (performance).
  if (HDevWindowStack::IsOpen())
    hv_WindowHandle = HDevWindowStack::GetActive();
  if (0 != (int(hv_WindowHandle!=hv_WindowText)))
  {
    HDevWindowStack::SetActive(hv_WindowText);
    hv_WindowHandle = hv_WindowText;
  }
  if (HDevWindowStack::IsOpen())
    ClearWindow(HDevWindowStack::GetActive());
  //
  GetDictTuple(hv_DisplayParam, "change_plot_interval_seconds", &hv_ChangePlotIntervalSeconds);
  GetDictTuple(hv_DisplayData, "last_change_plot", &hv_LastChange);
  CountSeconds(&hv_Seconds);
  GetDictTuple(hv_DisplayData, "plot_eval", &hv_PlotEval);
  if (0 != (int((hv_Seconds-hv_LastChange)>=hv_ChangePlotIntervalSeconds)))
  {
    hv_PlotEval = HTuple(hv_PlotEval.TupleNot()).TupleAnd(int((hv_EvalEpochs.TupleLength())>=2));
    SetDictTuple(hv_DisplayData, "plot_eval", hv_PlotEval);
    SetDictTuple(hv_DisplayData, "last_change_plot", hv_Seconds);
  }
  //This procedure can also be called after a training.
  //In such a case the parameter plot_function can be set to determine,
  //which plot shall be displayed.
  GetDictParam(hv_DisplayData, "key_exists", "plot_function", &hv_PlotFunctionExists);
  if (0 != hv_PlotFunctionExists)
  {
    GetDictTuple(hv_DisplayData, "plot_function", &hv_PlotFunction);
    hv_PlotEval = hv_PlotEval.TupleOr(int(hv_PlotFunction==HTuple("evaluation")));
  }
  if (0 != hv_PlotEval)
  {
    hv_TextPlot = "2/2";
  }
  else
  {
    hv_TextPlot = "1/2";
    hv_PlotTrainEval = 0;
  }
  hv_HeadlineText = ("Showing plot "+hv_TextPlot)+":";
  //
  //Shall the training evaluation be plotted?
  hv_PlotTrainEval = 0;
  //
  //In case there are missing evaluation values (-1),
  //we just reuse the previous values.
  if (0 != (hv_EvalValuesTrain.TupleLength()))
  {
    hv_PlotTrainEval = int((hv_EvalValuesTrain.TupleMax())!=-1);
    if (0 != hv_PlotTrainEval)
    {
      TupleFind(hv_EvalValuesTrain, -1, &hv_Indices);
      if (0 != (HTuple(int(hv_Indices!=-1)).TupleAnd(int(hv_Indices!=HTuple()))))
      {
        {
        HTuple end_val56 = (hv_Indices.TupleLength())-1;
        HTuple step_val56 = 1;
        for (hv_Index=0; hv_Index.Continue(end_val56, step_val56); hv_Index += step_val56)
        {
          if (0 != (int(HTuple(hv_Indices[hv_Index])==0)))
          {
            hv_EvalValuesTrain[0] = 0.0;
          }
          else
          {
            hv_EvalValuesTrain[HTuple(hv_Indices[hv_Index])] = HTuple(hv_EvalValuesTrain[HTuple(hv_Indices[hv_Index])-1]);
          }
        }
        }
      }
    }
  }
  //
  GetPart(hv_WindowText, &hv_PartRow1, &hv_PartColumn1, &hv_PartRow2, &hv_PartColumn2);
  GetWindowExtents(hv_WindowHandle, &hv__, &hv__, &hv_Width, &hv_Height);
  //Generate a background rectangle for the plot.
  //For a correct visualization of the rectangle, the region
  //may not be cut off.
  GetSystem("clip_region", &hv_ClipRegionValue);
  SetSystem("clip_region", "false");
  GenRectangle1(&ho_PlotBackground, 470, 9, hv_PartRow2-6, hv_PartColumn2-10);
  SetSystem("clip_region", hv_ClipRegionValue);
  //
  hv_PlotHeight = (((hv_PartRow2-30)-480)*hv_Height)/((hv_PartRow2-hv_PartRow1)+1);
  hv_LegendRow = 479;
  hv_LegendDistanceLeft = 65;
  hv_LegendDistanceRight = 85;
  //
  //Get change strategy parameters and check if a learning rate strategy exits
  hv_PlotLearningRateStrategy = 0;
  hv_RightMargin = 35;
  GetDictTuple(hv_TrainParam, "change_strategies", &hv_ChangeStrategies);
  if (0 != (int((hv_ChangeStrategies.TupleLength())>0)))
  {
    {
    HTuple end_val87 = (hv_ChangeStrategies.TupleLength())-1;
    HTuple step_val87 = 1;
    for (hv_Idx=0; hv_Idx.Continue(end_val87, step_val87); hv_Idx += step_val87)
    {
      hv_ChangeStrategy = HTuple(hv_ChangeStrategies[hv_Idx]);
      GetDictTuple(hv_ChangeStrategy, "model_param", &hv_ChangeStrategyName);
      if (0 != (int(hv_ChangeStrategyName==HTuple("learning_rate"))))
      {
        hv_PlotLearningRateStrategy = 1;
        GetDictTuple(hv_ChangeStrategy, "values", &hv_ChangeStrategiesValues);
        GetDictTuple(hv_ChangeStrategy, "initial_value", &hv_ChangeStrategiesInitial);
        //
        //Plot parameters for the learning rate
        hv_StrategyMin = hv_LearningRate.TupleMin();
        hv_StrategyMax = hv_LearningRate.TupleMax();
        hv_LogLRMin = (HTuple(1e-8).TupleMax2(hv_StrategyMin)).TupleLog10();
        hv_LogLRMax = (HTuple(1e-8).TupleMax2(hv_StrategyMax)).TupleLog10();
        //
        if (0 != (int(((hv_LogLRMax-hv_LogLRMin).TupleFabs())<1e-4)))
        {
          hv_LogLRMin = hv_LogLRMin-5e-5;
          hv_LogLRMax += 5e-5;
        }
        //
        hv_LRScale = (hv_LogLRMax-hv_LogLRMin)/hv_PlotHeight;
        hv_LROffset = 15;
        hv_LogLROffset = hv_LROffset*hv_LRScale;
        //
        hv_StartYLearningRate = HTuple(10).TuplePow(hv_LogLRMin-(1.0*hv_LogLROffset));
        hv_EndYLearningRate = HTuple(10).TuplePow(hv_LogLRMax+(1.0*hv_LogLROffset));
        hv_TicksYLearningRate = (hv_PlotHeight/5)*hv_LRScale;
        //
        hv_LogYLearningRate = "true";
        hv_RightMargin = 75;
      }
    }
    }
  }
  hv_LRColor = "#1332ffdd";
  hv_LRLineWidth = 2;
  hv_LRTextLegend = "'learning_rate'";
  //Space for legends
  hv_TopMarginPlots = 480+20;
  //
  //Determine current number of iterations.
  GetDictTuple(hv_TrainInfo, "num_iterations_per_epoch", &hv_NumIterationsPerEpoch);
  hv_Iterations = ((hv_Epochs*hv_NumIterationsPerEpoch).TupleCeil()).TupleInt();
  GetDictTuple(hv_TrainInfo, "num_epochs", &hv_NumEpochs);
  hv_NumIterations = ((hv_NumEpochs*hv_NumIterationsPerEpoch).TupleCeil()).TupleInt();
  hv_CurrentIteration = ((const HTuple&)hv_Iterations)[(hv_Iterations.TupleLength())-1];
  //
  //Determine x-axis values.
  GetDictTuple(hv_DisplayParam, "x_axis_label", &hv_XAxisLabel);
  if (0 != (int(hv_XAxisLabel==HTuple("epochs"))))
  {
    hv_ValuesX = hv_Epochs;
    hv_TicksX = HTuple(0.1).TupleMax2(((hv_Epochs.TupleMax())-(hv_Epochs.TupleMin()))*0.15);
    if (0 != (int(hv_EvalEpochs!=HTuple())))
    {
      hv_EvalValuesX = hv_EvalEpochs;
      hv_EvalTicksX = HTuple(0.1).TupleMax2(((hv_EvalEpochs.TupleMax())-(hv_EvalEpochs.TupleMin()))*0.15);
    }
    hv_TitleX = "Epochs";
  }
  else if (0 != (int(hv_XAxisLabel==HTuple("iterations"))))
  {
    hv_ValuesX = hv_Iterations;
    hv_TicksX = (HTuple(2).TupleMax2(((hv_Iterations.TupleMax())-(hv_Iterations.TupleMin()))*0.15)).TupleInt();
    if (0 != (int(hv_EvalEpochs!=HTuple())))
    {
      hv_EvalIterations = (hv_EvalEpochs*hv_NumIterationsPerEpoch).TupleCeil();
      hv_EvalValuesX = hv_EvalIterations;
      hv_EvalTicksX = (HTuple(2).TupleMax2(((hv_EvalIterations.TupleMax())-(hv_EvalIterations.TupleMin()))*0.15)).TupleInt();
    }
    hv_TitleX = "Iterations";
  }
  //
  //Determine y-axis values and plot the function as well as its texts.
  if (0 != hv_PlotEval)
  {
    if (HDevWindowStack::IsOpen())
      SetColor(HDevWindowStack::GetActive(),"white");
    if (HDevWindowStack::IsOpen())
      SetDraw(HDevWindowStack::GetActive(),"fill");
    if (HDevWindowStack::IsOpen())
      DispObj(ho_PlotBackground, HDevWindowStack::GetActive());
    hv_EvalValuesMin = (hv_EvalValues.TupleConcat(HTuple(0.0).TupleMax2(hv_EvalValuesTrain))).TupleMin();
    hv_EvalValuesMax = (hv_EvalValues.TupleConcat(hv_EvalValuesTrain)).TupleMax();
    if (0 != (int(((hv_EvalValuesMax-hv_EvalValuesMin).TupleAbs())<1e-3)))
    {
      hv_EvalValuesMin = hv_EvalValuesMin-5e-4;
      hv_EvalValuesMax += 5e-4;
    }
    hv_TicksY = (hv_EvalValuesMax-hv_EvalValuesMin)*0.1;
    hv_StartY = hv_EvalValuesMin-((hv_EvalValuesMax-hv_EvalValuesMin)*0.1);
    hv_EndY = hv_EvalValuesMax+((hv_EvalValuesMax-hv_EvalValuesMin)*0.1);
    //
    hv_YAxisLabel = "Evaluation value";
    GetDictTuple(hv_TrainInfo, "best_evaluation", &hv_BestEvaluationData);
    if (0 != (int((hv_BestEvaluationData.TupleLength())>0)))
    {
      GetDictTuple(hv_BestEvaluationData, "comparison_keys", &hv_BestEvaluationComparisonKeys);
      hv_YAxisLabel = "Evaluation value";
      if (0 != (int((hv_BestEvaluationComparisonKeys.TupleLength())>1)))
      {
        pretty_print_tuple(hv_BestEvaluationComparisonKeys, &hv_YAxisLabel);
        hv_YAxisLabel = ("mean("+hv_YAxisLabel)+")";
      }
      else
      {
        hv_YAxisLabel = hv_BestEvaluationComparisonKeys;
      }
    }
    //Use a smaller, non-bold font for the plot.
    set_display_font(hv_WindowText, 12, "mono", "false", "false");
    //Plot learning rate if the corresponding strategy exists.
    if (0 != hv_PlotLearningRateStrategy)
    {
      //
      //Display current values in appropriate colors.
      GetStringExtents(hv_WindowText, hv_LRTextLegend, &hv__, &hv__, &hv_StringExtendsLegendRight, 
          &hv__);
      if (HDevWindowStack::IsOpen())
        DispText(HDevWindowStack::GetActive(),hv_LRTextLegend, "image", hv_LegendRow, 
            (hv_Width-hv_StringExtendsLegendRight)-hv_LegendDistanceRight, hv_LRColor, 
            "box", "false");
      plot_tuple_no_window_handling(hv_WindowText, hv_ValuesX, hv_LearningRate, "", 
          "", hv_LRColor, ((((((((((((HTuple("log_y").Append("axes_color")).Append("start_y")).Append("end_y")).Append("ticks_y")).Append("margin_top")).Append("margin_bottom")).Append("margin_left")).Append("margin_right")).Append("line_width")).Append("axes_color")).Append("axis_location_y")).Append("format_y")), 
          ((((((((hv_LogYLearningRate.TupleConcat("black")).TupleConcat(hv_StartYLearningRate)).TupleConcat(hv_EndYLearningRate)).TupleConcat(hv_TicksYLearningRate)).TupleConcat(hv_TopMarginPlots)).TupleConcat((HTuple(30).Append(65)))).TupleConcat(hv_RightMargin)).TupleConcat(hv_LRLineWidth)).TupleConcat(((HTuple("#898b8f").Append("right")).Append(".1e"))));
    }
    //Plot validation evaluation values.
    plot_tuple_no_window_handling(hv_WindowText, hv_EvalValuesX, hv_EvalValues, hv_TitleX, 
        "", "#36a2eb", ((((((((((HTuple("axes_color").Append("ticks_x")).Append("ticks_y")).Append("start_y")).Append("end_y")).Append("margin_top")).Append("margin_bottom")).Append("margin_left")).Append("margin_right")).Append("line_width")).Append("axes_color")), 
        (((((((HTuple("black").TupleConcat(hv_EvalTicksX)).TupleConcat(hv_TicksY)).TupleConcat(hv_StartY)).TupleConcat(hv_EndY)).TupleConcat(hv_TopMarginPlots)).TupleConcat((HTuple(30).Append(65)))).TupleConcat(hv_RightMargin)).TupleConcat((HTuple(3).Append("#898b8f"))));
    //Plot train evaluation values.
    if (0 != hv_PlotTrainEval)
    {
      hv_Bullet = //'бё'
      "\241\361";
      hv_Line = //'иD'
      "\250D";
      hv_YAxisTitle = ((((("  '"+hv_YAxisLabel)+"' (")+hv_Line)+HTuple(" validation, "))+hv_Bullet)+"-- training)";
      plot_tuple_no_window_handling(hv_WindowText, hv_EvalValuesX, hv_EvalValuesTrain, 
          hv_TitleX, "", "#36a2eb", (((((((((((HTuple("style").Append("axes_color")).Append("ticks_x")).Append("ticks_y")).Append("start_y")).Append("end_y")).Append("margin_top")).Append("margin_bottom")).Append("margin_left")).Append("margin_right")).Append("line_width")).Append("axes_color")), 
          ((((((((HTuple(20).Append("black")).TupleConcat(hv_EvalTicksX)).TupleConcat(hv_TicksY)).TupleConcat(hv_StartY)).TupleConcat(hv_EndY)).TupleConcat(hv_TopMarginPlots)).TupleConcat((HTuple(30).Append(65)))).TupleConcat(hv_RightMargin)).TupleConcat((HTuple(3).Append("#898b8f"))));
      plot_tuple_no_window_handling(hv_WindowText, hv_EvalValuesX, hv_EvalValuesTrain, 
          hv_TitleX, "", "#36a2eb", (((((((((((HTuple("style").Append("axes_color")).Append("ticks_x")).Append("ticks_y")).Append("start_y")).Append("end_y")).Append("margin_top")).Append("margin_bottom")).Append("margin_left")).Append("margin_right")).Append("line_width")).Append("axes_color")), 
          ((((((((HTuple("circle").Append("black")).TupleConcat(hv_EvalTicksX)).TupleConcat(hv_TicksY)).TupleConcat(hv_StartY)).TupleConcat(hv_EndY)).TupleConcat(hv_TopMarginPlots)).TupleConcat((HTuple(30).Append(65)))).TupleConcat(hv_RightMargin)).TupleConcat((HTuple(3).Append("#898b8f"))));
    }
    else
    {
      hv_YAxisTitle = ("  '"+hv_YAxisLabel)+"'";
    }
    //Display title of y-axis.
    if (HDevWindowStack::IsOpen())
      DispText(HDevWindowStack::GetActive(),hv_YAxisTitle, "image", hv_LegendRow, 
          hv_LegendDistanceLeft, "#36a2eb", "box", "false");
    //Reset font.
    set_display_font(hv_WindowText, 16, "mono", "true", "false");
  }
  else
  {
    if (0 != (int((hv_Epochs.TupleLength())>3)))
    {
      if (HDevWindowStack::IsOpen())
        SetColor(HDevWindowStack::GetActive(),"white");
      if (HDevWindowStack::IsOpen())
        SetDraw(HDevWindowStack::GetActive(),"fill");
      if (HDevWindowStack::IsOpen())
        DispObj(ho_PlotBackground, HDevWindowStack::GetActive());
      //
      //Set StartY and EndY, such that there is a margin on top and bottom to
      //avoid that the plot overlaps with the axis captions. (With respect to
      //the logarithmic plotting of the Loss function)
      //Set this offset in window coordinates:
      hv_Offset = 15;
      //Calculate min max values to determine the correct offset in log
      //coordinates.
      hv_LogMin = (HTuple(0.00001).TupleMax2(hv_Loss.TupleMin())).TupleLog10();
      hv_LogMax = (HTuple(0.00001).TupleMax2(hv_Loss.TupleMax())).TupleLog10();
      //
      if (0 != (int(((hv_LogMax-hv_LogMin).TupleFabs())<0.0001)))
      {
        hv_LogMin = hv_LogMin-0.00005;
        hv_LogMax += 0.00005;
      }
      //
      hv_Scale = (hv_LogMax-hv_LogMin)/hv_PlotHeight;
      hv_LogOffset = hv_Offset*hv_Scale;
      hv_StartY = HTuple(10).TuplePow(hv_LogMin-hv_LogOffset);
      hv_EndY = HTuple(10).TuplePow(hv_LogMax+hv_LogOffset);
      hv_TicksY = (hv_PlotHeight/10)*hv_Scale;
      //
      //Use a smaller, non-bold font for the plot.
      set_display_font(hv_WindowText, 12, "mono", "false", "false");
      hv_LogY = "true";
      //Plot learning rate if the corresponding strategy exists.
      if (0 != hv_PlotLearningRateStrategy)
      {
        //
        //Display current values in appropriate colors.
        GetStringExtents(hv_WindowText, hv_LRTextLegend, &hv__, &hv__, &hv_StringExtendsLegendRight, 
            &hv__);
        if (HDevWindowStack::IsOpen())
          DispText(HDevWindowStack::GetActive(),hv_LRTextLegend, "image", hv_LegendRow, 
              (hv_Width-hv_StringExtendsLegendRight)-hv_LegendDistanceRight, hv_LRColor, 
              "box", "false");
        plot_tuple_no_window_handling(hv_WindowText, hv_ValuesX, hv_LearningRate, 
            "", "", hv_LRColor, ((((((((((((HTuple("log_y").Append("axes_color")).Append("start_y")).Append("end_y")).Append("ticks_y")).Append("margin_top")).Append("margin_bottom")).Append("margin_left")).Append("margin_right")).Append("line_width")).Append("axes_color")).Append("axis_location_y")).Append("format_y")), 
            ((((((((hv_LogY.TupleConcat("black")).TupleConcat(hv_StartYLearningRate)).TupleConcat(hv_EndYLearningRate)).TupleConcat(hv_TicksYLearningRate)).TupleConcat(hv_TopMarginPlots)).TupleConcat((HTuple(30).Append(65)))).TupleConcat(hv_RightMargin)).TupleConcat(hv_LRLineWidth)).TupleConcat(((HTuple("#898b8f").Append("right")).Append(".1e"))));
      }
      hv_YAxisLabel = "Loss";
      if (HDevWindowStack::IsOpen())
        DispText(HDevWindowStack::GetActive(),"  "+hv_YAxisLabel, "image", hv_LegendRow, 
            hv_LegendDistanceLeft, "#ff6384", "box", "false");
      plot_tuple_no_window_handling(hv_WindowText, hv_ValuesX, hv_Loss, hv_TitleX, 
          "", "#ff6384", (((((((((((HTuple("log_y").Append("axes_color")).Append("ticks_x")).Append("ticks_y")).Append("start_y")).Append("end_y")).Append("margin_top")).Append("margin_bottom")).Append("margin_left")).Append("margin_right")).Append("line_width")).Append("axes_color")), 
          ((((((((hv_LogY.TupleConcat("black")).TupleConcat(hv_TicksX)).TupleConcat(hv_TicksY)).TupleConcat(hv_StartY)).TupleConcat(hv_EndY)).TupleConcat(hv_TopMarginPlots)).TupleConcat((HTuple(30).Append(65)))).TupleConcat(hv_RightMargin)).TupleConcat((HTuple(3).Append("#898b8f"))));
      set_display_font(hv_WindowText, 16, "mono", "true", "false");
    }
    else
    {
      hv_HeadlineText = "Waiting for data to initialize the plot...";
    }
  }

  if (HDevWindowStack::IsOpen())
    DispText(HDevWindowStack::GetActive(),hv_HeadlineText, "image", 445, 9, "black", 
        "box", "false");
  //
  //Model parameter status.
  GetDictTuple(hv_DisplayParam, "status_model_params", &hv_TextModelParams);
  GetDictTuple(hv_TrainInfo, "model_params", &hv_ModelParams);
  hv_StatusModelParamsLeft = HTuple();
  hv_StatusModelParamsRight = HTuple();
  {
  HTuple end_val260 = (hv_TextModelParams.TupleLength())-1;
  HTuple step_val260 = 1;
  for (hv_Index=0; hv_Index.Continue(end_val260, step_val260); hv_Index += step_val260)
  {
    hv_ParName = HTuple(hv_TextModelParams[hv_Index]);
    try
    {
      GetDictTuple(hv_ModelParams, hv_ParName, &hv_Tuple);
    }
    // catch (Exception) 
    catch (HException &HDevExpDefaultException)
    {
      HDevExpDefaultException.ToHTuple(&hv_Exception);
      continue;
    }
    if (0 != (HTuple(int(hv_ParName==HTuple("batch_size_multiplier"))).TupleAnd(int(hv_Tuple==1))))
    {
      continue;
    }
    hv_StatusModelParamsLeft = hv_StatusModelParamsLeft.TupleConcat(("'"+hv_ParName)+"'");

    pretty_print_tuple(hv_Tuple, &hv_TupleStr);
    hv_StatusModelParamsRight = hv_StatusModelParamsRight.TupleConcat(hv_TupleStr);
  }
  }
  if (0 != (int((hv_StatusModelParamsLeft.TupleLength())>0)))
  {
    hv_StatusModelParamsLeft = HTuple("Model parameters:").TupleConcat("  "+hv_StatusModelParamsLeft);
    hv_StatusModelParamsRight = HTuple(" ").TupleConcat(hv_StatusModelParamsRight);
  }
  //
  //Evaluation status.
  hv_StatusEvaluationLeft = HTuple();
  hv_StatusEvaluationRight = HTuple();
  GetDictTuple(hv_TrainInfo, "best_evaluation", &hv_BestEvaluationData);
  if (0 != (int((hv_BestEvaluationData.TupleLength())>0)))
  {
    GetDictTuple(hv_BestEvaluationData, "comparison_keys", &hv_BestEvaluationComparisonKeys);
    GetDictTuple(hv_BestEvaluationData, "best_value", &hv_BestEvaluationValue);
    GetDictTuple(hv_BestEvaluationData, "best_info", &hv_BestEvaluationInfo);
    GetDictTuple(hv_BestEvaluationInfo, "epoch", &hv_BestEvaluationEpoch);
    GetDictTuple(hv_BestEvaluationData, "best_value_train", &hv_BestTrainEvaluationValue);
    GetDictTuple(hv_BestEvaluationData, "best_info_train", &hv_BestTrainEvaluationInfo);
    GetDictTuple(hv_BestTrainEvaluationInfo, "epoch", &hv_BestTrainEvaluationEpoch);
    if (0 != (int((hv_BestEvaluationComparisonKeys.TupleLength())>1)))
    {
      hv_StatusEvaluationLeft = hv_StatusEvaluationLeft.TupleConcat("Measures");
      hv_BestEvaluationComparisonKeysStr = HTuple(HTuple("multiple (")+(hv_BestEvaluationComparisonKeys.TupleLength()))+")";
    }
    else
    {
      hv_StatusEvaluationLeft = hv_StatusEvaluationLeft.TupleConcat("Measure");
      hv_BestEvaluationComparisonKeysStr = ("'"+hv_BestEvaluationComparisonKeys)+"'";
    }
    //
    hv_StatusEvaluationRight = hv_StatusEvaluationRight.TupleConcat(hv_BestEvaluationComparisonKeysStr);
    //Validation value.
    hv_StatusEvaluationLeft = hv_StatusEvaluationLeft.TupleConcat("Best value validation (value / epoch)");
    hv_StatusEvaluationRight = hv_StatusEvaluationRight.TupleConcat(((hv_BestEvaluationValue.TupleString("0.3f"))+" / ")+(hv_BestEvaluationEpoch.TupleString("0.1f")));
    //Training value.
    if (0 != hv_PlotTrainEval)
    {
      hv_StatusEvaluationLeft = hv_StatusEvaluationLeft.TupleConcat("Best value training (value / epoch)");
      hv_StatusEvaluationRight = hv_StatusEvaluationRight.TupleConcat(((hv_BestTrainEvaluationValue.TupleString("0.3f"))+" / ")+(hv_BestTrainEvaluationEpoch.TupleString("0.1f")));
    }
    //
    hv_StatusEvaluationLeft = HTuple("Evaluation:").TupleConcat("  "+hv_StatusEvaluationLeft);
    hv_StatusEvaluationRight = HTuple(" ").TupleConcat(hv_StatusEvaluationRight);
  }
  //
  //Train status.
  hv_StatusTrainLeft = HTuple();
  hv_StatusTrainRight = HTuple();
  GetDictTuple(hv_TrainInfo, "epoch", &hv_EpochReal);
  GetDictTuple(hv_TrainInfo, "num_epochs", &hv_NumEpochs);
  hv_StatusTrainLeft = hv_StatusTrainLeft.TupleConcat("Epoch");
  hv_StatusTrainRight = hv_StatusTrainRight.TupleConcat(((hv_EpochReal.TupleString(".1f"))+" of ")+(hv_NumEpochs.TupleString(".1f")));
  hv_StatusTrainLeft = hv_StatusTrainLeft.TupleConcat("Iteration");
  hv_StatusTrainRight = hv_StatusTrainRight.TupleConcat((hv_CurrentIteration+" of ")+hv_NumIterations);
  GetDictTuple(hv_TrainInfo, "mean_loss", &hv_MeanLoss);
  if (0 != (int((hv_MeanLoss.TupleLength())==0)))
  {
    hv_MeanLossStr = "";
  }
  else
  {
    hv_MeanLossStr = hv_MeanLoss.TupleString("0.4f");
  }
  hv_StatusTrainLeft = hv_StatusTrainLeft.TupleConcat("Loss");
  hv_StatusTrainRight = hv_StatusTrainRight.TupleConcat(hv_MeanLossStr);
  //
  //Elapsed and remaining time.
  GetDictParam(hv_DisplayData, "key_exists", "time_elapsed", &hv_TimeElapsedExists);
  if (0 != (hv_TimeElapsedExists.TupleNot()))
  {
    //During training the key is not set and the time has to be determined.
    GetDictTuple(hv_TrainInfo, "start_epoch", &hv_StartEpoch);
    GetDictTuple(hv_TrainInfo, "start_time", &hv_StartTime);
    estimate_progress(hv_StartTime, hv_StartEpoch, hv_EpochReal, hv_NumEpochs, &hv_SecondsElapsed, 
        &hv_SecondsRemaining, &hv_ProgressPercent, &hv_ProgressPerSecond);
    timespan_string(hv_SecondsElapsed, "auto", &hv_TimeElapsedString);
    timespan_string(hv_SecondsRemaining, "top2", &hv_TimeRemainingString);
    hv_StatusTrainLeft = hv_StatusTrainLeft.TupleConcat((HTuple("Time elapsed").Append("Time left")));
    hv_StatusTrainRight = (hv_StatusTrainRight.TupleConcat(hv_TimeElapsedString)).TupleConcat(hv_TimeRemainingString);
  }
  else
  {
    //For display after the finished training the key may be set.
    //In case of a given value, display it.
    GetDictTuple(hv_DisplayData, "time_elapsed", &hv_TimeElapsedString);
    if (0 != (int(hv_TimeElapsedString!=HTuple(""))))
    {
      hv_StatusTrainLeft = hv_StatusTrainLeft.TupleConcat("Time elapsed");
      hv_StatusTrainRight = hv_StatusTrainRight.TupleConcat(hv_TimeElapsedString);
    }
  }
  //
  //Device Status.
  hv_StatusDeviceLeft = HTuple();
  hv_StatusDeviceRight = HTuple();
  GetDictParam(hv_ModelParams, "key_exists", "device_name", &hv_DeviceNameExists);
  if (0 != hv_DeviceNameExists)
  {
    GetDictTuple(hv_ModelParams, "device_name", &hv_DeviceName);
    hv_StatusDeviceLeft = "Used Device: "+hv_DeviceName;
    hv_StatusDeviceRight = hv_StatusDeviceRight.TupleConcat(" ");
  }
  //
  //Combine all.
  hv_StatusLeft.Clear();
  hv_StatusLeft[0] = "train_dl_model";
  hv_StatusLeft[1] = " ";
  hv_StatusLeft[2] = " ";
  hv_StatusLeft.Append(hv_StatusTrainLeft);
  hv_StatusLeft.Append(" ");
  hv_StatusLeft.Append(hv_StatusEvaluationLeft);
  hv_StatusLeft.Append(" ");
  hv_StatusLeft.Append(hv_StatusModelParamsLeft);
  hv_StatusLeft.Append(" ");
  hv_StatusLeft.Append(hv_StatusDeviceLeft);
  hv_StatusRight.Clear();
  hv_StatusRight[0] = " ";
  hv_StatusRight[1] = " ";
  hv_StatusRight[2] = " ";
  hv_StatusRight.Append(hv_StatusTrainRight);
  hv_StatusRight.Append(" ");
  hv_StatusRight.Append(hv_StatusEvaluationRight);
  hv_StatusRight.Append(" ");
  hv_StatusRight.Append(hv_StatusModelParamsRight);
  hv_StatusRight.Append(" ");
  hv_StatusRight.Append(hv_StatusDeviceRight);
  //
  //Cut strings with too many chars.
  hv_MaxChars = 14;
  {
  HTuple end_val369 = (hv_StatusRight.TupleLength())-1;
  HTuple step_val369 = 1;
  for (hv_Index=0; hv_Index.Continue(end_val369, step_val369); hv_Index += step_val369)
  {
    hv_Str = HTuple(hv_StatusRight[hv_Index]);
    TupleIsString(hv_Str, &hv_IsString);
    if (0 != hv_IsString)
    {
      TupleStrlen(hv_Str, &hv_Length);
      if (0 != (int(hv_Length>hv_MaxChars)))
      {
        hv_SubStr = (hv_Str.TupleSubstr(0,hv_MaxChars-3))+"...";
        hv_StatusRight[hv_Index] = hv_SubStr;
      }
    }
  }
  }
  //
  //Display the text.
  GetDictTuple(hv_DisplayData, "window_text", &hv_WindowText);
  //
  if (HDevWindowStack::IsOpen())
    DispText(HDevWindowStack::GetActive(),hv_StatusLeft, "window", "top", "left", 
        "black", "box", "false");
  if (HDevWindowStack::IsOpen())
    DispText(HDevWindowStack::GetActive(),hv_StatusRight, "window", "top", "right", 
        "black", "box", "false");
  FlushBuffer(hv_WindowText);
  SetWindowParam(hv_WindowText, "flush", "true");
  GetPart(hv_WindowText, &hv_Row1, &hv_Column1, &hv_Row2, &hv_Column2);
  GetDictTuple(hv_DisplayData, "window_text_width", &hv_WindowTextWidth);
  GetDictTuple(hv_DisplayData, "window_text_height", &hv_WindowTextHeight);
  if (0 != (HTuple(int((hv_WindowTextWidth-1)!=(hv_Column2-hv_Column1))).TupleOr(int((hv_WindowTextHeight-1)!=(hv_Row2-hv_Row1)))))
  {
    if (HDevWindowStack::IsOpen())
      SetPart(HDevWindowStack::GetActive(),hv_Row1, hv_Column1, (hv_Row1+hv_WindowTextHeight)-1, 
          (hv_Column1+hv_WindowTextWidth)-1);
  }
  //
  return;
}

// Chapter: Graphics / Output
// Short Description: Display a map of weights. 
void dev_display_weight_regions (HObject ho_ImageWeight, HTuple hv_DrawTransparency, 
    HTuple hv_SegMaxWeight, HTuple *hv_Colors)
{

  // Local iconic variables
  HObject  ho_Domain, ho_WeightsRegion;

  // Local control variables
  HTuple  hv_NumColors, hv_WeightsColorsAlpha, hv_Rows;
  HTuple  hv_Columns, hv_GrayVal, hv_GrayValWeight, hv_ColorIndex;
  HTuple  hv_ClassColor;

  //
  //This procedure displays a map of the weights
  //given in ImageWeight as regions.
  //The transparency can be adjusted.
  //The used colors are returned.
  //
  //Define colors.
  hv_NumColors = 20;
  get_distinct_colors(hv_NumColors, 0, 0, 160, &(*hv_Colors));
  TupleInverse((*hv_Colors), &(*hv_Colors));
  hv_WeightsColorsAlpha = (*hv_Colors)+hv_DrawTransparency;
  //
  //Get gay values of ImageWeight.
  GetDomain(ho_ImageWeight, &ho_Domain);
  GetRegionPoints(ho_Domain, &hv_Rows, &hv_Columns);
  GetGrayval(ho_ImageWeight, hv_Rows, hv_Columns, &hv_GrayVal);
  //
  //Check that the gray values of the image
  //are below the specified maximum.
  if (0 != (int((hv_GrayVal.TupleMax())>hv_SegMaxWeight)))
  {
    throw HException(((("The maximum weight ("+(hv_GrayVal.TupleMax()))+") in the weight image is greater than the given SegMaxWeight (")+hv_SegMaxWeight)+").");
  }
  //
  while (0 != (int(hv_GrayVal!=HTuple())))
  {
    //Go through all gray value 'groups',
    //starting from the maximum.
    hv_GrayValWeight = hv_GrayVal.TupleMax();
    hv_GrayVal = hv_GrayVal.TupleRemove(hv_GrayVal.TupleFind(hv_GrayValWeight));
    Threshold(ho_ImageWeight, &ho_WeightsRegion, hv_GrayValWeight, hv_GrayValWeight);
    //
    //Visualize the respective group.
    hv_ColorIndex = (((hv_GrayValWeight/hv_SegMaxWeight)*(hv_NumColors-1)).TupleCeil()).TupleInt();
    hv_ClassColor = HTuple(hv_WeightsColorsAlpha[hv_ColorIndex]);
    if (HDevWindowStack::IsOpen())
      SetColor(HDevWindowStack::GetActive(),hv_ClassColor);
    if (HDevWindowStack::IsOpen())
      DispObj(ho_WeightsRegion, HDevWindowStack::GetActive());
  }
  return;
}

// Chapter: Develop
// Short Description: Open a new graphics window that preserves the aspect ratio of the given image size. 
void dev_open_window_fit_size (HTuple hv_Row, HTuple hv_Column, HTuple hv_Width, 
    HTuple hv_Height, HTuple hv_WidthLimit, HTuple hv_HeightLimit, LONG PictureControlID,HTuple *hv_WindowHandle)
{

  // Local iconic variables

  // Local control variables
  HTuple  hv_MinWidth, hv_MaxWidth, hv_MinHeight;
  HTuple  hv_MaxHeight, hv_ResizeFactor, hv_TempWidth, hv_TempHeight;
  HTuple  hv_WindowWidth, hv_WindowHeight;

  //This procedure open a new graphic window
  //such that it fits into the limits specified by WidthLimit
  //and HeightLimit, but also maintains the correct aspect ratio
  //given by Width and Height.
  //
  //If it is impossible to match the minimum and maximum extent requirements
  //at the same time (f.e. if the image is very long but narrow),
  //the maximum value gets a higher priority.
  //
  //Parse input tuple WidthLimit
  if (0 != (HTuple(int((hv_WidthLimit.TupleLength())==0)).TupleOr(int(hv_WidthLimit<0))))
  {
    hv_MinWidth = 500;
    hv_MaxWidth = 800;
  }
  else if (0 != (int((hv_WidthLimit.TupleLength())==1)))
  {
    hv_MinWidth = 0;
    hv_MaxWidth = hv_WidthLimit;
  }
  else
  {
    hv_MinWidth = ((const HTuple&)hv_WidthLimit)[0];
    hv_MaxWidth = ((const HTuple&)hv_WidthLimit)[1];
  }
  //Parse input tuple HeightLimit
  if (0 != (HTuple(int((hv_HeightLimit.TupleLength())==0)).TupleOr(int(hv_HeightLimit<0))))
  {
    hv_MinHeight = 400;
    hv_MaxHeight = 600;
  }
  else if (0 != (int((hv_HeightLimit.TupleLength())==1)))
  {
    hv_MinHeight = 0;
    hv_MaxHeight = hv_HeightLimit;
  }
  else
  {
    hv_MinHeight = ((const HTuple&)hv_HeightLimit)[0];
    hv_MaxHeight = ((const HTuple&)hv_HeightLimit)[1];
  }
  //
  //Test, if window size has to be changed.
  hv_ResizeFactor = 1;
  //First, expand window to the minimum extents (if necessary).
  if (0 != (HTuple(int(hv_MinWidth>hv_Width)).TupleOr(int(hv_MinHeight>hv_Height))))
  {
    hv_ResizeFactor = (((hv_MinWidth.TupleReal())/hv_Width).TupleConcat((hv_MinHeight.TupleReal())/hv_Height)).TupleMax();
  }
  hv_TempWidth = hv_Width*hv_ResizeFactor;
  hv_TempHeight = hv_Height*hv_ResizeFactor;
  //Then, shrink window to maximum extents (if necessary).
  if (0 != (HTuple(int(hv_MaxWidth<hv_TempWidth)).TupleOr(int(hv_MaxHeight<hv_TempHeight))))
  {
    hv_ResizeFactor = hv_ResizeFactor*((((hv_MaxWidth.TupleReal())/hv_TempWidth).TupleConcat((hv_MaxHeight.TupleReal())/hv_TempHeight)).TupleMin());
  }
  hv_WindowWidth = hv_Width*hv_ResizeFactor;
  hv_WindowHeight = hv_Height*hv_ResizeFactor;
  //Resize window
  SetWindowAttr("background_color","black");
  OpenWindow(hv_Row,hv_Column,hv_WindowWidth,hv_WindowHeight, PictureControlID,"visible","",&(*hv_WindowHandle));
  HDevWindowStack::Push((*hv_WindowHandle));
  if (HDevWindowStack::IsOpen())
    SetPart(HDevWindowStack::GetActive(),0, 0, hv_Height-1, hv_Width-1);
  return;
}

// Chapter: Develop
// Short Description: Resizes a graphics window with a given maximum extent such that it preserves the aspect ratio of a given width and height 
void dev_resize_window_fit_size (HTuple hv_Row, HTuple hv_Column, HTuple hv_Width, 
    HTuple hv_Height, HTuple hv_WidthLimit, HTuple hv_HeightLimit)
{

  // Local iconic variables

  // Local control variables
  HTuple  hv_MinWidth, hv_MaxWidth, hv_MinHeight;
  HTuple  hv_MaxHeight, hv_ResizeFactor, hv_TempWidth, hv_TempHeight;
  HTuple  hv_WindowWidth, hv_WindowHeight;

  //This procedure adjusts the size of the current window
  //such that it fits into the limits specified by WidthLimit
  //and HeightLimit, but also maintains the correct aspect ratio
  //given by Width and Height.
  //
  //If it is impossible to match the minimum and maximum extent requirements
  //at the same time (f.e. if the image is very long but narrow),
  //the maximum value gets a higher priority.
  //
  //Parse input tuple WidthLimit
  if (0 != (HTuple(int((hv_WidthLimit.TupleLength())==0)).TupleOr(int(hv_WidthLimit<0))))
  {
    hv_MinWidth = 500;
    hv_MaxWidth = 800;
  }
  else if (0 != (int((hv_WidthLimit.TupleLength())==1)))
  {
    hv_MinWidth = 0;
    hv_MaxWidth = hv_WidthLimit;
  }
  else
  {
    hv_MinWidth = ((const HTuple&)hv_WidthLimit)[0];
    hv_MaxWidth = ((const HTuple&)hv_WidthLimit)[1];
  }
  //Parse input tuple HeightLimit
  if (0 != (HTuple(int((hv_HeightLimit.TupleLength())==0)).TupleOr(int(hv_HeightLimit<0))))
  {
    hv_MinHeight = 400;
    hv_MaxHeight = 600;
  }
  else if (0 != (int((hv_HeightLimit.TupleLength())==1)))
  {
    hv_MinHeight = 0;
    hv_MaxHeight = hv_HeightLimit;
  }
  else
  {
    hv_MinHeight = ((const HTuple&)hv_HeightLimit)[0];
    hv_MaxHeight = ((const HTuple&)hv_HeightLimit)[1];
  }
  //
  //Test, if window size has to be changed.
  hv_ResizeFactor = 1;
  //First, expand window to the minimum extents (if necessary).
  if (0 != (HTuple(int(hv_MinWidth>hv_Width)).TupleOr(int(hv_MinHeight>hv_Height))))
  {
    hv_ResizeFactor = (((hv_MinWidth.TupleReal())/hv_Width).TupleConcat((hv_MinHeight.TupleReal())/hv_Height)).TupleMax();
  }
  hv_TempWidth = hv_Width*hv_ResizeFactor;
  hv_TempHeight = hv_Height*hv_ResizeFactor;
  //Then, shrink window to maximum extents (if necessary).
  if (0 != (HTuple(int(hv_MaxWidth<hv_TempWidth)).TupleOr(int(hv_MaxHeight<hv_TempHeight))))
  {
    hv_ResizeFactor = hv_ResizeFactor*((((hv_MaxWidth.TupleReal())/hv_TempWidth).TupleConcat((hv_MaxHeight.TupleReal())/hv_TempHeight)).TupleMin());
  }
  hv_WindowWidth = hv_Width*hv_ResizeFactor;
  hv_WindowHeight = hv_Height*hv_ResizeFactor;
  //Resize window
  if (HDevWindowStack::IsOpen())
    SetWindowExtents(HDevWindowStack::GetActive(),hv_Row, hv_Column, hv_WindowWidth, 
        hv_WindowHeight);
  if (HDevWindowStack::IsOpen())
    SetPart(HDevWindowStack::GetActive(),0, 0, hv_Height-1, hv_Width-1);
  return;
}

// Chapter: Develop
// Short Description: Switch dev_update_pc, dev_update_var and dev_update_window to 'off'. 
void dev_update_off ()
{

  //This procedure sets different update settings to 'off'.
  //This is useful to get the best performance and reduce overhead.
  //
  // dev_update_pc(...); only in hdevelop
  // dev_update_var(...); only in hdevelop
  // dev_update_window(...); only in hdevelop
  return;
}

// Chapter: Time
// Short Description: Estimate the remaining time for a task given the current progress. 
void estimate_progress (HTuple hv_SecondsStart, HTuple hv_ProgressMin, HTuple hv_ProgressCurrent, 
    HTuple hv_ProgressMax, HTuple *hv_SecondsElapsed, HTuple *hv_SecondsRemaining, 
    HTuple *hv_ProgressPercent, HTuple *hv_ProgressPerSecond)
{

  // Local iconic variables

  // Local control variables
  HTuple  hv_SecondsNow, hv_Epsilon, hv_ProgressRemaining;

  //
  //This procedure estimates the remaining time in seconds,
  //given a start time and a progress value.
  //
  //Get current time.
  CountSeconds(&hv_SecondsNow);
  //
  //Get elapsed time span.
  (*hv_SecondsElapsed) = hv_SecondsNow-hv_SecondsStart;
  //
  //A very small additive constant to avoid division by zero.
  hv_Epsilon = 1e-6;
  //
  //Estimate remaining time based on elapsed time.
  hv_ProgressRemaining = hv_ProgressMax-hv_ProgressCurrent;
  (*hv_ProgressPerSecond) = ((hv_ProgressCurrent-hv_ProgressMin).TupleReal())/(((*hv_SecondsElapsed).TupleReal())+hv_Epsilon);
  (*hv_SecondsRemaining) = hv_ProgressRemaining/((*hv_ProgressPerSecond)+hv_Epsilon);
  //
  //Get current progress in percent.
  (*hv_ProgressPercent) = (100*((hv_ProgressCurrent-hv_ProgressMin).TupleReal()))/(((hv_ProgressMax-hv_ProgressMin).TupleReal())+hv_Epsilon);
  return;
}

// Chapter: Deep Learning / Evaluation
// Short Description: Evaluate the model given by DLModelHandle on the selected samples of DLDataset. 
void evaluate_dl_model (HTuple hv_DLDataset, HTuple hv_DLModelHandle, HTuple hv_SampleSelectMethod, 
    HTuple hv_SampleSelectValues, HTuple hv_GenParam, HTuple *hv_EvaluationResult, 
    HTuple *hv_EvalParams)
{

  // Local iconic variables

  // Local control variables
  HTuple  hv_ModelType, hv_ClassIDs, hv_BatchSize;
  HTuple  hv_InstanceType, hv_Exception, hv_IgnoreDirection;
  HTuple  hv_Measures, hv_ModelIgnoreClassIDs, hv_ShowProgress;
  HTuple  hv_GenParamKeys, hv_GenParamIndex, hv_EvaluationType;
  HTuple  hv_NumClasses, hv_EvaluateInstances, hv_IoUThreshold;
  HTuple  hv_MaxNumDetections, hv_AreaRanges, hv_DetailedEvaluation;
  HTuple  hv_AllocationBlockLength, hv_IgnoreClassIDs, hv_AllIgnoreClassIDs;
  HTuple  hv_EvaluateClassIDs, hv_ClassesToEvaluate, hv_ClassNames;
  HTuple  hv_KeyExists, hv_ClassIDsToEvaluate, hv_AnomalyClassificationThresholds;
  HTuple  hv_ClassInfoExists, hv_DatasetClassIDs, hv_ClassIDsToClassNames;
  HTuple  hv_EvaluateClassNames, hv_DLSamples, hv_SampleIndices;
  HTuple  hv_NumSamples, hv_NumBatches, hv_RunningMeasures;
  HTuple  hv_Progress, hv_TaskInfo, hv_SecondsStart, hv_BatchIndex;
  HTuple  hv_BatchStart, hv_BatchEnd, hv_SamplesIndicesBatch;
  HTuple  hv_DLSamplesBatch, hv_DLResultsBatch, hv_SecondsElapsed;
  HTuple  hv_SecondsRemaining, hv_ProgressPercent, hv_ProgressPerSecond;
  HTuple  hv_TimeElapsedString, hv_TimeRemainingString;

  //This procedure applies the model given by DLModelHandle on the selected samples
  //of DLDataset and evaluates the results against the ground truth annotations
  //to calculate evaluation measures.
  //
  //Input:
  // - DLDataset.
  // - DLModelHandle.
  // - SampleSelectMethod: Method by which the samples are selected.
  // - SampleSelectValues: Identifier used to retrieve the samples from the DLDataset
  //                       for the corresponding selection method.
  // - GenParam: Parameters of the evaluation that should be changed from the default.
  //
  //Output:
  // - EvaluationResult: Dictionary containing the output measures.
  // - EvalParams: Dictionary with the used evaluation parameters.
  //
  //** Initialization: ***
  //
  GetDlModelParam(hv_DLModelHandle, "type", &hv_ModelType);
  if (0 != (HTuple(HTuple(HTuple(int(hv_ModelType!=HTuple("anomaly_detection"))).TupleAnd(int(hv_ModelType!=HTuple("classification")))).TupleAnd(int(hv_ModelType!=HTuple("detection")))).TupleAnd(int(hv_ModelType!=HTuple("segmentation")))))
  {
    throw HException("Unknown model type");
  }
  //
  //Get the class IDs as set in the model.
  if (0 != (int(hv_ModelType==HTuple("anomaly_detection"))))
  {
    //Default for anomaly detection is 0,1.
    hv_ClassIDs.Clear();
    hv_ClassIDs[0] = 0;
    hv_ClassIDs[1] = 1;
  }
  else
  {
    GetDlModelParam(hv_DLModelHandle, "class_ids", &hv_ClassIDs);
  }
  //
  //Get the batch size as set in the model.
  GetDlModelParam(hv_DLModelHandle, "batch_size", &hv_BatchSize);
  //
  //Generate default parameters.
  create_evaluation_default_param(hv_ModelType, hv_ClassIDs, &(*hv_EvalParams));
  //
  //Get model specific information.
  if (0 != (int(hv_ModelType==HTuple("detection"))))
  {
    try
    {
      GetDlModelParam(hv_DLModelHandle, "instance_type", &hv_InstanceType);
    }
    // catch (Exception) 
    catch (HException &HDevExpDefaultException)
    {
      HDevExpDefaultException.ToHTuple(&hv_Exception);
      hv_InstanceType = "rectangle1";
    }
    //Overwrite the instance_type.
    SetDictTuple((*hv_EvalParams), "instance_type", hv_InstanceType);
    //For rectangle2 detection with ignore_direction set to false, we also evaluate the
    //precision of the predicted angle using the Score of Angle Precision (SoAP).
    if (0 != (int(hv_InstanceType==HTuple("rectangle2"))))
    {
      GetDlModelParam(hv_DLModelHandle, "ignore_direction", &hv_IgnoreDirection);
      if (0 != (int(hv_IgnoreDirection==HTuple("false"))))
      {
        GetDictTuple((*hv_EvalParams), "measures", &hv_Measures);
        SetDictTuple((*hv_EvalParams), "measures", hv_Measures.TupleConcat("soap"));
      }
    }
  }
  else if (0 != (int(hv_ModelType==HTuple("segmentation"))))
  {
    GetDlModelParam(hv_DLModelHandle, "ignore_class_ids", &hv_ModelIgnoreClassIDs);
  }
  //
  //By default we do not show the progress of evaluation.
  hv_ShowProgress = 0;
  //
  //Set user specified parameters.
  if (0 != (int(hv_GenParam!=HTuple())))
  {
    GetDictParam(hv_GenParam, "keys", HTuple(), &hv_GenParamKeys);
    {
    HTuple end_val65 = (hv_GenParamKeys.TupleLength())-1;
    HTuple step_val65 = 1;
    for (hv_GenParamIndex=0; hv_GenParamIndex.Continue(end_val65, step_val65); hv_GenParamIndex += step_val65)
    {
      if (0 != (int(HTuple(hv_GenParamKeys[hv_GenParamIndex])==HTuple("measures"))))
      {
        //Set measures.
        GetDictTuple(hv_GenParam, "measures", &hv_Measures);
        SetDictTuple((*hv_EvalParams), "measures", hv_Measures);
      }
      else if (0 != (int(HTuple(hv_GenParamKeys[hv_GenParamIndex])==HTuple("evaluation_type"))))
      {
        //Set evaluation type.
        GetDictTuple(hv_GenParam, "evaluation_type", &hv_EvaluationType);
        SetDictTuple((*hv_EvalParams), "evaluation_type", hv_EvaluationType);
      }
      else if (0 != (int(HTuple(hv_GenParamKeys[hv_GenParamIndex])==HTuple("class_ids"))))
      {
        //Set class ids.
        GetDictTuple(hv_GenParam, "class_ids", &hv_ClassIDs);
        SetDictTuple((*hv_EvalParams), "class_ids", hv_ClassIDs);
      }
      else if (0 != (int(HTuple(hv_GenParamKeys[hv_GenParamIndex])==HTuple("num_classes"))))
      {
        //Set number of classes.
        GetDictTuple(hv_GenParam, "num_classes", &hv_NumClasses);
        SetDictTuple((*hv_EvalParams), "num_classes", hv_NumClasses);
      }
      else if (0 != (int(HTuple(hv_GenParamKeys[hv_GenParamIndex])==HTuple("evaluate_instances"))))
      {
        //Set boolean for instance-based evaluation.
        GetDictTuple(hv_GenParam, "evaluate_instances", &hv_EvaluateInstances);
        SetDictTuple((*hv_EvalParams), "evaluate_instances", hv_EvaluateInstances);
      }
      else if (0 != (int(HTuple(hv_GenParamKeys[hv_GenParamIndex])==HTuple("show_progress"))))
      {
        //Show the progress of the evaluation.
        GetDictTuple(hv_GenParam, "show_progress", &hv_ShowProgress);
        hv_ShowProgress = HTuple(int(hv_ShowProgress==HTuple("true"))).TupleOr(int(hv_ShowProgress==1));
      }
      else if (0 != (HTuple(int(HTuple(hv_GenParamKeys[hv_GenParamIndex])==HTuple("iou_threshold"))).TupleAnd(int(hv_ModelType==HTuple("detection")))))
      {
        //Set IoU threshold.
        GetDictTuple(hv_GenParam, "iou_threshold", &hv_IoUThreshold);
        SetDictTuple((*hv_EvalParams), "iou_threshold", hv_IoUThreshold);
      }
      else if (0 != (HTuple(int(HTuple(hv_GenParamKeys[hv_GenParamIndex])==HTuple("max_num_detections"))).TupleAnd(int(hv_ModelType==HTuple("detection")))))
      {
        //Set maximal number detections.
        GetDictTuple(hv_GenParam, "max_num_detections", &hv_MaxNumDetections);
        SetDictTuple((*hv_EvalParams), "max_num_detections", hv_MaxNumDetections);
      }
      else if (0 != (HTuple(int(HTuple(hv_GenParamKeys[hv_GenParamIndex])==HTuple("area_ranges"))).TupleAnd(int(hv_ModelType==HTuple("detection")))))
      {
        //Set area ranges.
        GetDictTuple(hv_GenParam, "area_ranges", &hv_AreaRanges);
        SetDictTuple((*hv_EvalParams), "area_ranges", hv_AreaRanges);
      }
      else if (0 != (HTuple(int(HTuple(hv_GenParamKeys[hv_GenParamIndex])==HTuple("detailed_evaluation"))).TupleAnd(int(hv_ModelType==HTuple("detection")))))
      {
        //Set detailed evaluation.
        GetDictTuple(hv_GenParam, "detailed_evaluation", &hv_DetailedEvaluation);
        SetDictTuple((*hv_EvalParams), "detailed_evaluation", hv_DetailedEvaluation);
      }
      else if (0 != (HTuple(int(HTuple(hv_GenParamKeys[hv_GenParamIndex])==HTuple("allocation_block_length"))).TupleAnd(int(hv_ModelType==HTuple("detection")))))
      {
        //Set length of blocks that are allocated during evaluation.
        GetDictTuple(hv_GenParam, "allocation_block_length", &hv_AllocationBlockLength);
        SetDictTuple((*hv_EvalParams), "allocation_block_length", hv_AllocationBlockLength);
      }
      else if (0 != (HTuple(int(HTuple(hv_GenParamKeys[hv_GenParamIndex])==HTuple("interpolate_pr_curves"))).TupleAnd(int(hv_ModelType==HTuple("detection")))))
      {
        //Set interpolation of precision-recall curves.
        GetDictTuple(hv_GenParam, "interpolate_pr_curves", &hv_DetailedEvaluation);
        SetDictTuple((*hv_EvalParams), "interpolate_pr_curves", hv_DetailedEvaluation);
      }
      else if (0 != (HTuple(int(HTuple(hv_GenParamKeys[hv_GenParamIndex])==HTuple("ignore_class_ids"))).TupleAnd(int(hv_ModelType==HTuple("segmentation")))))
      {
        //Set ignore class IDs.
        GetDictTuple(hv_GenParam, "ignore_class_ids", &hv_IgnoreClassIDs);
        //Merge the specified ignore class IDs with the model ignore class IDs.
        hv_AllIgnoreClassIDs = ((hv_ModelIgnoreClassIDs.TupleConcat(hv_IgnoreClassIDs)).TupleSort()).TupleUniq();
        SetDictTuple((*hv_EvalParams), "ignore_class_ids", hv_AllIgnoreClassIDs);
        //Remove the ignore class IDs from the model class IDs.
        TupleDifference(hv_ClassIDs, hv_IgnoreClassIDs, &hv_EvaluateClassIDs);
        SetDictTuple((*hv_EvalParams), "class_ids", hv_EvaluateClassIDs);
        SetDictTuple((*hv_EvalParams), "num_classes", hv_EvaluateClassIDs.TupleLength());
      }
      else if (0 != (HTuple(int(HTuple(hv_GenParamKeys[hv_GenParamIndex])==HTuple("class_names_to_evaluate"))).TupleAnd(int(hv_ModelType==HTuple("classification")))))
      {
        //Class names to be used in evaluation.
        //To transform the names to IDs later, one has to remember the class names.
        GetDictTuple(hv_GenParam, "class_names_to_evaluate", &hv_ClassesToEvaluate);
        GetDlModelParam(hv_DLModelHandle, "class_names", &hv_ClassNames);
        SetDictTuple((*hv_EvalParams), "class_names_to_evaluate", hv_ClassesToEvaluate);
        SetDictTuple((*hv_EvalParams), "class_names", hv_ClassNames);
        GetDictParam((*hv_EvalParams), "key_exists", "class_ids_to_evaluate", &hv_KeyExists);
        if (0 != hv_KeyExists)
        {
          //To avoid inconsistent class names/IDs, remove the older ones.
          RemoveDictKey((*hv_EvalParams), "class_ids_to_evaluate");
        }
      }
      else if (0 != (HTuple(int(HTuple(hv_GenParamKeys[hv_GenParamIndex])==HTuple("class_ids_to_evaluate"))).TupleAnd(int(hv_ModelType==HTuple("classification")))))
      {
        //Class IDs to be used in evaluation.
        GetDictTuple(hv_GenParam, "class_ids_to_evaluate", &hv_ClassIDsToEvaluate);
        SetDictTuple((*hv_EvalParams), "class_ids_to_evaluate", hv_ClassIDsToEvaluate);
        GetDictParam((*hv_EvalParams), "key_exists", "class_names_to_evaluate", &hv_KeyExists);
        if (0 != hv_KeyExists)
        {
          //To avoid inconsistent class names/IDs, remove the older ones.
          RemoveDictKey((*hv_EvalParams), "class_names_to_evaluate");
        }
      }
      else if (0 != (int(HTuple(hv_GenParamKeys[hv_GenParamIndex])==HTuple("anomaly_classification_thresholds"))))
      {
        //Set anomaly classification threshold for confusion matrices.
        GetDictTuple(hv_GenParam, "anomaly_classification_thresholds", &hv_AnomalyClassificationThresholds);
        SetDictTuple((*hv_EvalParams), "anomaly_classification_thresholds", hv_AnomalyClassificationThresholds);
      }
      else
      {
        throw HException(("Unknown parameter : '"+HTuple(hv_GenParamKeys[hv_GenParamIndex]))+"'");
      }
    }
    }
  }
  //
  //Get the class names from the dataset if present.
  GetDictParam(hv_DLDataset, "key_exists", (HTuple("class_names").Append("class_ids")), 
      &hv_ClassInfoExists);
  if (0 != (int((hv_ClassInfoExists.TupleSum())==(hv_ClassInfoExists.TupleLength()))))
  {
    GetDictTuple(hv_DLDataset, "class_names", &hv_ClassNames);
    GetDictTuple(hv_DLDataset, "class_ids", &hv_DatasetClassIDs);
    //Set the class names only for the class IDs that are evaluated.
    GetDictTuple((*hv_EvalParams), "class_ids", &hv_EvaluateClassIDs);
    hv_ClassIDsToClassNames = HTuple((hv_DatasetClassIDs.TupleMax())+1,"");
    hv_ClassIDsToClassNames[hv_DatasetClassIDs] = hv_ClassNames;
    hv_EvaluateClassNames = HTuple(hv_ClassIDsToClassNames[hv_EvaluateClassIDs]);
    //Set the class names to EvalParams.
    SetDictTuple((*hv_EvalParams), "class_names", hv_EvaluateClassNames);
  }
  //
  //Get indices of samples to read from the dataset.
  GetDictTuple(hv_DLDataset, "samples", &hv_DLSamples);
  //
  //Check if there are samples present in the dataset.
  if (0 != (int((hv_DLSamples.TupleLength())==0)))
  {
    throw HException("The provided set of samples in the dataset must be non-empty.");
  }
  //
  //Check for empty SampleSelectValues.
  if (0 != (int((hv_SampleSelectValues.TupleLength())==0)))
  {
    if (0 != (int(hv_SampleSelectMethod==HTuple("image_ids"))))
    {
      throw HException("The provided set of samples of the 'image_ids' selection must be non-empty.");
    }
    else if (0 != (int(hv_SampleSelectMethod==HTuple("sample_indices"))))
    {
      throw HException("The provided set of samples of the 'sample_indices' selection must be non-empty.");
    }
    else
    {
      throw HException("Provide a name for 'split' selection.");
    }
  }
  //
  //
  //Get the sample indices according to the sample selection method.
  hv_SampleIndices = HTuple();
  if (0 != (int(hv_SampleSelectMethod==HTuple("split"))))
  {
    //Get the samples of the split specified.
    find_dl_samples(hv_DLSamples, "split", hv_SampleSelectValues, "or", &hv_SampleIndices);
  }
  else if (0 != (int(hv_SampleSelectMethod==HTuple("image_ids"))))
  {
    //Get the samples specified by 'image_ids'.
    if (0 != (int((hv_SampleSelectValues.TupleLength())>(hv_DLSamples.TupleLength()))))
    {
      throw HException("The number of the image ids provided through 'image_id' is invalid.");
    }
    find_dl_samples(hv_DLSamples, "image_id", hv_SampleSelectValues, "or", &hv_SampleIndices);
  }
  else if (0 != (int(hv_SampleSelectMethod==HTuple("sample_indices"))))
  {
    //Get the samples specified by 'sample_indices'.
    if (0 != (HTuple(int((hv_SampleSelectValues.TupleMin())<0)).TupleOr(int((hv_SampleSelectValues.TupleMax())>((hv_DLSamples.TupleLength())-1)))))
    {
      throw HException("The range of the indices provided through 'sample_indices' is invalid.");
    }
    hv_SampleIndices = hv_SampleSelectValues;
  }
  else
  {
    throw HException(("Unknown sample selection method : '"+hv_SampleSelectMethod)+"'");
  }
  //
  //Get the number of batches.
  hv_NumSamples = hv_SampleIndices.TupleLength();
  hv_NumBatches = ((hv_NumSamples/(hv_BatchSize.TupleReal())).TupleCeil()).TupleInt();
  //
  //Check for empty samples selected by the selection method.
  if (0 != (int(hv_NumSamples==0)))
  {
    throw HException(("No samples present in the dataset that are part of the '"+hv_SampleSelectMethod)+"' selection.");
  }
  //
  //
  //** Running measures are initialized according to evaluation method.
  //
  init_running_evaluation_measures((*hv_EvalParams), &hv_RunningMeasures);
  //
  //
  //** Apply model to each image and gather evaluation information: ***
  //
  //Initialize progress variables.
  if (0 != hv_ShowProgress)
  {
    hv_Progress.Clear();
    hv_Progress[0] = "Procedure: evaluate_dl_model";
    hv_Progress[1] = "";
    hv_Progress[2] = "";
    hv_Progress[3] = "";
    if (0 != (int(hv_ModelType==HTuple("detection"))))
    {
      hv_TaskInfo = "Task: 1/2: Applying the model and collecting running evaluation measures";
      hv_Progress = hv_Progress.TupleConcat(hv_TaskInfo);
    }
    CountSeconds(&hv_SecondsStart);
    // dev_inspect_ctrl(...); only in hdevelop
  }
  //
  //Loop batchwise over the samples to be evaluated.
  {
  HTuple end_val239 = hv_NumBatches-1;
  HTuple step_val239 = 1;
  for (hv_BatchIndex=0; hv_BatchIndex.Continue(end_val239, step_val239); hv_BatchIndex += step_val239)
  {
    hv_BatchStart = hv_BatchIndex*hv_BatchSize;
    hv_BatchEnd = ((hv_BatchStart+hv_BatchSize)-1).TupleMin2(hv_NumSamples-1);
    hv_SamplesIndicesBatch = hv_SampleIndices.TupleSelectRange(hv_BatchStart,hv_BatchEnd);
    //
    //Read samples
    read_dl_samples(hv_DLDataset, hv_SamplesIndicesBatch, &hv_DLSamplesBatch);
    //
    //Apply the model.
    ApplyDlModel(hv_DLModelHandle, hv_DLSamplesBatch, HTuple(), &hv_DLResultsBatch);
    //
    //Update the running measures.
    update_running_evaluation_measures(hv_DLSamplesBatch, hv_DLResultsBatch, (*hv_EvalParams), 
        hv_RunningMeasures);
    //
    //Provide progress information.
    if (0 != hv_ShowProgress)
    {
      if (0 != (HTuple(int((hv_BatchIndex%10)==1)).TupleOr(int(hv_BatchIndex==(hv_NumBatches-1)))))
      {
        estimate_progress(hv_SecondsStart, 0, hv_BatchIndex, hv_NumBatches-1, &hv_SecondsElapsed, 
            &hv_SecondsRemaining, &hv_ProgressPercent, &hv_ProgressPerSecond);
        timespan_string(hv_SecondsElapsed, "auto", &hv_TimeElapsedString);
        timespan_string(hv_SecondsRemaining, "top2", &hv_TimeRemainingString);
        hv_Progress[1] = ("Progress: "+(hv_ProgressPercent.TupleRound()))+" %";
        hv_Progress[2] = "Time elapsed: "+hv_TimeElapsedString;
        hv_Progress[3] = "Time left: "+hv_TimeRemainingString;
      }
    }
  }
  }
  //
  //Provide progress information.
  if (0 != (hv_ShowProgress.TupleAnd(int(hv_ModelType==HTuple("detection")))))
  {
    hv_Progress.Clear();
    hv_Progress[0] = "Procedure: evaluate_dl_model";
    hv_Progress[1] = "";
    hv_Progress[2] = "";
    hv_Progress[1] = "Please wait...";
    hv_Progress[2] = "Task: 2/2: Calculating final evaluation measures";
  }
  //
  //
  //** Do the actual calculation of measures: ***
  //
  calculate_evaluation_measures(hv_RunningMeasures, (*hv_EvalParams), &(*hv_EvaluationResult));
  //
  //Close progress inspect.
  if (0 != hv_ShowProgress)
  {
    hv_Progress = "Done.";
    // dev_close_inspect_ctrl(...); only in hdevelop
  }
  //
  return;
}

// Chapter: Deep Learning / Model
// Short Description: Retrieve the indices of Samples that contain KeyName matching KeyValue according to the Mode set. 
void find_dl_samples (HTuple hv_Samples, HTuple hv_KeyName, HTuple hv_KeyValue, HTuple hv_Mode, 
    HTuple *hv_SampleIndices)
{

  // Local iconic variables

  // Local control variables
  HTuple  hv_NumKeyValues, hv_NumFound, hv_SampleIndex;
  HTuple  hv_Sample, hv_KeyExists, hv_Tuple, hv_Hit, hv_ValueIndex;
  HTuple  hv_Value;

  //
  //This procedure gets the indices of the samples that contain the
  //requested KeyName matching the requested KeyValue according to the Mode.
  //If there is no match, an empty tuple [] will be returned.
  //
  //Check input parameters.
  if (0 != (int((hv_KeyName.TupleLength())!=1)))
  {
    throw HException(HTuple("Invalid KeyName size: ")+(hv_KeyName.TupleLength()));
  }
  if (0 != (int((hv_Mode.TupleLength())!=1)))
  {
    throw HException(HTuple("Invalid Mode size: ")+(hv_Mode.TupleLength()));
  }
  if (0 != (HTuple(HTuple(int(hv_Mode!=HTuple("match"))).TupleAnd(int(hv_Mode!=HTuple("or")))).TupleAnd(int(hv_Mode!=HTuple("contain")))))
  {
    throw HException("Invalid Mode value: "+hv_Mode);
  }
  hv_NumKeyValues = hv_KeyValue.TupleLength();
  if (0 != (HTuple(int(hv_Mode==HTuple("contain"))).TupleAnd(int(hv_NumKeyValues<1))))
  {
    throw HException("Invalid KeyValue size for contain Mode: "+hv_NumKeyValues);
  }
  //
  //Find the indices.
  (*hv_SampleIndices) = HTuple(hv_Samples.TupleLength(),0);
  hv_NumFound = 0;
  //
  {
  HTuple end_val24 = (hv_Samples.TupleLength())-1;
  HTuple step_val24 = 1;
  for (hv_SampleIndex=0; hv_SampleIndex.Continue(end_val24, step_val24); hv_SampleIndex += step_val24)
  {
    hv_Sample = HTuple(hv_Samples[hv_SampleIndex]);
    GetDictParam(hv_Sample, "key_exists", hv_KeyName, &hv_KeyExists);
    if (0 != hv_KeyExists)
    {
      GetDictTuple(hv_Sample, hv_KeyName, &hv_Tuple);
      if (0 != (int(hv_Mode==HTuple("match"))))
      {
        //Mode 'match': Tuple must be equal KeyValue.
        hv_Hit = int(hv_Tuple==hv_KeyValue);
      }
      else if (0 != (HTuple(int(hv_Mode==HTuple("or"))).TupleAnd(int((hv_Tuple.TupleLength())==1))))
      {
        //Mode 'or': Tuple must have only 1 element and it has to be equal to any of KeyValues elements.
        hv_Hit = int((hv_KeyValue.TupleFindFirst(hv_Tuple))>=0);
      }
      else if (0 != (int(hv_Mode==HTuple("contain"))))
      {
        //Mode 'contain': Tuple must contain any of the elements in KeyValue.
        {
        HTuple end_val37 = hv_NumKeyValues-1;
        HTuple step_val37 = 1;
        for (hv_ValueIndex=0; hv_ValueIndex.Continue(end_val37, step_val37); hv_ValueIndex += step_val37)
        {
          hv_Value = HTuple(hv_KeyValue[hv_ValueIndex]);
          hv_Hit = int((hv_Tuple.TupleFindFirst(hv_Value))>=0);
          if (0 != hv_Hit)
          {
            break;
          }
        }
        }
      }
      else
      {
        //Unsupported configuration.
        hv_Hit = 0;
      }
      if (0 != hv_Hit)
      {
        (*hv_SampleIndices)[hv_NumFound] = hv_SampleIndex;
        hv_NumFound += 1;
      }
    }
  }
  }
  (*hv_SampleIndices) = (*hv_SampleIndices).TupleSelectRange(0,hv_NumFound-1);
  return;
}

// Chapter: XLD / Creation
// Short Description: Creates an arrow shaped XLD contour. 
void gen_arrow_contour_xld (HObject *ho_Arrow, HTuple hv_Row1, HTuple hv_Column1, 
    HTuple hv_Row2, HTuple hv_Column2, HTuple hv_HeadLength, HTuple hv_HeadWidth)
{

  // Local iconic variables
  HObject  ho_TempArrow;

  // Local control variables
  HTuple  hv_Length, hv_ZeroLengthIndices, hv_DR;
  HTuple  hv_DC, hv_HalfHeadWidth, hv_RowP1, hv_ColP1, hv_RowP2;
  HTuple  hv_ColP2, hv_Index;

  //This procedure generates arrow shaped XLD contours,
  //pointing from (Row1, Column1) to (Row2, Column2).
  //If starting and end point are identical, a contour consisting
  //of a single point is returned.
  //
  //input parameteres:
  //Row1, Column1: Coordinates of the arrows' starting points
  //Row2, Column2: Coordinates of the arrows' end points
  //HeadLength, HeadWidth: Size of the arrow heads in pixels
  //
  //output parameter:
  //Arrow: The resulting XLD contour
  //
  //The input tuples Row1, Column1, Row2, and Column2 have to be of
  //the same length.
  //HeadLength and HeadWidth either have to be of the same length as
  //Row1, Column1, Row2, and Column2 or have to be a single element.
  //If one of the above restrictions is violated, an error will occur.
  //
  //
  //Init
  GenEmptyObj(&(*ho_Arrow));
  //
  //Calculate the arrow length
  DistancePp(hv_Row1, hv_Column1, hv_Row2, hv_Column2, &hv_Length);
  //
  //Mark arrows with identical start and end point
  //(set Length to -1 to avoid division-by-zero exception)
  hv_ZeroLengthIndices = hv_Length.TupleFind(0);
  if (0 != (int(hv_ZeroLengthIndices!=-1)))
  {
    hv_Length[hv_ZeroLengthIndices] = -1;
  }
  //
  //Calculate auxiliary variables.
  hv_DR = (1.0*(hv_Row2-hv_Row1))/hv_Length;
  hv_DC = (1.0*(hv_Column2-hv_Column1))/hv_Length;
  hv_HalfHeadWidth = hv_HeadWidth/2.0;
  //
  //Calculate end points of the arrow head.
  hv_RowP1 = (hv_Row1+((hv_Length-hv_HeadLength)*hv_DR))+(hv_HalfHeadWidth*hv_DC);
  hv_ColP1 = (hv_Column1+((hv_Length-hv_HeadLength)*hv_DC))-(hv_HalfHeadWidth*hv_DR);
  hv_RowP2 = (hv_Row1+((hv_Length-hv_HeadLength)*hv_DR))-(hv_HalfHeadWidth*hv_DC);
  hv_ColP2 = (hv_Column1+((hv_Length-hv_HeadLength)*hv_DC))+(hv_HalfHeadWidth*hv_DR);
  //
  //Finally create output XLD contour for each input point pair
  {
  HTuple end_val45 = (hv_Length.TupleLength())-1;
  HTuple step_val45 = 1;
  for (hv_Index=0; hv_Index.Continue(end_val45, step_val45); hv_Index += step_val45)
  {
    if (0 != (int(HTuple(hv_Length[hv_Index])==-1)))
    {
      //Create_ single points for arrows with identical start and end point
      GenContourPolygonXld(&ho_TempArrow, HTuple(hv_Row1[hv_Index]), HTuple(hv_Column1[hv_Index]));
    }
    else
    {
      //Create arrow contour
      GenContourPolygonXld(&ho_TempArrow, ((((HTuple(hv_Row1[hv_Index]).TupleConcat(HTuple(hv_Row2[hv_Index]))).TupleConcat(HTuple(hv_RowP1[hv_Index]))).TupleConcat(HTuple(hv_Row2[hv_Index]))).TupleConcat(HTuple(hv_RowP2[hv_Index]))).TupleConcat(HTuple(hv_Row2[hv_Index])), 
          ((((HTuple(hv_Column1[hv_Index]).TupleConcat(HTuple(hv_Column2[hv_Index]))).TupleConcat(HTuple(hv_ColP1[hv_Index]))).TupleConcat(HTuple(hv_Column2[hv_Index]))).TupleConcat(HTuple(hv_ColP2[hv_Index]))).TupleConcat(HTuple(hv_Column2[hv_Index])));
    }
    ConcatObj((*ho_Arrow), ho_TempArrow, &(*ho_Arrow));
  }
  }
  return;
}

// Chapter: Deep Learning / Classification
// Short Description: Compute a confusion matrix, which an be visualized and/or returned. 
void gen_confusion_matrix (HTuple hv_GroundTruthLabels, HTuple hv_PredictedClasses, 
    HTuple hv_GenParamName, HTuple hv_GenParamValue, HTuple hv_WindowHandle, HTuple *hv_ConfusionMatrix)
{

  // Local iconic variables

  // Local control variables
  HTuple  hv_DisplayMatrix, hv_ReturnMatrix, hv_DisplayColor;
  HTuple  hv_DisplayColumnWidth, hv_GenParamIndex, hv_CalculateRelativeMatrix;
  HTuple  hv_Classes, hv_NumClasses, hv_AbsoluteMatrixID;
  HTuple  hv_RelativeMatrixID, hv_ColumnMatrix, hv_Class;
  HTuple  hv_ThisLabel, hv_NumClassGroundTruth, hv_RowMatrix;
  HTuple  hv_PredictedClass, hv_ThisPredictedClass, hv_NumMatches;
  HTuple  hv_RelativeError, hv_StringWidths, hv_StringIndex;
  HTuple  hv_String, hv_Ascent, hv_Descent, hv_StringWidth;
  HTuple  hv_StringHeight, hv_MaxStringWidth, hv_RowStart;
  HTuple  hv_RowDistance, hv_RowEnd, hv_ColumnStart, hv_ColumnOffset;
  HTuple  hv_ColumnEnd, hv_Width, hv_Height, hv_WidthLimit;
  HTuple  hv_HeightLimit, hv_TextRow, hv_TextColumn, hv_Index;
  HTuple  hv_Text, hv_Row, hv_Column, hv_AbsoluteTransposedMatrixID;
  HTuple  hv_MatrixText, hv_MatrixMaxID, hv_MaxValue, hv_StringConversion;
  HTuple  hv_RelativeTransposedMatrixID, hv_TextColor, hv_RelativeValues;
  HTuple  hv_Thresholds, hv_Colors, hv_Greater, hv_Indices;
  HTuple  hv_DiagonalIndex, hv_Value;

  //This procedure computes a confusion matrix.
  //Therefore, it compares the classes
  //assigned in GroundTruthLabels and PredictedClasses.
  //The resulting confusion matrix can be
  //visualized, returned, or both.
  //In each case, the output can be changed
  //via generic parameters using GenParamName and GenParamValue.
  //For the visualization, the graphics window
  //must be specified with WindowHandle.
  //
  if (0 != (int((hv_GroundTruthLabels.TupleLength())!=(hv_PredictedClasses.TupleLength()))))
  {
    throw HException("Number of ground truth labels and predicted classes must be equal.");
  }
  //
  //Set generic parameter defaults.
  hv_DisplayMatrix = "absolute";
  hv_ReturnMatrix = "absolute";
  hv_DisplayColor = "true";
  hv_DisplayColumnWidth = "minimal";
  //
  //Parse generic parameters.
  {
  HTuple end_val21 = (hv_GenParamName.TupleLength())-1;
  HTuple step_val21 = 1;
  for (hv_GenParamIndex=0; hv_GenParamIndex.Continue(end_val21, step_val21); hv_GenParamIndex += step_val21)
  {
    if (0 != (int(HTuple(hv_GenParamName[hv_GenParamIndex])==HTuple("display_matrix"))))
    {
      //Set 'display_matrix'.
      hv_DisplayMatrix = HTuple(hv_GenParamValue[hv_GenParamIndex]);
    }
    else if (0 != (int(HTuple(hv_GenParamName[hv_GenParamIndex])==HTuple("return_matrix"))))
    {
      //Set 'return_matrix'.
      hv_ReturnMatrix = HTuple(hv_GenParamValue[hv_GenParamIndex]);
    }
    else if (0 != (int(HTuple(hv_GenParamName[hv_GenParamIndex])==HTuple("display_color"))))
    {
      //Set 'display_color'.
      hv_DisplayColor = HTuple(hv_GenParamValue[hv_GenParamIndex]);
    }
    else if (0 != (int(HTuple(hv_GenParamName[hv_GenParamIndex])==HTuple("display_column_width"))))
    {
      //Set 'display_column_width'.
      hv_DisplayColumnWidth = HTuple(hv_GenParamValue[hv_GenParamIndex]);
    }
    else
    {
      throw HException(("Unknown generic parameter: '"+HTuple(hv_GenParamName[hv_GenParamIndex]))+"'");
    }
  }
  }
  //
  if (0 != (HTuple(HTuple(int(hv_DisplayMatrix==HTuple("relative"))).TupleOr(int(hv_ReturnMatrix==HTuple("relative")))).TupleOr(int(hv_DisplayColor==HTuple("true")))))
  {
    hv_CalculateRelativeMatrix = 1;
  }
  else
  {
    hv_CalculateRelativeMatrix = 0;
  }
  //
  //Calculate the confusion matrix with absolute values
  //and the confusion matrix with relative errors.
  //We start with an empty matrix
  //and add the number of matching labels.
  hv_Classes = (hv_GroundTruthLabels.TupleSort()).TupleUniq();
  hv_NumClasses = hv_Classes.TupleLength();
  CreateMatrix(hv_NumClasses, hv_NumClasses, 0, &hv_AbsoluteMatrixID);
  if (0 != hv_CalculateRelativeMatrix)
  {
    CreateMatrix(hv_NumClasses, hv_NumClasses, 0, &hv_RelativeMatrixID);
  }
  {
  HTuple end_val55 = hv_NumClasses-1;
  HTuple step_val55 = 1;
  for (hv_ColumnMatrix=0; hv_ColumnMatrix.Continue(end_val55, step_val55); hv_ColumnMatrix += step_val55)
  {
    hv_Class = HTuple(hv_Classes[hv_ColumnMatrix]);
    hv_ThisLabel = hv_GroundTruthLabels.TupleEqualElem(hv_Class);
    if (0 != hv_CalculateRelativeMatrix)
    {
      //Obtain the number of ground truth labels per class.
      hv_NumClassGroundTruth = hv_ThisLabel.TupleSum();
    }
    {
    HTuple end_val62 = hv_NumClasses-1;
    HTuple step_val62 = 1;
    for (hv_RowMatrix=0; hv_RowMatrix.Continue(end_val62, step_val62); hv_RowMatrix += step_val62)
    {
      //Select classes for this row/column.
      hv_PredictedClass = HTuple(hv_Classes[hv_RowMatrix]);
      //Check whether the input data
      //corresponds to these classes.
      hv_ThisPredictedClass = hv_PredictedClasses.TupleEqualElem(hv_PredictedClass);
      //Count the number of elements where the predicted class
      //matches the ground truth label.
      hv_NumMatches = ((hv_ThisLabel+hv_ThisPredictedClass).TupleEqualElem(2)).TupleSum();
      //Set value in matrix.
      SetValueMatrix(hv_AbsoluteMatrixID, hv_RowMatrix, hv_ColumnMatrix, hv_NumMatches);
      if (0 != hv_CalculateRelativeMatrix)
      {
        if (0 != (int(hv_NumClassGroundTruth>0)))
        {
          hv_RelativeError = (hv_NumMatches.TupleReal())/hv_NumClassGroundTruth;
        }
        else
        {
          hv_RelativeError = 0;
        }
        SetValueMatrix(hv_RelativeMatrixID, hv_RowMatrix, hv_ColumnMatrix, hv_RelativeError);
      }
    }
    }
  }
  }
  //
  //Return the result.
  if (0 != (int(hv_ReturnMatrix==HTuple("absolute"))))
  {
    CopyMatrix(hv_AbsoluteMatrixID, &(*hv_ConfusionMatrix));
  }
  else if (0 != (int(hv_ReturnMatrix==HTuple("relative"))))
  {
    CopyMatrix(hv_RelativeMatrixID, &(*hv_ConfusionMatrix));
  }
  else if (0 != (int(hv_ReturnMatrix==HTuple("none"))))
  {
    //No matrix is returned.
  }
  else
  {
    throw HException("Unsupported mode for 'return_matrix'");
  }
  //
  //Display the matrix.
  if (0 != (int(hv_DisplayMatrix!=HTuple("none"))))
  {
    //
    //Find maximal string width and set display position parameters.
    hv_StringWidths = HTuple();
    //Get the string width of each class.
    {
    HTuple end_val101 = (hv_Classes.TupleLength())-1;
    HTuple step_val101 = 1;
    for (hv_StringIndex=0; hv_StringIndex.Continue(end_val101, step_val101); hv_StringIndex += step_val101)
    {
      hv_String = HTuple(hv_Classes[hv_StringIndex]);
      GetStringExtents(hv_WindowHandle, hv_String, &hv_Ascent, &hv_Descent, &hv_StringWidth, 
          &hv_StringHeight);
      hv_StringWidths = hv_StringWidths.TupleConcat(hv_StringWidth);
    }
    }
    //The columns should have a minimum width for 4 characters.
    GetStringExtents(hv_WindowHandle, "test", &hv_Ascent, &hv_Descent, &hv_StringWidth, 
        &hv_StringHeight);
    hv_MaxStringWidth = (hv_StringWidths.TupleMax()).TupleMax2(hv_StringWidth);
    //Get the maximum string width
    //and resize the window accordingly.
    hv_RowStart = 80;
    hv_RowDistance = hv_StringHeight+10;
    hv_RowEnd = hv_StringHeight*7;
    hv_ColumnStart = 50+hv_MaxStringWidth;
    hv_ColumnOffset = 20;
    hv_ColumnEnd = hv_ColumnOffset;
    //
    //Adapt the window size to fit the confusion matrix.
    if (0 != (int(hv_DisplayColumnWidth==HTuple("minimal"))))
    {
      //Every column of the confusion matrix is as narrow as possible
      //based to the respective string widths.
      hv_Width = (((hv_StringWidths.TupleSum())+(hv_ColumnOffset*hv_NumClasses))+hv_ColumnStart)+hv_ColumnEnd;
    }
    else if (0 != (int(hv_DisplayColumnWidth==HTuple("equal"))))
    {
      //Every column of the confusion matrix should have the same width.
      //based on the maximum string width.
      hv_Width = (((hv_MaxStringWidth+hv_ColumnOffset)*hv_NumClasses)+hv_ColumnStart)+hv_ColumnEnd;
    }
    else
    {
      throw HException("");
    }
    hv_Height = ((hv_RowDistance*hv_NumClasses)+hv_RowStart)+hv_RowEnd;
    HDevWindowStack::SetActive(hv_WindowHandle);
    if (HDevWindowStack::IsOpen())
      ClearWindow(HDevWindowStack::GetActive());
    //
    //Set reasonable limits for graphics window (adapt if necessary).
    hv_WidthLimit.Clear();
    hv_WidthLimit[0] = 450;
    hv_WidthLimit[1] = 1920;
    hv_HeightLimit.Clear();
    hv_HeightLimit[0] = 250;
    hv_HeightLimit[1] = 1080;
    if (0 != (HTuple(int(hv_Width>HTuple(hv_WidthLimit[1]))).TupleOr(int(hv_Height>HTuple(hv_HeightLimit[1])))))
    {
      throw HException("Confusion Matrix does not fit into graphics window. Please adapt font and/or size limits.");
    }
    dev_resize_window_fit_size(0, 0, hv_Width, hv_Height, hv_WidthLimit, hv_HeightLimit);
    //
    //Get display coordinates.
    //Get row coordinates for display.
    hv_TextRow = HTuple();
    {
    HTuple end_val145 = hv_NumClasses-1;
    HTuple step_val145 = 1;
    for (hv_ColumnMatrix=0; hv_ColumnMatrix.Continue(end_val145, step_val145); hv_ColumnMatrix += step_val145)
    {
      hv_TextRow = hv_TextRow.TupleConcat(HTuple::TupleGenSequence(0,(hv_NumClasses-1)*hv_RowDistance,hv_RowDistance));
    }
    }
    //Get column coordinates for display.
    hv_TextColumn = HTuple();
    {
    HTuple end_val150 = hv_NumClasses-1;
    HTuple step_val150 = 1;
    for (hv_Index=0; hv_Index.Continue(end_val150, step_val150); hv_Index += step_val150)
    {
      hv_TextColumn = hv_TextColumn.TupleConcat(HTuple(hv_NumClasses,hv_ColumnStart));
      if (0 != (int(hv_DisplayColumnWidth==HTuple("minimal"))))
      {
        hv_ColumnStart = (hv_ColumnStart+HTuple(hv_StringWidths[hv_Index]))+hv_ColumnOffset;
      }
      else if (0 != (int(hv_DisplayColumnWidth==HTuple("equal"))))
      {
        hv_ColumnStart = (hv_ColumnStart+hv_MaxStringWidth)+hv_ColumnOffset;
      }
    }
    }
    //Display the confusion matrix with a margin from the top.
    hv_TextRow += hv_RowStart;
    //Display axis titles.
    if (HDevWindowStack::IsOpen())
      DispText(HDevWindowStack::GetActive(),"Ground truth labels", "window", "top", 
          "right", "white", "box", "false");
    if (HDevWindowStack::IsOpen())
      DispText(HDevWindowStack::GetActive(),"Predicted classes", "window", "bottom", 
          "left", "white", "box", "false");
    {
    HTuple end_val163 = (hv_Classes.TupleLength())-1;
    HTuple step_val163 = 1;
    for (hv_Index=0; hv_Index.Continue(end_val163, step_val163); hv_Index += step_val163)
    {
      hv_Text = HTuple(hv_Classes[hv_Index]);
      //Display predicted class names.
      hv_Row = HTuple(hv_TextRow[hv_Index]);
      hv_Column = (HTuple(hv_TextColumn[0])-hv_MaxStringWidth)-hv_ColumnOffset;
      if (HDevWindowStack::IsOpen())
        DispText(HDevWindowStack::GetActive(),hv_Text, "window", hv_Row, hv_Column, 
            "light gray", "box", "false");
      //Display ground truth label names.
      hv_Row = HTuple(hv_TextRow[0])-hv_RowDistance;
      hv_Column = HTuple(hv_TextColumn[hv_Index*hv_NumClasses]);
      if (HDevWindowStack::IsOpen())
        DispText(HDevWindowStack::GetActive(),hv_Text, "window", hv_Row, hv_Column, 
            "light gray", "box", "false");
    }
    }
    //
    //Get the confusion matrix values for display.
    if (0 != (int(hv_DisplayMatrix==HTuple("absolute"))))
    {
      //Displayed matrix corresponds to the transposed returned matrix.
      TransposeMatrix(hv_AbsoluteMatrixID, &hv_AbsoluteTransposedMatrixID);
      GetFullMatrix(hv_AbsoluteTransposedMatrixID, &hv_MatrixText);
      ClearMatrix(hv_AbsoluteTransposedMatrixID);
      //Align the numbers right.
      MaxMatrix(hv_AbsoluteMatrixID, "full", &hv_MatrixMaxID);
      GetFullMatrix(hv_MatrixMaxID, &hv_MaxValue);
      ClearMatrix(hv_MatrixMaxID);
      hv_StringConversion = (((hv_MaxValue.TupleLog10()).TupleCeil()).TupleInt())+".0f";
      hv_MatrixText = hv_MatrixText.TupleString(hv_StringConversion);
    }
    else
    {
      //Displayed matrix corresponds to the transposed returned matrix.
      TransposeMatrix(hv_RelativeMatrixID, &hv_RelativeTransposedMatrixID);
      GetFullMatrix(hv_RelativeTransposedMatrixID, &hv_MatrixText);
      ClearMatrix(hv_RelativeTransposedMatrixID);
      hv_MatrixText = hv_MatrixText.TupleString(".2f");
    }
    //Set color for displayed confusion matrix.
    if (0 != (int(hv_DisplayColor==HTuple("true"))))
    {
      TupleGenConst(hv_MatrixText.TupleLength(), "#666666", &hv_TextColor);
      //Use the relative values to adapt the color of the text.
      TransposeMatrix(hv_RelativeMatrixID, &hv_RelativeTransposedMatrixID);
      GetFullMatrix(hv_RelativeTransposedMatrixID, &hv_RelativeValues);
      ClearMatrix(hv_RelativeTransposedMatrixID);
      //Set the colors and respective thresholds for the off-diagonal values.
      hv_Thresholds.Clear();
      hv_Thresholds[0] = 0.0;
      hv_Thresholds[1] = 0.05;
      hv_Thresholds[2] = 0.1;
      hv_Thresholds[3] = 0.2;
      hv_Colors.Clear();
      hv_Colors[0] = "#8C4D4D";
      hv_Colors[1] = "#B33333";
      hv_Colors[2] = "#D91A1A";
      hv_Colors[3] = "#FF0000";
      {
      HTuple end_val204 = (hv_Thresholds.TupleLength())-1;
      HTuple step_val204 = 1;
      for (hv_Index=0; hv_Index.Continue(end_val204, step_val204); hv_Index += step_val204)
      {
        TupleGreaterElem(hv_RelativeValues, HTuple(hv_Thresholds[hv_Index]), &hv_Greater);
        TupleFind(hv_Greater, 1, &hv_Indices);
        if (0 != (int(hv_Indices!=-1)))
        {
          TupleReplace(hv_TextColor, hv_Indices, HTuple(hv_Colors[hv_Index]), &hv_TextColor);
        }
        else
        {
          break;
        }
      }
      }
      //Set the colors and respective thresholds for the diagonal values.
      hv_Thresholds.Clear();
      hv_Thresholds[0] = -0.01;
      hv_Thresholds[1] = 0.60;
      hv_Thresholds[2] = 0.80;
      hv_Thresholds[3] = 0.90;
      hv_Thresholds[4] = 0.95;
      hv_Thresholds[5] = 0.98;
      hv_Colors.Clear();
      hv_Colors[0] = "#666666";
      hv_Colors[1] = "#508650";
      hv_Colors[2] = "#419C41";
      hv_Colors[3] = "#2BBD2B";
      hv_Colors[4] = "#15DE15";
      hv_Colors[5] = "#00FF00";
      {
      HTuple end_val216 = hv_NumClasses-1;
      HTuple step_val216 = 1;
      for (hv_DiagonalIndex=0; hv_DiagonalIndex.Continue(end_val216, step_val216); hv_DiagonalIndex += step_val216)
      {
        GetValueMatrix(hv_RelativeMatrixID, hv_DiagonalIndex, hv_DiagonalIndex, &hv_Value);
        {
        HTuple end_val218 = (hv_Thresholds.TupleLength())-1;
        HTuple step_val218 = 1;
        for (hv_Index=0; hv_Index.Continue(end_val218, step_val218); hv_Index += step_val218)
        {
          if (0 != (int(hv_Value>HTuple(hv_Thresholds[hv_Index]))))
          {
            hv_TextColor[hv_DiagonalIndex*(hv_NumClasses+1)] = HTuple(hv_Colors[hv_Index]);
          }
          else
          {
            break;
          }
        }
        }
      }
      }
    }
    else
    {
      //Default value for the text color.
      TupleGenConst(hv_MatrixText.TupleLength(), "white", &hv_TextColor);
    }
    //
    //Display confusion matrix.
    if (HDevWindowStack::IsOpen())
      DispText(HDevWindowStack::GetActive(),hv_MatrixText, "window", hv_TextRow, 
          hv_TextColumn, hv_TextColor, "box", "false");
    //
    //Clean up.
    if (0 != hv_CalculateRelativeMatrix)
    {
      ClearMatrix(hv_RelativeMatrixID);
    }
    ClearMatrix(hv_AbsoluteMatrixID);
  }
  return;
}

// Chapter: Deep Learning / Model
// Short Description: The procedure returns DLSample dicts for given sample indices of a DLDataset. 
void gen_dl_samples (HTuple hv_DLDataset, HTuple hv_SampleIndices, HTuple hv_RestrictKeysDLSample, 
    HTuple hv_GenParam, HTuple *hv_DLSampleBatch)
{

  // Local iconic variables
  HObject  ho_ImageRaw, ho_ImageAnomaly, ho_RegionAnomaly;
  HObject  ho_ImageSegmentation;

  // Local control variables
  HTuple  hv_ImageDir, hv_DLSamples, hv_MinIndex;
  HTuple  hv_MaxIndex, hv_InstanceType, hv_IgnoreMissing;
  HTuple  hv_GenParamName, hv_IndexGenParam, hv_DLSamplesProc;
  HTuple  hv_BboxKeyList, hv_ImageIndex, hv_DLSample, hv_ImageID;
  HTuple  hv_ImageName, hv_FileName, hv_Exception, hv_AnomalyLabelExists;
  HTuple  hv_AnomalyLabel, hv_AnomalyFileNameExists, hv_AnomalyDir;
  HTuple  hv_AnomalyFileName, hv_ExceptionImageAnomaly, hv_ExceptionRegionAnomaly;
  HTuple  hv_Width, hv_Height, hv_ImageLabelIDExists, hv_ImageLabelID;
  HTuple  hv_BboxExists, hv_BboxLabels, hv_KeysExist, hv_MissingKeyIndices;
  HTuple  hv_IndexParam, hv_BboxCoord, hv_SegKeyExists, hv_SegmentationDir;
  HTuple  hv_SegmentationName, hv_ExceptionSegmentation;

  //
  //This procedure creates DLSampleBatch, a tuple of DLSample dictionaries, with
  //the image data for each DLDataset sample, that was selected through SampleIndices.
  //The keys to be transferred can be restricted using RestrictKeysDLSample,
  //which is switched off ('off') by default.
  //The procedure returns all generated DLSample dictionaries in the tuple
  //DLSampleBatch.
  //Setting the GenParam 'ignore_missing_labels' controls whether an error is thrown,
  //if no ground truth annotation information is available for a given image.
  //
  //Get the image directory.
  GetDictTuple(hv_DLDataset, "image_dir", &hv_ImageDir);
  //
  //Get the samples from the DLDataset.
  GetDictTuple(hv_DLDataset, "samples", &hv_DLSamples);
  //
  //Check the input values.
  //
  //Check that the given indices are valid.
  TupleMin(hv_SampleIndices, &hv_MinIndex);
  TupleMax(hv_SampleIndices, &hv_MaxIndex);
  if (0 != (HTuple(int(hv_MinIndex<0)).TupleOr(int(hv_MaxIndex>((hv_DLSamples.TupleLength())-1)))))
  {
    throw HException("The given SampleIndices are not within the range of available samples in DLDataset.");
  }
  //
  //Check if the given method is valid.
  if (0 != (int((hv_RestrictKeysDLSample.TupleLength())==1)))
  {
    if (0 != (int((HTuple((((((HTuple("anomaly_detection").Append("detection")).Append("segmentation")).Append("classification")).Append("image_only")).Append("off")).TupleFind(hv_RestrictKeysDLSample)).TupleMax())==-1)))
    {
      throw HException("Unknown RestrictKeysDLSample : "+hv_RestrictKeysDLSample);
    }
  }
  else
  {
    throw HException("RestrictKeysDLSample must be specified by one string.");
  }
  //
  //Generic Parameters.
  //Set default values.
  hv_InstanceType = "rectangle1";
  //For missing labels an error is thrown.
  if (0 != (int(hv_RestrictKeysDLSample==HTuple("off"))))
  {
    hv_IgnoreMissing = 1;
  }
  else
  {
    hv_IgnoreMissing = 0;
  }
  //
  //Transfer generic parameters.
  if (0 != (int(hv_GenParam!=HTuple())))
  {
    GetDictParam(hv_GenParam, "keys", HTuple(), &hv_GenParamName);
    {
    HTuple end_val47 = (hv_GenParamName.TupleLength())-1;
    HTuple step_val47 = 1;
    for (hv_IndexGenParam=0; hv_IndexGenParam.Continue(end_val47, step_val47); hv_IndexGenParam += step_val47)
    {
      if (0 != (int(HTuple(hv_GenParamName[hv_IndexGenParam])==HTuple("ignore_missing_labels"))))
      {
        GetDictTuple(hv_GenParam, "ignore_missing_labels", &hv_IgnoreMissing);
        if (0 != (HTuple(HTuple(int(hv_IgnoreMissing==1)).TupleOr(int(hv_IgnoreMissing==0))).TupleNot()))
        {
          throw HException("The GenParam ignore_missing_labels must be true or false.");
        }
      }
      else if (0 != (int(HTuple(hv_GenParamName[hv_IndexGenParam])==HTuple("instance_type"))))
      {
        if (0 != (int((HTuple((HTuple("detection").Append("off")).TupleFind(hv_RestrictKeysDLSample)).TupleMax())==-1)))
        {
          throw HException("The GenParam instance_type can only be set for RestrictKeysDLSample detection or off.");
        }
        GetDictTuple(hv_GenParam, "instance_type", &hv_InstanceType);
        if (0 != (int((HTuple((HTuple("rectangle1").Append("rectangle2")).TupleFind(hv_InstanceType)).TupleMax())==-1)))
        {
          throw HException("The GenParam instance_type must be either 'rectangle1' or 'rectangle2'.");
        }
      }
      else
      {
        throw HException("Unknown GenParam key : "+HTuple(hv_GenParamName[hv_IndexGenParam]));
      }
    }
    }
  }
  //
  //Get the samples to be processed.
  hv_DLSamplesProc = HTuple(hv_DLSamples[hv_SampleIndices]);
  //
  //Initialize the tuple for collection the DLSample dictionaries.
  (*hv_DLSampleBatch) = HTuple(hv_SampleIndices.TupleLength(),HTuple(HNULL));
  //
  //Set the BboxKeyList according to the InstanceType.
  if (0 != (int((HTuple((HTuple("detection").Append("off")).TupleFind(hv_RestrictKeysDLSample)).TupleMax())!=-1)))
  {
    hv_BboxKeyList.Clear();
    hv_BboxKeyList[0] = "bbox_col1";
    hv_BboxKeyList[1] = "bbox_row1";
    hv_BboxKeyList[2] = "bbox_col2";
    hv_BboxKeyList[3] = "bbox_row2";
    if (0 != (int(hv_InstanceType==HTuple("rectangle2"))))
    {
      hv_BboxKeyList.Clear();
      hv_BboxKeyList[0] = "bbox_row";
      hv_BboxKeyList[1] = "bbox_col";
      hv_BboxKeyList[2] = "bbox_length1";
      hv_BboxKeyList[3] = "bbox_length2";
      hv_BboxKeyList[4] = "bbox_phi";
    }
  }
  //Loop over all selected samples and create a DLSample dictionary
  //for each dictionary in the DLDataset samples.
  {
  HTuple end_val82 = (hv_SampleIndices.TupleLength())-1;
  HTuple step_val82 = 1;
  for (hv_ImageIndex=0; hv_ImageIndex.Continue(end_val82, step_val82); hv_ImageIndex += step_val82)
  {
    //
    //Create the DLSample dictionary
    CreateDict(&hv_DLSample);
    //
    //Set the image key.
    GetDictTuple(HTuple(hv_DLSamplesProc[hv_ImageIndex]), "image_id", &hv_ImageID);
    SetDictTuple(hv_DLSample, "image_id", hv_ImageID);
    //
    //Read image.
    //The relative file path of the image is specified in image_name.
    GetDictTuple(HTuple(hv_DLSamplesProc[hv_ImageIndex]), "image_file_name", &hv_ImageName);
    //
    if (0 != (int((hv_ImageDir.TupleStrlen())==0)))
    {
      hv_FileName = hv_ImageName;
    }
    else
    {
      hv_FileName = (hv_ImageDir+"/")+hv_ImageName;
    }
    try
    {
      ReadImage(&ho_ImageRaw, hv_FileName);
      //Insert image into dictionary.
      SetDictObject(ho_ImageRaw, hv_DLSample, "image");
    }
    // catch (Exception) 
    catch (HException &HDevExpDefaultException)
    {
      HDevExpDefaultException.ToHTuple(&hv_Exception);
      throw HException((((("Error for reading/setting image "+hv_FileName)+" with ID ")+hv_ImageID)+" : Error code ")+HTuple(hv_Exception[0]));
    }
    //
    //Read specific data.
    //
    if (0 != (int(hv_RestrictKeysDLSample!=HTuple("image_only"))))
    {
      //
      //Transfer anomaly detection relevant data.
      if (0 != (int((HTuple((HTuple("anomaly_detection").Append("off")).TupleFind(hv_RestrictKeysDLSample)).TupleMax())!=-1)))
      {
        //Check the existence of the label key.
        GetDictParam(HTuple(hv_DLSamplesProc[hv_ImageIndex]), "key_exists", "anomaly_label", 
            &hv_AnomalyLabelExists);
        if (0 != hv_AnomalyLabelExists)
        {
          //Get the image label.
          GetDictTuple(HTuple(hv_DLSamplesProc[hv_ImageIndex]), "anomaly_label", 
              &hv_AnomalyLabel);
          //Check the existence of the anomaly file name key. If not found it is just ignored.
          GetDictParam(HTuple(hv_DLSamplesProc[hv_ImageIndex]), "key_exists", "anomaly_file_name", 
              &hv_AnomalyFileNameExists);
          if (0 != hv_AnomalyFileNameExists)
          {
            //Get the ground truth anomaly directory.
            GetDictTuple(hv_DLDataset, "anomaly_dir", &hv_AnomalyDir);
            //Get the image file name.
            GetDictTuple(HTuple(hv_DLSamplesProc[hv_ImageIndex]), "anomaly_file_name", 
                &hv_AnomalyFileName);
            //Read the ground truth anomaly image.
            try
            {
              ReadImage(&ho_ImageAnomaly, (hv_AnomalyDir+"/")+hv_AnomalyFileName);
            }
            // catch (ExceptionImageAnomaly) 
            catch (HException &HDevExpDefaultException)
            {
              HDevExpDefaultException.ToHTuple(&hv_ExceptionImageAnomaly);
              //If the file is not an image, try to read the ground truth anomaly region.
              //Then, convert this region to a ground truth anomaly image.
              try
              {
                ReadRegion(&ho_RegionAnomaly, (hv_AnomalyDir+"/")+hv_AnomalyFileName);
              }
              // catch (ExceptionRegionAnomaly) 
              catch (HException &HDevExpDefaultException)
              {
                HDevExpDefaultException.ToHTuple(&hv_ExceptionRegionAnomaly);
                throw HException((("Error: Could not read the anomaly ground truth information of image_id "+hv_ImageID)+" : Error code ")+HTuple(hv_ExceptionImageAnomaly[0]));
              }
              GetImageSize(ho_ImageRaw, &hv_Width, &hv_Height);
              GenImageConst(&ho_ImageAnomaly, "byte", hv_Width, hv_Height);
              OverpaintRegion(ho_ImageAnomaly, ho_ImageAnomaly, 0, "fill");
              OverpaintRegion(ho_ImageAnomaly, ho_RegionAnomaly, 1, "fill");
            }
            //Insert anomaly image into DLSample dictionary.
            SetDictObject(ho_ImageAnomaly, hv_DLSample, "anomaly_ground_truth");
          }
          //
          //Insert anomaly label into DLSample dictionary.
          SetDictTuple(hv_DLSample, "anomaly_label", hv_AnomalyLabel);
          //Insert anomaly label id into DLSample dictionary.
          if (0 != (int(hv_AnomalyLabel==HTuple("nok"))))
          {
            SetDictTuple(hv_DLSample, "anomaly_label_id", 1);
          }
          else
          {
            SetDictTuple(hv_DLSample, "anomaly_label_id", 0);
          }
        }
        else if (0 != (HTuple(hv_AnomalyLabelExists.TupleNot()).TupleAnd(hv_IgnoreMissing.TupleNot())))
        {
          throw HException(("For image_id "+hv_ImageID)+" the key 'anomaly_label' is missing. Missing keys can be ignored using the GenParam ignore_missing_labels.");
        }
      }
      //
      //Transfer classification relevant data.
      if (0 != (int((HTuple((HTuple("classification").Append("off")).TupleFind(hv_RestrictKeysDLSample)).TupleMax())!=-1)))
      {
        //Check the existence of the required key.
        GetDictParam(HTuple(hv_DLSamplesProc[hv_ImageIndex]), "key_exists", "image_label_id", 
            &hv_ImageLabelIDExists);
        if (0 != hv_ImageLabelIDExists)
        {
          //Transfer the image label.
          GetDictTuple(HTuple(hv_DLSamplesProc[hv_ImageIndex]), "image_label_id", 
              &hv_ImageLabelID);
          SetDictTuple(hv_DLSample, "image_label_id", hv_ImageLabelID);
        }
        else if (0 != (HTuple(hv_ImageLabelIDExists.TupleNot()).TupleAnd(hv_IgnoreMissing.TupleNot())))
        {
          throw HException(("For image_id "+hv_ImageID)+" the key 'image_label_id' is missing. Missing keys can be ignored using the GenParam ignore_missing_labels.");
        }
      }
      //
      //Transfer detection relevant data.
      if (0 != (int((HTuple((HTuple("detection").Append("off")).TupleFind(hv_RestrictKeysDLSample)).TupleMax())!=-1)))
      {
        //Check the existence of the required key.
        GetDictParam(HTuple(hv_DLSamplesProc[hv_ImageIndex]), "key_exists", "bbox_label_id", 
            &hv_BboxExists);
        if (0 != hv_BboxExists)
        {
          //Transfer the bounding box labels.
          GetDictTuple(HTuple(hv_DLSamplesProc[hv_ImageIndex]), "bbox_label_id", 
              &hv_BboxLabels);
          SetDictTuple(hv_DLSample, "bbox_label_id", hv_BboxLabels);
          //Transfer the bounding box coordinates.
          GetDictParam(HTuple(hv_DLSamplesProc[hv_ImageIndex]), "key_exists", hv_BboxKeyList, 
              &hv_KeysExist);
          if (0 != (HTuple(int((hv_KeysExist.TupleSum())!=(hv_KeysExist.TupleLength()))).TupleAnd(hv_IgnoreMissing.TupleNot())))
          {
            hv_MissingKeyIndices = (hv_KeysExist.TupleEqualElem(0)).TupleFind(1);
            throw HException((("For image_id "+hv_ImageID)+HTuple(", an error has occurred when transferring the key "))+HTuple(hv_BboxKeyList[hv_MissingKeyIndices]));
          }
          {
          HTuple end_val186 = (hv_BboxKeyList.TupleLength())-1;
          HTuple step_val186 = 1;
          for (hv_IndexParam=0; hv_IndexParam.Continue(end_val186, step_val186); hv_IndexParam += step_val186)
          {
            GetDictTuple(HTuple(hv_DLSamplesProc[hv_ImageIndex]), HTuple(hv_BboxKeyList[hv_IndexParam]), 
                &hv_BboxCoord);
            SetDictTuple(hv_DLSample, HTuple(hv_BboxKeyList[hv_IndexParam]), hv_BboxCoord);
          }
          }
        }
        else if (0 != (hv_IgnoreMissing.TupleNot()))
        {
          throw HException(("For image_id "+hv_ImageID)+" there is no key bbox_label_id. Missing keys can be ignored using the GenParam ignore_missing_labels.");
        }
      }
      //
      //Transfer segmentation relevant data.
      if (0 != (int((HTuple((HTuple("segmentation").Append("off")).TupleFind(hv_RestrictKeysDLSample)).TupleMax())!=-1)))
      {
        //Check the existence of the required keys.
        GetDictParam(HTuple(hv_DLSamplesProc[hv_ImageIndex]), "key_exists", "segmentation_file_name", 
            &hv_SegKeyExists);
        if (0 != hv_SegKeyExists)
        {
          //Get the ground truth segmentation directory.
          GetDictTuple(hv_DLDataset, "segmentation_dir", &hv_SegmentationDir);
          //Get the image file name.
          GetDictTuple(HTuple(hv_DLSamplesProc[hv_ImageIndex]), "segmentation_file_name", 
              &hv_SegmentationName);
          //Read the ground truth segmentation image.
          try
          {
            ReadImage(&ho_ImageSegmentation, (hv_SegmentationDir+"/")+hv_SegmentationName);
          }
          // catch (ExceptionSegmentation) 
          catch (HException &HDevExpDefaultException)
          {
            HDevExpDefaultException.ToHTuple(&hv_ExceptionSegmentation);
            throw HException((("Error for reading segmentation file of image_id "+hv_ImageID)+" : Error code ")+HTuple(hv_ExceptionSegmentation[0]));
          }
          //Insert image into DLSample dictionary.
          SetDictObject(ho_ImageSegmentation, hv_DLSample, "segmentation_image");
        }
        else if (0 != (hv_IgnoreMissing.TupleNot()))
        {
          throw HException(("For image_id "+hv_ImageID)+" there is no key segmentation_file_name. Missing keys can be ignored using the GenParam ignore_missing_labels.");
        }
      }
    }
    //
    //Collect all data dictionaries of all processed indices.
    (*hv_DLSampleBatch)[hv_ImageIndex] = hv_DLSample;
  }
  }
  return;
}

// Chapter: Deep Learning / Model
// Short Description: Store the given images in a tuple of dictionaries DLSamples. 
void gen_dl_samples_from_images (HObject ho_Images, HTuple *hv_DLSampleBatch)
{

  // Local iconic variables
  HObject  ho_Image;

  // Local control variables
  HTuple  hv_NumImages, hv_ImageIndex, hv_DLSample;

  //
  //This procedure creates DLSampleBatch, a tuple
  //containing a dictionary DLSample
  //for every image given in Images.
  //
  //Initialize output tuple.
  CountObj(ho_Images, &hv_NumImages);
  (*hv_DLSampleBatch) = HTuple(hv_NumImages,-1);
  //
  //Loop through all given images.
  {
  HTuple end_val10 = hv_NumImages-1;
  HTuple step_val10 = 1;
  for (hv_ImageIndex=0; hv_ImageIndex.Continue(end_val10, step_val10); hv_ImageIndex += step_val10)
  {
    SelectObj(ho_Images, &ho_Image, hv_ImageIndex+1);
    //Create DLSample from image.
    CreateDict(&hv_DLSample);
    SetDictObject(ho_Image, hv_DLSample, "image");
    //
    //Collect the DLSamples.
    (*hv_DLSampleBatch)[hv_ImageIndex] = hv_DLSample;
  }
  }
  return;
}

// Chapter: Deep Learning / Semantic Segmentation
// Short Description: Generates weight images for the training dataset. 
void gen_dl_segmentation_weight_images (HTuple hv_DLDataset, HTuple hv_DLPreprocessParam, 
    HTuple hv_ClassWeights, HTuple hv_GenParam)
{

  // Local iconic variables
  HObject  ho_SegmentationImage, ho_WeightImage;
  HObject  ho_IgnoreRegion, ho_IgnoreRegionTmp, ho_ClassRegion;

  // Local control variables
  HTuple  hv_KeyExists, hv_ClassIDs, hv_OverwriteFiles;
  HTuple  hv_GenParamKeys, hv_GenParamIndex, hv_IgnoreClassIDs;
  HTuple  hv_Exception, hv_DLSamples, hv_SampleIndices, hv_InitNewImage;
  HTuple  hv_SampleIndex, hv_DLSample, hv_WeightImageExists;
  HTuple  hv_SampleImageID, hv_ImageWidth, hv_ImageHeight;
  HTuple  hv_IgnoreIndex, hv_IgnoreClassID, hv_ClassIndex;
  HTuple  hv_ClassID, hv_Weight;

  //
  //This procedure generates for each training sample in DLDataset a weight image,
  //that is used as input to the loss in a segmentation model.
  //The dictionary DLDataset needs a key 'dlsample_dir', assigning a directory
  //in which for every sample a dictionary DLSample has to exist.
  //The procedure reads for each training sample the dictionary DLSample,
  //generates a weight image according to the specified ClassWeights
  //and overwrites the DLSample with the updated sample including the weight image.
  //
  //Check input data.
  GetDictParam(hv_DLDataset, "key_exists", ((HTuple("dlsample_dir").Append("samples")).Append("class_ids")), 
      &hv_KeyExists);
  if (0 != (HTuple(hv_KeyExists[0]).TupleNot()))
  {
    throw HException("DLDataset needs a key-value pair for 'dlsample_dir'");
  }
  if (0 != (HTuple(hv_KeyExists[1]).TupleNot()))
  {
    throw HException("DLDataset needs a key-value pair for 'samples'");
  }
  if (0 != (HTuple(hv_KeyExists[2]).TupleNot()))
  {
    throw HException("DLDataset needs a key-value pair for 'class_ids'");
  }
  //
  GetDictTuple(hv_DLDataset, "class_ids", &hv_ClassIDs);
  if (0 != (int(hv_ClassWeights==HTuple())))
  {
    throw HException("ClassWeights is empty");
  }
  else if (0 != (HTuple(int((hv_ClassWeights.TupleLength())!=(hv_ClassIDs.TupleLength()))).TupleAnd(int((hv_ClassWeights.TupleLength())!=1))))
  {
    throw HException("ClassWeights must be either a single value or of the same length as the DLDataset ClassIDs.");
  }
  //
  if (0 != (int(((hv_ClassWeights.TupleLessElem(0)).TupleFind(1))>-1)))
  {
    throw HException("ClassWeights must be greater or equal zero.");
  }
  else if (0 != (HTuple(int((hv_ClassWeights.TupleLength())==1)).TupleAnd(int(hv_ClassWeights<=0))))
  {
    throw HException(HTuple("If only a single weight is given as ClassWeights, this must be greater than zero."));
  }
  //
  //Check the validity of the preprocessing parameters.
  check_dl_preprocess_param(hv_DLPreprocessParam);
  //
  //Set defaults.
  hv_OverwriteFiles = 0;
  //
  //Overwrite defaults specified in GenParam.
  if (0 != (int(hv_GenParam!=HTuple())))
  {
    GetDictParam(hv_GenParam, "keys", HTuple(), &hv_GenParamKeys);
    {
    HTuple end_val43 = (hv_GenParamKeys.TupleLength())-1;
    HTuple step_val43 = 1;
    for (hv_GenParamIndex=0; hv_GenParamIndex.Continue(end_val43, step_val43); hv_GenParamIndex += step_val43)
    {
      if (0 != (int(HTuple(hv_GenParamKeys[hv_GenParamIndex])==HTuple("overwrite_files"))))
      {
        //Set parameter for overwriting files.
        GetDictTuple(hv_GenParam, "overwrite_files", &hv_OverwriteFiles);
        if (0 != (HTuple(int(hv_OverwriteFiles!=0)).TupleAnd(int(hv_OverwriteFiles!=1))))
        {
          throw HException("'overwrite_files' must be either true or false");
        }
      }
      else
      {
        throw HException(("Unknown parameter: '"+HTuple(hv_GenParamKeys[hv_GenParamIndex]))+"'");
      }
    }
    }
  }
  //
  //Get the IDs of the classes to be ignored.
  try
  {
    GetDictTuple(hv_DLPreprocessParam, "ignore_class_ids", &hv_IgnoreClassIDs);
  }
  // catch (Exception) 
  catch (HException &HDevExpDefaultException)
  {
    HDevExpDefaultException.ToHTuple(&hv_Exception);
    hv_IgnoreClassIDs = HTuple();
  }
  //
  //Get the samples from the dataset.
  GetDictTuple(hv_DLDataset, "samples", &hv_DLSamples);
  //
  //Get the indices of the samples belonging to the defined split.
  find_dl_samples(hv_DLSamples, "split", "train", "or", &hv_SampleIndices);
  //
  //Get system info on init_new_image.
  GetSystem("init_new_image", &hv_InitNewImage);
  //
  //Loop over training samples.
  {
  HTuple end_val73 = (hv_SampleIndices.TupleLength())-1;
  HTuple step_val73 = 1;
  for (hv_SampleIndex=0; hv_SampleIndex.Continue(end_val73, step_val73); hv_SampleIndex += step_val73)
  {
    //
    //Read the DLSample.
    read_dl_samples(hv_DLDataset, HTuple(hv_SampleIndices[hv_SampleIndex]), &hv_DLSample);
    //
    //Check if there is already a weight image in the DLSample.
    GetDictParam(hv_DLSample, "key_exists", "weight_image", &hv_WeightImageExists);
    if (0 != (hv_WeightImageExists.TupleAnd(hv_OverwriteFiles.TupleNot())))
    {
      GetDictTuple(hv_DLSample, "image_id", &hv_SampleImageID);
      throw HException(("The DLSample with image_id "+hv_SampleImageID)+" already contains a weight image. Force overwriting using the parameter 'overwrite_files' to true.");
    }
    //
    //Get the segmentation image.
    GetDictObject(&ho_SegmentationImage, hv_DLSample, "segmentation_image");
    //
    //Generate the weight image.
    //
    //Initialize the weight image with 0.
    GetImageSize(ho_SegmentationImage, &hv_ImageWidth, &hv_ImageHeight);
    GenImageConst(&ho_WeightImage, "real", hv_ImageWidth, hv_ImageHeight);
    //Clear image.
    if (0 != (int(hv_InitNewImage==HTuple("false"))))
    {
      OverpaintRegion(ho_WeightImage, ho_WeightImage, 0, "fill");
    }
    //
    if (0 != (int((hv_ClassWeights.TupleLength())==1)))
    {
      //Constant class weight.
      OverpaintRegion(ho_WeightImage, ho_WeightImage, HTuple(hv_ClassWeights[0]), 
          "fill");
      //
      if (0 != (int((hv_IgnoreClassIDs.TupleLength())>0)))
      {
        //Set ignore region to 0.
        GenEmptyRegion(&ho_IgnoreRegion);
        {
        HTuple end_val105 = (hv_IgnoreClassIDs.TupleLength())-1;
        HTuple step_val105 = 1;
        for (hv_IgnoreIndex=0; hv_IgnoreIndex.Continue(end_val105, step_val105); hv_IgnoreIndex += step_val105)
        {
          hv_IgnoreClassID = HTuple(hv_IgnoreClassIDs[hv_IgnoreIndex]);
          Threshold(ho_SegmentationImage, &ho_IgnoreRegionTmp, hv_IgnoreClassID, 
              hv_IgnoreClassID);
          Union2(ho_IgnoreRegion, ho_IgnoreRegionTmp, &ho_IgnoreRegion);
        }
        }
        OverpaintRegion(ho_WeightImage, ho_IgnoreRegion, 0., "fill");
      }
    }
    else
    {
      //Loop over model ClassIDs.
      {
      HTuple end_val114 = (hv_ClassIDs.TupleLength())-1;
      HTuple step_val114 = 1;
      for (hv_ClassIndex=0; hv_ClassIndex.Continue(end_val114, step_val114); hv_ClassIndex += step_val114)
      {
        if (0 != (HTuple(int(hv_IgnoreClassIDs==HTuple())).TupleOr(int((hv_IgnoreClassIDs.TupleFind(HTuple(hv_ClassIDs[hv_ClassIndex])))==-1))))
        {
          //Set the pixel values of the weight image according to ClassWeights.
          hv_ClassID = HTuple(hv_ClassIDs[hv_ClassIndex]);
          hv_Weight = HTuple(hv_ClassWeights[hv_ClassIndex]);
          Threshold(ho_SegmentationImage, &ho_ClassRegion, hv_ClassID, hv_ClassID);
          OverpaintRegion(ho_WeightImage, ho_ClassRegion, hv_Weight, "fill");
        }
        else
        {
          //Ignore class has weight 0 which is already set.
        }
      }
      }
    }
    //
    //Add the weight image to DLSample.
    SetDictObject(ho_WeightImage, hv_DLSample, "weight_image");
    //
    //Write the updated DLSample.
    write_dl_samples(hv_DLDataset, HTuple(hv_SampleIndices[hv_SampleIndex]), hv_DLSample, 
        HTuple(), HTuple());
  }
  }
  //
  //On success we store the class weights for later reference in the DLDataset.
  SetDictTuple(hv_DLDataset, "class_weights", hv_ClassWeights);
  //
  return;
}

// Chapter: Deep Learning / Classification
// Short Description: Generate a tiled image for the classified DLSamples and add indications whether the predictions are true or not. 
void gen_tiled_classification_image_result (HObject *ho_TiledImageRow, HTuple hv_DLSamples, 
    HTuple hv_SpacingCol, HTuple hv_PredictionsCorrect, HTuple hv_ResClasses, HTuple *hv_TextImageRows, 
    HTuple *hv_TextImageColumns, HTuple *hv_TextImageWidth, HTuple *hv_TextImageHeight)
{

  // Local iconic variables
  HObject  ho_GTImagesRaw, ho_GTImage, ho_Channel;
  HObject  ho_RegionContourCheck, ho_Rectangle1, ho_Rectangle2;
  HObject  ho_RegionContourCross, ho_TopContour, ho_LeftContour;
  HObject  ho_RightContour, ho_BottomContour, ho_Frame, ho_TextImageOneChannel;
  HObject  ho_TextImageOneChannels, ho_TextImage, ho_SeparateImageOneChannel;
  HObject  ho_SeparateImageOneChannels, ho_SeparateImage, ho_GTImages;
  HObject  ho_GTImageChannelsScaled, ho_GTImageChannel, ho_GTImageChannelScaled;
  HObject  ho_GTImageR, ho_GTImageG, ho_GTImageB, ho_RegionContour;

  // Local control variables
  HTuple  hv_NumSamples, hv_NumRows, hv_NumColumns;
  HTuple  hv_Index, hv_NumChannels, hv_GrayMin, hv_GrayMax;
  HTuple  hv_IndexChannel, hv_GrayMinTmp, hv_GrayMaxTmp, hv__;
  HTuple  hv_ImageWidth, hv_ImageHeight, hv_Length1, hv_Length2;
  HTuple  hv_HomMat2DIdentity, hv_HomMat2DRotate, hv_HomMat2DCompose;
  HTuple  hv_FrameSize, hv_SeparateImageHeight, hv_SeparateImageWidth;
  HTuple  hv_ScaleMax, hv_ScaleMin, hv_Color, hv_Number, hv_TiledRows;
  HTuple  hv_TiledColumns, hv_TiledHeights, hv_TiledWidths;
  HTuple  hv_MinusOnes;
  HTupleVector  hvec_GrayMins(1), hvec_GrayMaxs(1);

  //
  //This procedure generates a tiled image for the classified DLSamples.
  //In the process it adds indications if the prediction was
  //correct (green frame and checkmark) or wrong (red frame and cross).
  //
  hv_NumSamples = hv_DLSamples.TupleLength();
  //
  //Get number of rows/columns of tiled image.
  if (0 != (int(hv_NumSamples>9)))
  {
    throw HException("More than 9 images cannot be shown in the training progress.");
  }
  if (0 != (HTuple(HTuple(int(hv_NumSamples==1)).TupleOr(int(hv_NumSamples==4))).TupleOr(int(hv_NumSamples==9))))
  {
    hv_NumRows = (hv_NumSamples.TupleSqrt()).TupleInt();
    hv_NumColumns = hv_NumRows;
  }
  else
  {
    hv_NumRows = hv_NumSamples.TupleMin2(3);
    hv_NumColumns = ((hv_NumSamples-1)/hv_NumRows)+1;
  }
  //
  //Get images and minimal/maximal gray values.
  GenEmptyObj(&ho_GTImagesRaw);
  {
  HTuple end_val21 = hv_NumSamples-1;
  HTuple step_val21 = 1;
  for (hv_Index=0; hv_Index.Continue(end_val21, step_val21); hv_Index += step_val21)
  {
    GetDictObject(&ho_GTImage, HTuple(hv_DLSamples[hv_Index]), "image");
    ConcatObj(ho_GTImagesRaw, ho_GTImage, &ho_GTImagesRaw);
    CountChannels(ho_GTImage, &hv_NumChannels);
    hv_GrayMin = HTuple();
    hv_GrayMax = HTuple();
    {
    HTuple end_val27 = hv_NumChannels-1;
    HTuple step_val27 = 1;
    for (hv_IndexChannel=0; hv_IndexChannel.Continue(end_val27, step_val27); hv_IndexChannel += step_val27)
    {
      AccessChannel(ho_GTImage, &ho_Channel, hv_IndexChannel+1);
      MinMaxGray(ho_Channel, ho_Channel, 0, &hv_GrayMinTmp, &hv_GrayMaxTmp, &hv__);
      hv_GrayMin = hv_GrayMin.TupleConcat(hv_GrayMinTmp);
      hv_GrayMax = hv_GrayMax.TupleConcat(hv_GrayMaxTmp);
    }
    }
    hvec_GrayMins[hv_Index] = HTupleVector(hv_GrayMin);
    hvec_GrayMaxs[hv_Index] = HTupleVector(hv_GrayMax);
  }
  }
  //
  //Create a checkmark, a cross, and a frame.
  //Note: It is assumed that all images have the same size.
  //Set parameters for displaying crosses/hooks at the bottom-right corner.
  GetImageSize(ho_GTImage, &hv_ImageWidth, &hv_ImageHeight);
  hv_Length1 = (hv_ImageWidth.TupleMin2(hv_ImageHeight))/5;
  hv_Length2 = (hv_ImageWidth.TupleMin2(hv_ImageHeight))/25;
  HomMat2dIdentity(&hv_HomMat2DIdentity);
  HomMat2dRotate(hv_HomMat2DIdentity, HTuple(45).TupleRad(), 0, 0, &hv_HomMat2DRotate);
  HomMat2dTranslate(hv_HomMat2DRotate, hv_ImageHeight-(0.75*hv_Length1), hv_ImageWidth-(0.75*hv_Length1), 
      &hv_HomMat2DCompose);
  //Generate checkmark.
  GenContourPolygonXld(&ho_RegionContourCheck, (((((-hv_Length2).TupleConcat(hv_Length2)).TupleConcat(hv_Length2)).TupleConcat((HTuple(0).Append(0)))).TupleConcat(-hv_Length2)).TupleConcat(-hv_Length2), 
      ((((((HTuple(0).Append(0)).TupleConcat(hv_Length1)).TupleConcat(hv_Length1)).TupleConcat(hv_Length2)).TupleConcat(hv_Length2)).TupleConcat(0))-(hv_Length1/2));
  AffineTransContourXld(ho_RegionContourCheck, &ho_RegionContourCheck, hv_HomMat2DCompose);
  //Generate cross.
  GenRectangle2ContourXld(&ho_Rectangle1, 0, 0, 0, hv_Length1/2, hv_Length2/2);
  GenRectangle2ContourXld(&ho_Rectangle2, 0, 0, 0, hv_Length2/2, hv_Length1/2);
  ConcatObj(ho_Rectangle1, ho_Rectangle2, &ho_RegionContourCross);
  AffineTransContourXld(ho_RegionContourCross, &ho_RegionContourCross, hv_HomMat2DCompose);
  //Generate the color frame.
  hv_FrameSize = 7;
  GenContourPolygonXld(&ho_TopContour, ((((HTuple(0).Append(0)).TupleConcat(hv_FrameSize)).TupleConcat(hv_FrameSize)).TupleConcat(0))-0.5, 
      (((HTuple(0).TupleConcat(hv_ImageWidth)).TupleConcat(hv_ImageWidth)).TupleConcat((HTuple(0).Append(0))))-0.5);
  GenContourPolygonXld(&ho_LeftContour, ((((HTuple(0).Append(0)).TupleConcat(hv_ImageHeight)).TupleConcat(hv_ImageHeight)).TupleConcat(0))-0.5, 
      (((HTuple(0).TupleConcat(hv_FrameSize)).TupleConcat(hv_FrameSize)).TupleConcat((HTuple(0).Append(0))))-0.5);
  GenContourPolygonXld(&ho_RightContour, (((HTuple(0).TupleConcat(hv_ImageHeight)).TupleConcat(hv_ImageHeight)).TupleConcat((HTuple(0).Append(0))))-0.5, 
      ((((hv_ImageWidth.TupleConcat(hv_ImageWidth)).TupleConcat(hv_ImageWidth-hv_FrameSize)).TupleConcat(hv_ImageWidth-hv_FrameSize)).TupleConcat(hv_ImageWidth))-0.5);
  GenContourPolygonXld(&ho_BottomContour, ((((hv_ImageHeight.TupleConcat(hv_ImageHeight-hv_FrameSize)).TupleConcat(hv_ImageHeight-hv_FrameSize)).TupleConcat(hv_ImageHeight)).TupleConcat(hv_ImageHeight))-0.5, 
      ((((HTuple(0).Append(0)).TupleConcat(hv_ImageWidth)).TupleConcat(hv_ImageWidth)).TupleConcat(0))-0.5);
  ConcatObj(ho_TopContour, ho_RightContour, &ho_Frame);
  ConcatObj(ho_Frame, ho_BottomContour, &ho_Frame);
  ConcatObj(ho_Frame, ho_LeftContour, &ho_Frame);
  //
  //Greate black image to print in text later.
  (*hv_TextImageHeight) = 40;
  (*hv_TextImageWidth) = hv_ImageWidth;
  GenImageConst(&ho_TextImageOneChannel, "real", (*hv_TextImageWidth), (*hv_TextImageHeight));
  ConcatObj(ho_TextImageOneChannel, ho_TextImageOneChannel, &ho_TextImageOneChannels
      );
  ConcatObj(ho_TextImageOneChannels, ho_TextImageOneChannel, &ho_TextImageOneChannels
      );
  ChannelsToImage(ho_TextImageOneChannels, &ho_TextImage);
  //
  //Greate black image to separate columns.
  hv_SeparateImageHeight = hv_NumRows*(hv_ImageHeight+(*hv_TextImageHeight));
  hv_SeparateImageWidth = 4;
  GenImageConst(&ho_SeparateImageOneChannel, "real", hv_SeparateImageWidth, hv_SeparateImageHeight);
  ConcatObj(ho_SeparateImageOneChannel, ho_SeparateImageOneChannel, &ho_SeparateImageOneChannels
      );
  ConcatObj(ho_SeparateImageOneChannels, ho_SeparateImageOneChannel, &ho_SeparateImageOneChannels
      );
  ChannelsToImage(ho_SeparateImageOneChannels, &ho_SeparateImage);
  //
  //Adapt images with frame and checkmark/cross.
  GenEmptyObj(&ho_GTImages);
  {
  HTuple end_val82 = hv_NumSamples-1;
  HTuple step_val82 = 1;
  for (hv_Index=0; hv_Index.Continue(end_val82, step_val82); hv_Index += step_val82)
  {
    //Scale iamges to [0,1] to have identical color ranges.
    SelectObj(ho_GTImagesRaw, &ho_GTImage, hv_Index+1);
    CountChannels(ho_GTImage, &hv_NumChannels);
    GenEmptyObj(&ho_GTImageChannelsScaled);
    {
    HTuple end_val87 = hv_NumChannels-1;
    HTuple step_val87 = 1;
    for (hv_IndexChannel=0; hv_IndexChannel.Continue(end_val87, step_val87); hv_IndexChannel += step_val87)
    {
      AccessChannel(ho_GTImage, &ho_GTImageChannel, hv_IndexChannel+1);
      hv_ScaleMax = HTuple(hvec_GrayMaxs[hv_Index].T()[hv_IndexChannel]);
      hv_ScaleMin = HTuple(hvec_GrayMins[hv_Index].T()[hv_IndexChannel]);
      if (0 != (int((hv_ScaleMax-hv_ScaleMin)>1e-5)))
      {
        ScaleImage(ho_GTImageChannel, &ho_GTImageChannelScaled, 1.0/(hv_ScaleMax-hv_ScaleMin), 
            (-hv_ScaleMin)/(hv_ScaleMax-hv_ScaleMin));
      }
      else
      {
        ScaleImage(ho_GTImageChannel, &ho_GTImageChannelScaled, 0, 0);
      }
      ConcatObj(ho_GTImageChannelsScaled, ho_GTImageChannelScaled, &ho_GTImageChannelsScaled
          );
    }
    }
    ChannelsToImage(ho_GTImageChannelsScaled, &ho_GTImage);
    //The frame has 3 channels, hence ensure that GTImage
    //has equal number of channels for display.
    if (0 != (int(hv_NumChannels!=3)))
    {
      //Just take the first channel and use this to generate
      //an image with 3 channels for visualization.
      AccessChannel(ho_GTImage, &ho_GTImageR, 1);
      CopyImage(ho_GTImageR, &ho_GTImageG);
      CopyImage(ho_GTImageR, &ho_GTImageB);
      Compose3(ho_GTImageR, ho_GTImageG, ho_GTImageB, &ho_GTImage);
    }
    //
    if (0 != (int(HTuple(hv_PredictionsCorrect[hv_Index])==1)))
    {
      //If the actual image is predicted correctly, plot a green frame and
      //a green checkmark.
      ho_RegionContour = ho_RegionContourCheck;
      hv_Color.Clear();
      hv_Color[0] = 0;
      hv_Color[1] = 1;
      hv_Color[2] = 0;
    }
    else
    {
      //Same as for the correct predictions, plot red frame and
      //a red cross for uncorrect predictions.
      ho_RegionContour = ho_RegionContourCross;
      hv_Color.Clear();
      hv_Color[0] = 1;
      hv_Color[1] = 0;
      hv_Color[2] = 0;
    }
    if (HDevWindowStack::IsOpen())
      SetDraw(HDevWindowStack::GetActive(),"margin");
    PaintXld(ho_Frame, ho_GTImage, &ho_GTImage, hv_Color);
    PaintXld(ho_RegionContour, ho_GTImage, &ho_GTImage, hv_Color);
    ConcatObj(ho_GTImages, ho_GTImage, &ho_GTImages);
    //Add TextImage.
    ConcatObj(ho_GTImages, ho_TextImage, &ho_GTImages);
  }
  }
  //
  //Tile the images. The maximum is 3x3=9 samples.
  //Set row/column positions of upper-left corners of images and text images.
  CountObj(ho_GTImages, &hv_Number);
  TupleGenConst(hv_Number, 0, &hv_TiledRows);
  TupleGenConst(hv_Number, 0, &hv_TiledColumns);
  TupleGenConst(hv_Number/2, 0, &(*hv_TextImageRows));
  TupleGenConst(hv_Number/2, 0, &(*hv_TextImageColumns));
  {
  HTuple end_val136 = (hv_Number/2)-1;
  HTuple step_val136 = 1;
  for (hv_Index=0; hv_Index.Continue(end_val136, step_val136); hv_Index += step_val136)
  {
    hv_TiledRows[2*hv_Index] = ((*hv_TextImageHeight)+hv_ImageHeight)*(hv_Index%hv_NumRows);
    (*hv_TextImageRows)[hv_Index] = (((*hv_TextImageHeight)+hv_ImageHeight)*(hv_Index%hv_NumRows))+hv_ImageHeight;
    hv_TiledRows[(2*hv_Index)+1] = HTuple((*hv_TextImageRows)[hv_Index]);
    hv_TiledColumns[2*hv_Index] = (hv_ImageWidth+hv_SeparateImageWidth)*(hv_Index/hv_NumRows);
    (*hv_TextImageColumns)[hv_Index] = HTuple(hv_TiledColumns[2*hv_Index]);
    hv_TiledColumns[(2*hv_Index)+1] = HTuple((*hv_TextImageColumns)[hv_Index]);
  }
  }
  //Add images and row/column positions of upper-left corners of SeparateImages.
  {
  HTuple end_val145 = hv_NumColumns-2;
  HTuple step_val145 = 1;
  for (hv_Index=0; hv_Index.Continue(end_val145, step_val145); hv_Index += step_val145)
  {
    ConcatObj(ho_GTImages, ho_SeparateImage, &ho_GTImages);
    hv_TiledRows[hv_TiledRows.TupleLength()] = 0;
    hv_TiledColumns[hv_TiledColumns.TupleLength()] = (hv_Index+1)*hv_ImageWidth;
  }
  }
  //
  hv_TiledHeights = hv_NumRows*((*hv_TextImageHeight)+hv_ImageHeight);
  hv_TiledWidths = (hv_NumColumns*hv_ImageWidth)+((hv_NumColumns-1)*hv_SeparateImageWidth);
  TupleGenConst(hv_TiledRows.TupleLength(), -1, &hv_MinusOnes);
  TileImagesOffset(ho_GTImages, &(*ho_TiledImageRow), hv_TiledRows, hv_TiledColumns, 
      hv_MinusOnes, hv_MinusOnes, hv_MinusOnes, hv_MinusOnes, hv_TiledWidths, hv_TiledHeights);
  //
  return;
}

// Chapter: Deep Learning / Semantic Segmentation
// Short Description: Generate a tiled image for segmentation DLSamples. 
void gen_tiled_segmentation_image (HObject *ho_TiledImageRow, HTuple hv_DLSamples, 
    HTuple hv_SpacingCol, HTuple hv_Width, HTuple hv_Height)
{

  // Local iconic variables
  HObject  ho_GTImages, ho_GTImage;

  // Local control variables
  HTuple  hv_Crop, hv_OffsetRow, hv_NumSamples;
  HTuple  hv_Index;

  //
  //This procedure tiles the segmentation images of given samples in a row.
  //
  TupleGenConst(hv_Width.TupleLength(), -1, &hv_Crop);
  TupleGenConst(hv_Width.TupleLength(), 0, &hv_OffsetRow);
  hv_NumSamples = hv_DLSamples.TupleLength();
  GenEmptyObj(&ho_GTImages);
  {
  HTuple end_val7 = hv_NumSamples-1;
  HTuple step_val7 = 1;
  for (hv_Index=0; hv_Index.Continue(end_val7, step_val7); hv_Index += step_val7)
  {
    GetDictObject(&ho_GTImage, HTuple(hv_DLSamples[hv_Index]), "segmentation_image");
    ConcatObj(ho_GTImages, ho_GTImage, &ho_GTImages);
  }
  }
  //
  TileImagesOffset(ho_GTImages, &(*ho_TiledImageRow), hv_OffsetRow, (HTuple(0).TupleConcat((hv_Width+hv_SpacingCol).TupleCumul())).TupleSelectRange(0,(hv_Width.TupleLength())-1), 
      hv_Crop, hv_Crop, hv_Crop, hv_Crop, HTuple(((hv_Width+hv_SpacingCol).TupleCumul())[(hv_Width.TupleLength())-1])-hv_SpacingCol, 
      hv_Height.TupleMax());
  return;
}

// Chapter: Deep Learning / Anomaly Detection
// Short Description: Get the ground truth anomaly label and label ID. 
void get_anomaly_ground_truth_label (HTuple hv_SampleKeys, HTuple hv_DLSample, HTuple *hv_AnomalyLabelGroundTruth, 
    HTuple *hv_AnomalyLabelIDGroundTruth)
{

  //
  //This procedure returns the anomaly ground truth label.
  //
  if (0 != (int((hv_SampleKeys.TupleFind("anomaly_label"))!=-1)))
  {
    GetDictTuple(hv_DLSample, "anomaly_label", &(*hv_AnomalyLabelGroundTruth));
  }
  else
  {
    throw HException("Ground truth class label cannot be found in DLSample.");
  }
  if (0 != (int((hv_SampleKeys.TupleFind("anomaly_label_id"))!=-1)))
  {
    GetDictTuple(hv_DLSample, "anomaly_label_id", &(*hv_AnomalyLabelIDGroundTruth));
  }
  else
  {
    throw HException("Ground truth class label id cannot be found in DLSample.");
  }
  //
  return;
}

// Chapter: Deep Learning / Anomaly Detection
// Short Description: Get the anomaly results out of DLResult and apply thresholds (if specified). 
void get_anomaly_result (HObject *ho_AnomalyImage, HObject *ho_AnomalyRegion, HTuple hv_DLResult, 
    HTuple hv_ResultKeys, HTuple hv_AnomalyClassThreshold, HTuple hv_AnomalyRegionThreshold, 
    HTuple *hv_AnomalyScore, HTuple *hv_AnomalyClassID, HTuple *hv_AnomalyClassThresholdDisplay, 
    HTuple *hv_AnomalyRegionThresholdDisplay)
{

  // Local iconic variables

  // Local control variables
  HTuple  hv_AnomalyImageExists;

  //
  //This procedure returns the inference results in DLResult which are
  //the anomaly image and the anomaly score. It also returns the
  //classification of the anomaly score and segmentation of anomalous
  //pixels in the anomaly image by applying the specified thresholds if
  //given. Otherwise the results from DLResult are used.
  //
  if (0 != (int((hv_ResultKeys.TupleFind("anomaly_image"))!=-1)))
  {
    GetDictParam(hv_DLResult, "key_exists", "anomaly_image", &hv_AnomalyImageExists);
    if (0 != hv_AnomalyImageExists)
    {
      GetDictObject(&(*ho_AnomalyImage), hv_DLResult, "anomaly_image");
    }
    else
    {
      throw HException("Result anomaly image could not be found in DLResult.");
    }
  }
  else
  {
    throw HException("Result anomaly image could not be found in DLResult.");
  }
  //
  if (0 != (int((hv_ResultKeys.TupleFind("anomaly_score"))!=-1)))
  {
    GetDictTuple(hv_DLResult, "anomaly_score", &(*hv_AnomalyScore));
  }
  else
  {
    throw HException("Result anomaly score could not be found in DLResult.");
  }
  //
  (*hv_AnomalyRegionThresholdDisplay) = -1;
  if (0 != (int(hv_AnomalyRegionThreshold!=-1)))
  {
    //Apply threshold for segmentation result.
    if (0 != (int((hv_AnomalyRegionThreshold.TupleLength())!=1)))
    {
      throw HException("Selected 'anomaly_region_threshold' must be specified by exactly one value.");
    }
    if (0 != (HTuple(int(hv_AnomalyRegionThreshold>1)).TupleOr(int(hv_AnomalyRegionThreshold<0))))
    {
      throw HException("Selected 'anomaly_region_threshold' out of range. It must be between 0 and 1.");
    }
    Threshold((*ho_AnomalyImage), &(*ho_AnomalyRegion), hv_AnomalyRegionThreshold, 
        1);
    (*hv_AnomalyRegionThresholdDisplay) = hv_AnomalyRegionThreshold;
  }
  else
  {
    //If no threshold is given, use the threshold and resulting anomaly region out of DLResult.
    if (0 != (int((hv_ResultKeys.TupleFind("anomaly_region"))!=-1)))
    {
      GetDictObject(&(*ho_AnomalyRegion), hv_DLResult, "anomaly_region");
      GetDictTuple(hv_DLResult, "anomaly_segmentation_threshold", &(*hv_AnomalyRegionThresholdDisplay));
    }
    else
    {
      GenEmptyObj(&(*ho_AnomalyRegion));
    }
  }
  //
  (*hv_AnomalyClassThresholdDisplay) = -1;
  (*hv_AnomalyClassID) = -1;
  if (0 != (int(hv_AnomalyClassThreshold!=-1)))
  {
    //Apply threshold for classification result.
    if (0 != (int((hv_AnomalyClassThreshold.TupleLength())!=1)))
    {
      throw HException("Selected 'anomaly_classification_threshold' must be specified by exactly one value.");
    }
    if (0 != (int(hv_AnomalyClassThreshold<0.0)))
    {
      throw HException("Selected 'anomaly_classification_threshold' cannot be negative.");
    }
    if (0 != (int((*hv_AnomalyScore)<hv_AnomalyClassThreshold)))
    {
      (*hv_AnomalyClassID) = 0;
    }
    else
    {
      (*hv_AnomalyClassID) = 1;
    }
    (*hv_AnomalyClassThresholdDisplay) = hv_AnomalyClassThreshold;
  }
  else
  {
    //If no threshold is given, use the threshold and resulting class id out of DLResult.
    if (0 != (int((hv_ResultKeys.TupleFind("anomaly_class_id"))!=-1)))
    {
      GetDictTuple(hv_DLResult, "anomaly_class_id", &(*hv_AnomalyClassID));
      GetDictTuple(hv_DLResult, "anomaly_classification_threshold", &(*hv_AnomalyClassThresholdDisplay));
    }
    else
    {
      (*hv_AnomalyClassID) = -1;
    }
  }
  //
  return;
}

// Chapter: Graphics / Window
// Short Description: Get the next child window that can be used for visualization. 
void get_child_window (HTuple hv_HeightImage, HTuple hv_Font, HTuple hv_FontSize, 
    HTuple hv_Text, HTuple hv_PrevWindowCoordinates, HTuple hv_WindowHandleDict, 
    HTuple hv_WindowHandleKey, HTuple *hv_WindowImageRatio, HTuple *hv_PrevWindowCoordinatesOut)
{

  // Local iconic variables

  // Local control variables
  HTuple  hv_OpenNewWindow, hv_WindowHandles, hv_ParentWindowHandle;
  HTuple  hv_ChildWindowHandle, hv_Exception, hv_MetaInfo;
  HTuple  hv_WindowRow, hv_WindowColumn, hv_WindowWidth, hv_WindowHeight;

  //
  //This procedure returns the next child window that
  //is used for visualization. If ReuseWindows is true
  //and WindowHandleList is suitable, the window handles
  //that are passed over are used. Else, this procedure
  //opens a new window, either next to the last ones, or
  //in a new row.
  //
  //First, check if the requested window is already available.
  hv_OpenNewWindow = 0;
  try
  {
    GetDictTuple(hv_WindowHandleDict, hv_WindowHandleKey, &hv_WindowHandles);
    hv_ParentWindowHandle = ((const HTuple&)hv_WindowHandles)[0];
    hv_ChildWindowHandle = ((const HTuple&)hv_WindowHandles)[1];
    //Check if window handle is valid.
    try
    {
      FlushBuffer(hv_ChildWindowHandle);
    }
    // catch (Exception) 
    catch (HException &HDevExpDefaultException)
    {
      HDevExpDefaultException.ToHTuple(&hv_Exception);
      //Since there is something wrong with the current window, create a new one.
      hv_OpenNewWindow = 1;
    }
  }
  // catch (Exception) 
  catch (HException &HDevExpDefaultException)
  {
    HDevExpDefaultException.ToHTuple(&hv_Exception);
    hv_OpenNewWindow = 1;
  }
  //
  //Get next child window.
  if (0 != (hv_OpenNewWindow.TupleNot()))
  {
    //
    //If possible, reuse existing window handles.
    HDevWindowStack::SetActive(hv_ChildWindowHandle);
    if (HDevWindowStack::IsOpen())
      ClearWindow(HDevWindowStack::GetActive());
    set_display_font(hv_ChildWindowHandle, hv_FontSize, hv_Font, "true", "false");
    //
    GetDictTuple(hv_WindowHandleDict, "meta_information", &hv_MetaInfo);
    //
    //Get previous window coordinates.
    GetWindowExtents(hv_ParentWindowHandle, &hv_WindowRow, &hv_WindowColumn, &hv_WindowWidth, 
        &hv_WindowHeight);
    (*hv_WindowImageRatio) = hv_WindowHeight/(hv_HeightImage*1.0);
    //
    try
    {
      //
      //Get WindowImageRatio from parent window.
      GetDictTuple(hv_MetaInfo, hv_WindowHandleKey+"_window_image_ratio_height", 
          &(*hv_WindowImageRatio));
      //
      //Get previous window coordinates.
      GetDictTuple(hv_MetaInfo, hv_WindowHandleKey+"_child_window_coordinates", &(*hv_PrevWindowCoordinatesOut));
    }
    // catch (Exception) 
    catch (HException &HDevExpDefaultException)
    {
      HDevExpDefaultException.ToHTuple(&hv_Exception);
      //
      //Set WindowImageRatio from parent window.
      GetWindowExtents(hv_ParentWindowHandle, &hv_WindowRow, &hv_WindowColumn, &hv_WindowWidth, 
          &hv_WindowHeight);
      (*hv_WindowImageRatio) = hv_WindowHeight/(hv_HeightImage*1.0);
      //
      //Set previous window coordinates.
      (*hv_PrevWindowCoordinatesOut)[0] = hv_WindowRow;
      (*hv_PrevWindowCoordinatesOut)[1] = hv_WindowColumn;
      (*hv_PrevWindowCoordinatesOut)[2] = hv_WindowWidth;
      (*hv_PrevWindowCoordinatesOut)[3] = hv_WindowHeight;
    }
  }
  else
  {
    //
    //Open a new child window.
    open_child_window(hv_ParentWindowHandle, hv_Font, hv_FontSize, hv_Text, hv_PrevWindowCoordinates, 
        hv_WindowHandleDict, hv_WindowHandleKey, &hv_ChildWindowHandle, &(*hv_PrevWindowCoordinatesOut));
    SetWindowParam(hv_ChildWindowHandle, "flush", "false");
    SetDictTuple(hv_WindowHandleDict, hv_WindowHandleKey, hv_ParentWindowHandle.TupleConcat(hv_ChildWindowHandle));
  }
  //
  return;
}

// Chapter: Deep Learning / Classification
// Short Description: Get the ground truth classification label id. 
void get_classification_ground_truth (HTuple hv_SampleKeys, HTuple hv_DLSample, HTuple *hv_ClassificationLabelIDGroundTruth)
{

  //
  //This procedure returns the classification ground truth label ID.
  //
  if (0 != (int((hv_SampleKeys.TupleFind("image_label_id"))!=-1)))
  {
    GetDictTuple(hv_DLSample, "image_label_id", &(*hv_ClassificationLabelIDGroundTruth));
  }
  else
  {
    throw HException("Ground truth class label cannot be found in DLSample.");
  }
  //
  return;
}

// Chapter: Deep Learning / Classification
// Short Description: Get the predicted classification class ID. 
void get_classification_result (HTuple hv_ResultKeys, HTuple hv_DLResult, HTuple *hv_ClassificationClassID)
{

  // Local iconic variables

  //
  //This procedure returns the predicted classification class ID.
  //
  if (0 != (int((hv_ResultKeys.TupleFind("classification_class_ids"))!=-1)))
  {
    GetDictTuple(hv_DLResult, "classification_class_ids", &(*hv_ClassificationClassID));
    if (0 != (int(((*hv_ClassificationClassID).TupleLength())>0)))
    {
      (*hv_ClassificationClassID) = ((const HTuple&)(*hv_ClassificationClassID))[0];
    }
  }
  else
  {
    throw HException("Key entry 'classification_class_ids' could not be found in DLResult.");
  }
  //
  return;
}

// Chapter: Deep Learning / Semantic Segmentation
// Short Description: Get the confidences of the segmentation result. 
void get_confidence_image (HObject *ho_ImageConfidence, HTuple hv_ResultKeys, HTuple hv_DLResult)
{

  //
  //This procedure returns confidences of the segmentation result.
  //
  if (0 != (int((hv_ResultKeys.TupleFind("segmentation_confidence"))!=-1)))
  {
    GetDictObject(&(*ho_ImageConfidence), hv_DLResult, "segmentation_confidence");
  }
  else if (0 != (int((hv_ResultKeys.TupleFind("segmentation_confidences"))!=-1)))
  {
    GetDictObject(&(*ho_ImageConfidence), hv_DLResult, "segmentation_confidences");
  }
  else
  {
    throw HException("Confidence image could not be found in DLSample.");
  }
  return;
}

// Chapter: Deep Learning / Model
// Short Description: Generate NumColors distinct colors 
void get_distinct_colors (HTuple hv_NumColors, HTuple hv_Random, HTuple hv_StartColor, 
    HTuple hv_EndColor, HTuple *hv_Colors)
{

  // Local iconic variables
  HObject  ho_HLSImageH, ho_HLSImageL, ho_HLSImageS;
  HObject  ho_ImageR, ho_ImageG, ho_ImageB;

  // Local control variables
  HTuple  hv_IsString, hv_Hue, hv_Lightness, hv_Saturation;
  HTuple  hv_Rows, hv_Columns, hv_Red, hv_Green, hv_Blue;

  //
  //We get distinct color-values first in HLS color-space.
  //Assumes hue [0, EndColor), lightness [0, 1), saturation [0, 1).
  //
  //Parameter checks.
  //NumColors.
  if (0 != (int(hv_NumColors<1)))
  {
    throw HException("NumColors should be at least 1");
  }
  if (0 != ((hv_NumColors.TupleIsInt()).TupleNot()))
  {
    throw HException("NumColors should be of type int");
  }
  if (0 != (int((hv_NumColors.TupleLength())!=1)))
  {
    throw HException("NumColors should have length 1");
  }
  //Random.
  if (0 != (HTuple(int(hv_Random!=0)).TupleAnd(int(hv_Random!=1))))
  {
    TupleIsString(hv_Random, &hv_IsString);
    if (0 != hv_IsString)
    {
      hv_Random = HTuple(int(hv_Random==HTuple("true"))).TupleOr("false");
    }
    else
    {
      throw HException("Random should be either true or false");
    }
  }
  //StartColor.
  if (0 != (int((hv_StartColor.TupleLength())!=1)))
  {
    throw HException("StartColor should have length 1");
  }
  if (0 != (HTuple(int(hv_StartColor<0)).TupleOr(int(hv_StartColor>255))))
  {
    throw HException(HTuple("StartColor should be in the range [0, 255]"));
  }
  if (0 != ((hv_StartColor.TupleIsInt()).TupleNot()))
  {
    throw HException("StartColor should be of type int");
  }
  //EndColor.
  if (0 != (int((hv_EndColor.TupleLength())!=1)))
  {
    throw HException("EndColor should have length 1");
  }
  if (0 != (HTuple(int(hv_EndColor<0)).TupleOr(int(hv_EndColor>255))))
  {
    throw HException(HTuple("EndColor should be in the range [0, 255]"));
  }
  if (0 != ((hv_EndColor.TupleIsInt()).TupleNot()))
  {
    throw HException("EndColor should be of type int");
  }
  //
  //Color generation.
  if (0 != (int(hv_StartColor>hv_EndColor)))
  {
    hv_EndColor += 255;
  }
  if (0 != (int(hv_NumColors!=1)))
  {
    hv_Hue = (hv_StartColor+((((hv_EndColor-hv_StartColor)*(HTuple::TupleGenSequence(0,hv_NumColors-1,1).TupleReal()))/((hv_NumColors-1).TupleReal())).TupleInt()))%255;
  }
  else
  {
    hv_Hue = (hv_StartColor.TupleConcat(hv_EndColor)).TupleMean();
  }
  if (0 != hv_Random)
  {
    hv_Hue = ((const HTuple&)hv_Hue)[HTuple::TupleRand(hv_NumColors).TupleSortIndex()];
    hv_Lightness = (((5.0+HTuple::TupleRand(hv_NumColors))*255.0)/10.0).TupleInt();
    hv_Saturation = (((9.0+HTuple::TupleRand(hv_NumColors))*255.0)/10.0).TupleInt();
  }
  else
  {
    hv_Lightness = (HTuple(hv_NumColors,0.55)*255.0).TupleInt();
    hv_Saturation = (HTuple(hv_NumColors,0.95)*255.0).TupleInt();
  }
  //
  //Write colors to a 3-channel image in order to transform easier.
  GenImageConst(&ho_HLSImageH, "byte", 1, hv_NumColors);
  GenImageConst(&ho_HLSImageL, "byte", 1, hv_NumColors);
  GenImageConst(&ho_HLSImageS, "byte", 1, hv_NumColors);
  GetRegionPoints(ho_HLSImageH, &hv_Rows, &hv_Columns);
  SetGrayval(ho_HLSImageH, hv_Rows, hv_Columns, hv_Hue);
  SetGrayval(ho_HLSImageL, hv_Rows, hv_Columns, hv_Lightness);
  SetGrayval(ho_HLSImageS, hv_Rows, hv_Columns, hv_Saturation);
  //
  //Convert from HLS to RGB.
  TransToRgb(ho_HLSImageH, ho_HLSImageL, ho_HLSImageS, &ho_ImageR, &ho_ImageG, &ho_ImageB, 
      "hls");
  //
  //Get RGB-values and transform to Hex.
  GetGrayval(ho_ImageR, hv_Rows, hv_Columns, &hv_Red);
  GetGrayval(ho_ImageG, hv_Rows, hv_Columns, &hv_Green);
  GetGrayval(ho_ImageB, hv_Rows, hv_Columns, &hv_Blue);
  (*hv_Colors) = (("#"+(hv_Red.TupleString("02x")))+(hv_Green.TupleString("02x")))+(hv_Blue.TupleString("02x"));
  return;
  //
}

// Chapter: Deep Learning / Model
// Short Description: Generate NumColors distinct colors 
void get_distinct_colors_dev_display_pie_chart (HTuple hv_NumColors, HTuple hv_Random, 
    HTuple hv_StartColor, HTuple hv_EndColor, HTuple *hv_Colors)
{

  // Local iconic variables
  HObject  ho_HLSImageH, ho_HLSImageL, ho_HLSImageS;
  HObject  ho_ImageR, ho_ImageG, ho_ImageB;

  // Local control variables
  HTuple  hv_IsString, hv_Hue, hv_Lightness, hv_Saturation;
  HTuple  hv_Rows, hv_Columns, hv_Red, hv_Green, hv_Blue;

  //
  //We get distinct color-values first in HLS color-space.
  //Assumes hue [0, EndColor), lightness [0, 1), saturation [0, 1).
  //
  //Parameter checks.
  //NumColors.
  if (0 != (int(hv_NumColors<1)))
  {
    throw HException("NumColors should be at least 1");
  }
  if (0 != ((hv_NumColors.TupleIsInt()).TupleNot()))
  {
    throw HException("NumColors should be of type int");
  }
  if (0 != (int((hv_NumColors.TupleLength())!=1)))
  {
    throw HException("NumColors should have length 1");
  }
  //Random.
  if (0 != (HTuple(int(hv_Random!=0)).TupleAnd(int(hv_Random!=1))))
  {
    TupleIsString(hv_Random, &hv_IsString);
    if (0 != hv_IsString)
    {
      hv_Random = HTuple(int(hv_Random==HTuple("true"))).TupleOr("false");
    }
    else
    {
      throw HException("Random should be either true or false");
    }
  }
  //StartColor.
  if (0 != (int((hv_StartColor.TupleLength())!=1)))
  {
    throw HException("StartColor should have length 1");
  }
  if (0 != (HTuple(int(hv_StartColor<0)).TupleOr(int(hv_StartColor>255))))
  {
    throw HException(HTuple("StartColor should be in the range [0, 255]"));
  }
  if (0 != ((hv_StartColor.TupleIsInt()).TupleNot()))
  {
    throw HException("StartColor should be of type int");
  }
  //EndColor.
  if (0 != (int((hv_EndColor.TupleLength())!=1)))
  {
    throw HException("EndColor should have length 1");
  }
  if (0 != (HTuple(int(hv_EndColor<0)).TupleOr(int(hv_EndColor>255))))
  {
    throw HException(HTuple("EndColor should be in the range [0, 255]"));
  }
  if (0 != ((hv_EndColor.TupleIsInt()).TupleNot()))
  {
    throw HException("EndColor should be of type int");
  }
  //
  //Color generation.
  if (0 != (int(hv_StartColor>hv_EndColor)))
  {
    hv_EndColor += 255;
  }
  if (0 != (int(hv_NumColors!=1)))
  {
    hv_Hue = (hv_StartColor+((((hv_EndColor-hv_StartColor)*(HTuple::TupleGenSequence(0,hv_NumColors-1,1).TupleReal()))/((hv_NumColors-1).TupleReal())).TupleInt()))%255;
  }
  else
  {
    hv_Hue = (hv_StartColor.TupleConcat(hv_EndColor)).TupleMean();
  }
  if (0 != hv_Random)
  {
    hv_Hue = ((const HTuple&)hv_Hue)[HTuple::TupleRand(hv_NumColors).TupleSortIndex()];
    hv_Lightness = (((5.0+HTuple::TupleRand(hv_NumColors))*255.0)/10.0).TupleInt();
    hv_Saturation = (((9.0+HTuple::TupleRand(hv_NumColors))*255.0)/10.0).TupleInt();
  }
  else
  {
    hv_Lightness = (HTuple(hv_NumColors,0.55)*255.0).TupleInt();
    hv_Saturation = (HTuple(hv_NumColors,0.95)*255.0).TupleInt();
  }
  //
  //Write colors to a 3-channel image in order to transform easier.
  GenImageConst(&ho_HLSImageH, "byte", 1, hv_NumColors);
  GenImageConst(&ho_HLSImageL, "byte", 1, hv_NumColors);
  GenImageConst(&ho_HLSImageS, "byte", 1, hv_NumColors);
  GetRegionPoints(ho_HLSImageH, &hv_Rows, &hv_Columns);
  SetGrayval(ho_HLSImageH, hv_Rows, hv_Columns, hv_Hue);
  SetGrayval(ho_HLSImageL, hv_Rows, hv_Columns, hv_Lightness);
  SetGrayval(ho_HLSImageS, hv_Rows, hv_Columns, hv_Saturation);
  //
  //Convert from HLS to RGB.
  TransToRgb(ho_HLSImageH, ho_HLSImageL, ho_HLSImageS, &ho_ImageR, &ho_ImageG, &ho_ImageB, 
      "hls");
  //
  //Get RGB-values and transform to Hex.
  GetGrayval(ho_ImageR, hv_Rows, hv_Columns, &hv_Red);
  GetGrayval(ho_ImageG, hv_Rows, hv_Columns, &hv_Green);
  GetGrayval(ho_ImageB, hv_Rows, hv_Columns, &hv_Blue);
  (*hv_Colors) = (("#"+(hv_Red.TupleString("02x")))+(hv_Green.TupleString("02x")))+(hv_Blue.TupleString("02x"));
  return;
  //
}

// Chapter: Deep Learning / Model
// Short Description: Generates certain colors for different ClassNames 
void get_dl_class_colors (HTuple hv_ClassNames, HTuple *hv_Colors)
{

  // Local iconic variables

  // Local control variables
  HTuple  hv_NumColors, hv_ColorsRainbow, hv_ClassNamesGood;
  HTuple  hv_IndexFind, hv_GoodIdx, hv_CurrentColor, hv_GreenIdx;

  //
  //This procedure returns for each class a certain color.
  //
  //Define distinct colors for the classes.
  hv_NumColors = hv_ClassNames.TupleLength();
  //Get distinct colors without randomness makes neighboring colors look very similar.
  //We use a workaround to get deterministic colors where subsequent colors are distinguishable.
  get_distinct_colors(hv_NumColors, 0, 0, 200, &hv_ColorsRainbow);
  TupleInverse(hv_ColorsRainbow, &hv_ColorsRainbow);
  make_neighboring_colors_distinguishable(hv_ColorsRainbow, &(*hv_Colors));
  //If a class 'OK','ok', 'good' or 'GOOD' is present set this class to green.
  //Only the first occurrence found is set to a green shade.
  hv_ClassNamesGood.Clear();
  hv_ClassNamesGood[0] = "good";
  hv_ClassNamesGood[1] = "GOOD";
  hv_ClassNamesGood[2] = "ok";
  hv_ClassNamesGood[3] = "OK";
  {
  HTuple end_val13 = (hv_ClassNamesGood.TupleLength())-1;
  HTuple step_val13 = 1;
  for (hv_IndexFind=0; hv_IndexFind.Continue(end_val13, step_val13); hv_IndexFind += step_val13)
  {
    hv_GoodIdx = hv_ClassNames.TupleFindFirst(HTuple(hv_ClassNamesGood[hv_IndexFind]));
    if (0 != (HTuple(int(hv_GoodIdx!=-1)).TupleAnd(int((hv_ClassNames.TupleLength())<=8))))
    {
      //If number of classes is <= 8, swap color with a green color.
      hv_CurrentColor = HTuple((*hv_Colors)[hv_GoodIdx]);
      hv_GreenIdx = HTuple((hv_ClassNames.TupleLength())/2.0).TupleFloor();
      //Set to pure green.
      (*hv_Colors)[hv_GoodIdx] = "#00ff00";
      //Write original color to a green entry.
      (*hv_Colors)[hv_GreenIdx] = hv_CurrentColor;
      break;
    }
    else if (0 != (HTuple(int(hv_GoodIdx!=-1)).TupleAnd(int((hv_ClassNames.TupleLength())>8))))
    {
      //If number of classes is larger than 8, set the respective color to green.
      (*hv_Colors)[hv_GoodIdx] = "#00ff00";
      break;
    }
  }
  }
  return;
}

// Chapter: Deep Learning / Model
// Short Description: Get a parameter value from GenParamValue with the name RequestedGenParamName. 
void get_genparam_single_value (HTuple hv_GenParamName, HTuple hv_GenParamValue, 
    HTuple hv_RequestedGenParamName, HTuple *hv_FoundGenParamValue)
{

  // Local iconic variables

  // Local control variables
  HTuple  hv_Indices;

  //
  //Get a parameter value from GenParamValue with the name RequestedGenParamName,
  //which is allowed to appear only once in GenParamName.
  //
  //Set a default in case no value is provided.
  (*hv_FoundGenParamValue) = HTuple();
  //Set user provided values, if provided.
  hv_Indices = hv_GenParamName.TupleFind(hv_RequestedGenParamName);
  if (0 != (HTuple(int((hv_Indices.TupleLength())==1)).TupleAnd(int(hv_Indices!=-1))))
  {
    (*hv_FoundGenParamValue) = HTuple(hv_GenParamValue[hv_Indices]);
  }
  else if (0 != (int((hv_Indices.TupleLength())>1)))
  {
    //Throw an error if more than one value was provided for RequestedGenParamName.
    throw HException(("Only a single parameter dictionary or none is allowed for '"+hv_RequestedGenParamName)+"'.");
  }
  return;
}

// Chapter: Deep Learning / Model
// Short Description: Get the image of a sample. 
void get_image (HObject *ho_Image, HTuple hv_SampleKeys, HTuple hv_DLSample)
{

  //
  //This procedure returns the image of a sample.
  //
  if (0 != (int((hv_SampleKeys.TupleFind("image"))!=-1)))
  {
    GetDictObject(&(*ho_Image), hv_DLSample, "image");
  }
  else
  {
    throw HException("Image could not be found in DLSample.");
  }
  return;
}

// Chapter: Graphics / Window
// Short Description: Get the next window that can be used for visualization. 
void get_next_window (HTuple hv_Font, HTuple hv_FontSize, HTuple hv_ShowBottomDesc, 
    HTuple hv_WidthImage, HTuple hv_HeightImage, HTuple hv_MapColorBarWidth, HTuple hv_ScaleWindows, 
    HTuple hv_ThresholdWidth, HTuple hv_PrevWindowCoordinates, HTuple hv_WindowHandleDict, 
    HTuple hv_WindowHandleKey, HTuple *hv_CurrentWindowHandle, HTuple *hv_WindowImageRatioHeight, 
    HTuple *hv_PrevWindowCoordinatesOut)
{

  // Local iconic variables

  // Local control variables
  HTuple  hv_OpenNewWindow, hv_WindowHandles, hv_Value;
  HTuple  hv_Exception, hv_Ascent, hv_Descent, hv__, hv_MarginBottom;
  HTuple  hv_WindowImageRatioWidth, hv_SetPartRow2, hv_SetPartColumn2;
  HTuple  hv_MetaInfo;

  //
  //This procedure returns the next window that
  //is used for visualization. If ReuseWindows is true
  //and WindowHandleList is suitable, the window handles
  //that are passed over are used. Else, this procedure
  //opens a new window, either next to the last ones, or
  //in a new row.
  //
  //First, check if the requested window is already available.
  hv_OpenNewWindow = 0;
  try
  {
    GetDictTuple(hv_WindowHandleDict, hv_WindowHandleKey, &hv_WindowHandles);
    (*hv_CurrentWindowHandle) = ((const HTuple&)hv_WindowHandles)[0];
    //Check if window handle is valid.
    try
    {
      GetWindowParam((*hv_CurrentWindowHandle), "flush", &hv_Value);
    }
    // catch (Exception) 
    catch (HException &HDevExpDefaultException)
    {
      HDevExpDefaultException.ToHTuple(&hv_Exception);
      //If there is something wrong with the current window, create a new one.
      hv_OpenNewWindow = 1;
      RemoveDictKey(hv_WindowHandleDict, hv_WindowHandleKey);
    }
  }
  // catch (Exception) 
  catch (HException &HDevExpDefaultException)
  {
    HDevExpDefaultException.ToHTuple(&hv_Exception);
    hv_OpenNewWindow = 1;
  }
  //
  //Get next window.
  if (0 != (hv_OpenNewWindow.TupleNot()))
  {
    //
    //If possible, reuse existing window handles.
    HDevWindowStack::SetActive((*hv_CurrentWindowHandle));
    if (HDevWindowStack::IsOpen())
      ClearWindow(HDevWindowStack::GetActive());
    set_display_font((*hv_CurrentWindowHandle), hv_FontSize, hv_Font, "true", "false");
    //
    //Calculate MarginBottom.
    if (0 != hv_ShowBottomDesc)
    {
      GetStringExtents((*hv_CurrentWindowHandle), "test_string", &hv_Ascent, &hv_Descent, 
          &hv__, &hv__);
      hv_MarginBottom = ((2*12)+hv_Ascent)+hv_Descent;
    }
    else
    {
      hv_MarginBottom = 0;
    }
    //
    //Get and set meta information for current window.
    update_window_meta_information((*hv_CurrentWindowHandle), hv_WidthImage, hv_HeightImage, 
        0, 0, hv_MapColorBarWidth, hv_MarginBottom, &(*hv_WindowImageRatioHeight), 
        &hv_WindowImageRatioWidth, &hv_SetPartRow2, &hv_SetPartColumn2, &(*hv_PrevWindowCoordinatesOut));
    //
    //Update meta information.
    GetDictTuple(hv_WindowHandleDict, "meta_information", &hv_MetaInfo);
    SetDictTuple(hv_MetaInfo, hv_WindowHandleKey+"_window_image_ratio_height", (*hv_WindowImageRatioHeight));
    SetDictTuple(hv_MetaInfo, hv_WindowHandleKey+"_window_image_ratio_width", hv_WindowImageRatioWidth);
    SetDictTuple(hv_MetaInfo, hv_WindowHandleKey+"_set_part_row2", hv_SetPartRow2);
    SetDictTuple(hv_MetaInfo, hv_WindowHandleKey+"_set_part_column2", hv_SetPartColumn2);
    SetDictTuple(hv_MetaInfo, hv_WindowHandleKey+"_margin_bottom", hv_MarginBottom);
    SetDictTuple(hv_MetaInfo, hv_WindowHandleKey+"_map_color_bar_with", hv_MapColorBarWidth);
    SetDictTuple(hv_MetaInfo, hv_WindowHandleKey+"_window_coordinates", (*hv_PrevWindowCoordinatesOut));
  }
  else
  {
    //
    //Open a new window.
    open_next_window(hv_Font, hv_FontSize, hv_ShowBottomDesc, hv_WidthImage, hv_HeightImage, 
        hv_MapColorBarWidth, hv_ScaleWindows, hv_ThresholdWidth, hv_PrevWindowCoordinates, 
        hv_WindowHandleDict, hv_WindowHandleKey, &(*hv_CurrentWindowHandle), &(*hv_WindowImageRatioHeight), 
        &(*hv_PrevWindowCoordinatesOut));
    SetWindowParam((*hv_CurrentWindowHandle), "flush", "false");
  }
  //
  return;
}

// Chapter: Deep Learning / Semantic Segmentation
// Short Description: Get the ground truth segmentation image. 
void get_segmentation_image_ground_truth (HObject *ho_SegmentationImagGroundTruth, 
    HTuple hv_SampleKeys, HTuple hv_DLSample)
{

  //
  //This procedure returns the ground truth segmentation image.
  //
  if (0 != (int((hv_SampleKeys.TupleFind("segmentation_image"))!=-1)))
  {
    GetDictObject(&(*ho_SegmentationImagGroundTruth), hv_DLSample, "segmentation_image");
  }
  else
  {
    throw HException("Ground truth segmentation image could not be found in DLSample.");
  }
  return;
}

// Chapter: Deep Learning / Semantic Segmentation
// Short Description: Get the predicted segmentation result image. 
void get_segmentation_image_result (HObject *ho_SegmentationImageResult, HTuple hv_ResultKeys, 
    HTuple hv_DLResult)
{

  //
  //This procedure returns the predicted segmentation result image.
  //
  if (0 != (int((hv_ResultKeys.TupleFind("segmentation_image"))!=-1)))
  {
    GetDictObject(&(*ho_SegmentationImageResult), hv_DLResult, "segmentation_image");
  }
  else
  {
    throw HException("Result segmentation data could not be found in DLSample.");
  }
  return;
}

// Chapter: Deep Learning / Semantic Segmentation
// Short Description: Get the weight image of a sample. 
void get_weight_image (HObject *ho_ImageWeight, HTuple hv_SampleKeys, HTuple hv_DLSample)
{

  //
  //This procedure returns the segmentation weight image of a sample.
  //
  if (0 != (int((hv_SampleKeys.TupleFind("weight_image"))!=-1)))
  {
    GetDictObject(&(*ho_ImageWeight), hv_DLSample, "weight_image");
  }
  else
  {
    throw HException("Weight image could not be found in DLSample.");
  }
  return;
}

// Chapter: File / Misc
void images_exist (HTuple hv_ImageList)
{

  // Local iconic variables

  // Local control variables
  HTuple  hv_OS, hv_Separator, hv_HalconImages;
  HTuple  hv_Index, hv_ImageExists, hv_DirIndex;

  //
  //Get HalconImages directories.
  GetSystem("operating_system", &hv_OS);
  if (0 != (int((hv_OS.TupleSubstr(0,2))==HTuple("Win"))))
  {
    hv_Separator = ";";
  }
  else
  {
    hv_Separator = ":";
  }
  GetSystem("image_dir", &hv_HalconImages);
  TupleSplit(hv_HalconImages, hv_Separator, &hv_HalconImages);
  //
  //Loop over images and segmentations.
  {
  HTuple end_val12 = (hv_ImageList.TupleLength())-1;
  HTuple step_val12 = 1;
  for (hv_Index=0; hv_Index.Continue(end_val12, step_val12); hv_Index += step_val12)
  {
    FileExists(HTuple(hv_ImageList[hv_Index]), &hv_ImageExists);
    if (0 != (hv_ImageExists.TupleNot()))
    {
      //Check in HalconImages directories.
      {
      HTuple end_val16 = (hv_HalconImages.TupleLength())-1;
      HTuple step_val16 = 1;
      for (hv_DirIndex=0; hv_DirIndex.Continue(end_val16, step_val16); hv_DirIndex += step_val16)
      {
        FileExists((HTuple(hv_HalconImages[hv_DirIndex])+"/")+HTuple(hv_ImageList[hv_Index]), 
            &hv_ImageExists);
        if (0 != hv_ImageExists)
        {
          break;
        }
      }
      }
      if (0 != (hv_ImageExists.TupleNot()))
      {
        throw HException(("Image "+HTuple(hv_ImageList[hv_Index]))+" does not exist");
      }
    }
  }
  }
  return;
}

// Chapter: Deep Learning / Evaluation
// Short Description: Initialize the dictionary RunningMeasures for the evaluation. 
void init_running_evaluation_measures (HTuple hv_EvalParams, HTuple *hv_RunningMeasures)
{

  // Local iconic variables

  // Local control variables
  HTuple  hv_Valid, hv_Exception, hv_NumClasses;
  HTuple  hv_EvaluationType, hv_Measures, hv_ClassIDs, hv_InstanceType;
  HTuple  hv_EvalOrientation, hv_AllocationBlockLength, hv_IoUThreshs;
  HTuple  hv_MaxNumDetections, hv_AreaRanges, hv_AreaNames;
  HTuple  hv_MinAreas, hv_MaxAreas, hv_DetailedEvaluation;
  HTuple  hv_MaxNumIdx, hv_MaxNum, hv_CurrentRunningMeasure;
  HTuple  hv_AreaIdx, hv_AreaRunningMeasure, hv_I, hv_IoURunningMeasure;
  HTuple  hv_ClsIdx, hv_ClassRunningMeasures, hv_Confidence;
  HTuple  hv_IgnoreClassIDs, hv_CalcConfMatrix, hv_MatrixSize;
  HTuple  hv_PixelConfusionMatrix, hv_MaxId, hv_ClsIDToClsIdx;
  HTuple  hv_TP, hv_FP, hv_FN;

  //
  //This procedure initializes the dictionary RunningMeasures for evaluation.
  //It uses the evaluation parameters to initialize the running measures accordingly.
  //
  //The structure of RunningMeasures depends on the entry 'evaluate_instances' in the dictionary EvalParams.
  //
  //The dictionary RunningMeasures can be updated based on the per-batch/per-image evaluation results.
  //
  CreateDict(&(*hv_RunningMeasures));
  //Check that the necessary evaluation parameters exist.
  validate_evaluation_param(hv_EvalParams, &hv_Valid, &hv_Exception);
  if (0 != (hv_Valid.TupleNot()))
  {
    throw HException(HTuple("Invalid EvalParams, ")+hv_Exception);
  }
  //
  //Get general evaluation parameters.
  GetDictTuple(hv_EvalParams, "num_classes", &hv_NumClasses);
  GetDictTuple(hv_EvalParams, "evaluation_type", &hv_EvaluationType);
  GetDictTuple(hv_EvalParams, "measures", &hv_Measures);
  GetDictTuple(hv_EvalParams, "class_ids", &hv_ClassIDs);
  //
  if (0 != (int(hv_EvaluationType==HTuple("anomaly_detection"))))
  {
    //RunningMeasures contains:
    //- image_ids:          IDs of the images.
    //- anomaly_label_ids:  Class IDs of ground truth labels.
    //- anomaly_scores:     Predicted image level anomaly scores.
    SetDictTuple((*hv_RunningMeasures), "image_ids", HTuple());
    SetDictTuple((*hv_RunningMeasures), "anomaly_label_ids", HTuple());
    SetDictTuple((*hv_RunningMeasures), "anomaly_scores", HTuple());
  }
  else if (0 != (int(hv_EvaluationType==HTuple("classification"))))
  {
    //RunningMeasures contains:
    //- image_ids:          IDs of the images.
    //- image_label_ids:    Class IDs of ground truth labels.
    //- top1_predictions:   Class IDs of the top predicted class.
    //- topk_predictions:   Class IDs of top-K predicted classes.
    SetDictTuple((*hv_RunningMeasures), "image_ids", HTuple());
    SetDictTuple((*hv_RunningMeasures), "image_label_ids", HTuple());
    SetDictTuple((*hv_RunningMeasures), "top1_predictions", HTuple());
    SetDictTuple((*hv_RunningMeasures), "topk_predictions", HTuple());
  }
  else if (0 != (int(hv_EvaluationType==HTuple("detection"))))
  {
    //RunningMeasures contains:
    //For each maximal number of regions (MaxNumDetections):
    // - For each area range (AreaRanges):
    //   -- confidence:     Confidence (score) of each result.
    //   -- num_gt:         Total number of ground truth instances per class.
    //   -- num_pred:       Total number of predictions per class.
    //   -- num_gt_ignore:  Number of ignored ground truth instances per class.
    //   -- for each IoU-threshold:
    //      --- For each class:
    //          ---- is_tp:                  TP/FP assignment of result.
    //          ---- ignore:                 Ignore/Not-Ignore assignment of result.
    //          ---- abs_orientation_diff (for instance_type 'rectangle2' with measure SoAP):
    //                                       Absolute orientation difference of the result.
    //
    //Check if the orientation difference is to be evaluated.
    GetDictTuple(hv_EvalParams, "instance_type", &hv_InstanceType);
    hv_EvalOrientation = 0;
    if (0 != (HTuple(int(hv_InstanceType==HTuple("rectangle2"))).TupleAnd(HTuple(int((hv_Measures.TupleFind("soap"))!=-1)).TupleOr(int((hv_Measures.TupleFind("all"))!=-1)))))
    {
      hv_EvalOrientation = 1;
    }
    //
    //Calculating the measures confidence, is_tp, ignore, and abs_orientation_diff,
    //arrays are allocated with -1 in blocks of AllocationBlockLength
    //(thus, if a block is filled, the next block is allocated).
    //Otherwise the arrays have to be concatenated which is rather slow.
    //The actual length of the array is garnered in num_pred.
    GetDictTuple(hv_EvalParams, "allocation_block_length", &hv_AllocationBlockLength);
    GetDictTuple(hv_EvalParams, "iou_threshold", &hv_IoUThreshs);
    GetDictTuple(hv_EvalParams, "max_num_detections", &hv_MaxNumDetections);
    GetDictTuple(hv_EvalParams, "area_ranges", &hv_AreaRanges);
    //AreaRanges is a dictionary containing 'name', 'min_area', 'max_area'.
    GetDictTuple(hv_AreaRanges, "name", &hv_AreaNames);
    GetDictTuple(hv_AreaRanges, "min", &hv_MinAreas);
    GetDictTuple(hv_AreaRanges, "max", &hv_MaxAreas);
    //Check if a detailed evaluation will be performed.
    GetDictTuple(hv_EvalParams, "detailed_evaluation", &hv_DetailedEvaluation);
    //Set a result dictionary for each maximal number of detections and IoU-threshold.
    {
    HTuple end_val77 = (hv_MaxNumDetections.TupleLength())-1;
    HTuple step_val77 = 1;
    for (hv_MaxNumIdx=0; hv_MaxNumIdx.Continue(end_val77, step_val77); hv_MaxNumIdx += step_val77)
    {
      hv_MaxNum = HTuple(hv_MaxNumDetections[hv_MaxNumIdx]);
      CreateDict(&hv_CurrentRunningMeasure);
      {
      HTuple end_val80 = (hv_AreaNames.TupleLength())-1;
      HTuple step_val80 = 1;
      for (hv_AreaIdx=0; hv_AreaIdx.Continue(end_val80, step_val80); hv_AreaIdx += step_val80)
      {
        CreateDict(&hv_AreaRunningMeasure);
        {
        HTuple end_val82 = (hv_IoUThreshs.TupleLength())-1;
        HTuple step_val82 = 1;
        for (hv_I=0; hv_I.Continue(end_val82, step_val82); hv_I += step_val82)
        {
          CreateDict(&hv_IoURunningMeasure);
          {
          HTuple end_val84 = hv_NumClasses-1;
          HTuple step_val84 = 1;
          for (hv_ClsIdx=0; hv_ClsIdx.Continue(end_val84, step_val84); hv_ClsIdx += step_val84)
          {
            CreateDict(&hv_ClassRunningMeasures);
            SetDictTuple(hv_ClassRunningMeasures, "is_tp", HTuple(hv_AllocationBlockLength,-1));
            SetDictTuple(hv_ClassRunningMeasures, "ignore", HTuple(hv_AllocationBlockLength,-1));
            if (0 != hv_EvalOrientation)
            {
              SetDictTuple(hv_ClassRunningMeasures, "abs_orientation_diff", HTuple(hv_AllocationBlockLength,-1));
            }
            if (0 != hv_DetailedEvaluation)
            {
              SetDictTuple(hv_ClassRunningMeasures, "is_fp_class", HTuple(hv_AllocationBlockLength,-1));
              SetDictTuple(hv_ClassRunningMeasures, "is_fp_background", HTuple(hv_AllocationBlockLength,-1));
              SetDictTuple(hv_ClassRunningMeasures, "is_fp_localization", HTuple(hv_AllocationBlockLength,-1));
              SetDictTuple(hv_ClassRunningMeasures, "is_fp_duplicate", HTuple(hv_AllocationBlockLength,-1));
              SetDictTuple(hv_ClassRunningMeasures, "is_fp_multiple", HTuple(hv_AllocationBlockLength,-1));
              if (0 != hv_EvalOrientation)
              {
                SetDictTuple(hv_ClassRunningMeasures, "abs_orientation_diff_class", 
                    HTuple(hv_AllocationBlockLength,-1));
                SetDictTuple(hv_ClassRunningMeasures, "abs_orientation_diff_localization", 
                    HTuple(hv_AllocationBlockLength,-1));
                SetDictTuple(hv_ClassRunningMeasures, "abs_orientation_diff_duplicate", 
                    HTuple(hv_AllocationBlockLength,-1));
                SetDictTuple(hv_ClassRunningMeasures, "abs_orientation_diff_multiple", 
                    HTuple(hv_AllocationBlockLength,-1));
              }
              SetDictTuple(hv_IoURunningMeasure, "image_ids_with_false_negatives", 
                  HTuple(hv_AllocationBlockLength,-1));
              SetDictTuple(hv_IoURunningMeasure, "image_ids_with_false_positives", 
                  HTuple(hv_AllocationBlockLength,-1));
              SetDictTuple(hv_IoURunningMeasure, "num_image_ids_with_false_negatives", 
                  0);
              SetDictTuple(hv_IoURunningMeasure, "num_image_ids_with_false_positives", 
                  0);
            }
            SetDictTuple(hv_IoURunningMeasure, "class_"+HTuple(hv_ClassIDs[hv_ClsIdx]), 
                hv_ClassRunningMeasures);
          }
          }
          SetDictTuple(hv_AreaRunningMeasure, "iou_"+((""+HTuple(hv_IoUThreshs[hv_I])).TupleRegexpReplace("\\.","")), 
              hv_IoURunningMeasure);
        }
        }
        CreateDict(&hv_Confidence);
        {
        HTuple end_val113 = hv_NumClasses-1;
        HTuple step_val113 = 1;
        for (hv_ClsIdx=0; hv_ClsIdx.Continue(end_val113, step_val113); hv_ClsIdx += step_val113)
        {
          SetDictTuple(hv_Confidence, "class_"+HTuple(hv_ClassIDs[hv_ClsIdx]), HTuple(hv_AllocationBlockLength,-1.0));
        }
        }
        SetDictTuple(hv_AreaRunningMeasure, "confidence", hv_Confidence);
        SetDictTuple(hv_AreaRunningMeasure, "num_gt", HTuple(hv_NumClasses,0));
        SetDictTuple(hv_AreaRunningMeasure, "num_pred", HTuple(hv_NumClasses,0));
        SetDictTuple(hv_AreaRunningMeasure, "num_gt_ignore", HTuple(hv_NumClasses,0));
        SetDictTuple(hv_CurrentRunningMeasure, "area_"+HTuple(hv_AreaNames[hv_AreaIdx]), 
            hv_AreaRunningMeasure);
      }
      }
      if (0 != (int(hv_MaxNum==-1)))
      {
        hv_MaxNum = "all";
      }
      SetDictTuple((*hv_RunningMeasures), "max_num_detections_"+hv_MaxNum, hv_CurrentRunningMeasure);
    }
    }
  }
  else if (0 != (int(hv_EvaluationType==HTuple("segmentation"))))
  {
    //RunningMeasures contains:
    //if confusion matrix in Measures (slower but more information).
    // - confusion matrix per pixel.
    //else:
    // - TP/FP/FN (pixel numbers per class).
    //
    //Incorporate ignore class IDs.
    GetDictTuple(hv_EvalParams, "ignore_class_ids", &hv_IgnoreClassIDs);
    //
    //Check if we need to compute/update the confusion matrix.
    hv_CalcConfMatrix = HTuple(int((hv_Measures.TupleFind("pixel_confusion_matrix"))>-1)).TupleOr(int((hv_Measures.TupleFind("all"))>-1));
    if (0 != hv_CalcConfMatrix)
    {
      //Define the size of the confusion matrix.
      hv_MatrixSize = hv_NumClasses+(int((hv_IgnoreClassIDs.TupleLength())>0));
      CreateMatrix(hv_MatrixSize, hv_MatrixSize, 0, &hv_PixelConfusionMatrix);
      SetDictTuple((*hv_RunningMeasures), "pixel_confusion_matrix", hv_PixelConfusionMatrix);
      //
      //If the class IDs are not running indices from 0 to NumClasses we
      //define a mapping from class IDs to class indices.
      if (0 != (HTuple(int(hv_ClassIDs!=HTuple::TupleGenSequence(0,(hv_ClassIDs.TupleLength())-1,1))).TupleOr(int((hv_IgnoreClassIDs.TupleLength())>0))))
      {
        //Get the max ID that can occur.
        hv_MaxId = (hv_ClassIDs.TupleMax())+(int((hv_IgnoreClassIDs.TupleLength())>0));
        //Define the basic mapping.
        TupleGenConst(hv_MaxId+1, -1, &hv_ClsIDToClsIdx);
        hv_ClsIDToClsIdx[hv_ClassIDs] = HTuple::TupleGenSequence(0,(hv_ClassIDs.TupleLength())-1,1);
        //Map ignore IDs to the next higher one.
        hv_ClsIDToClsIdx[hv_IgnoreClassIDs] = (hv_ClsIDToClsIdx.TupleMax())+1;
        //Set the mapping to the evaluation parameters.
        SetDictTuple(hv_EvalParams, "class_id_mapping", hv_ClsIDToClsIdx);
      }
    }
    else
    {
      hv_TP = HTuple(hv_NumClasses,0);
      hv_FP = HTuple(hv_NumClasses,0);
      hv_FN = HTuple(hv_NumClasses,0);
      SetDictTuple((*hv_RunningMeasures), "tp", hv_TP);
      SetDictTuple((*hv_RunningMeasures), "fp", hv_FP);
      SetDictTuple((*hv_RunningMeasures), "fn", hv_FN);
    }
  }
  //
  return;
}

// Chapter: Deep Learning / Model
// Short Description: Initialize change strategies data. 
void init_train_dl_model_change_strategies (HTuple hv_TrainParam, HTuple *hv_ChangeStrategyData)
{

  // Local iconic variables

  // Local control variables
  HTuple  hv_ChangeStrategies, hv_Enabled, hv_Index;
  HTuple  hv_ChangeStrategy, hv_ModelParam, hv_Epochs, hv_Values;
  HTuple  hv_Initial, hv_Indices, hv_ScaleThresholdExists;

  //
  //Initialize a dictionary with the change strategies data.
  CreateDict(&(*hv_ChangeStrategyData));
  GetDictTuple(hv_TrainParam, "change_strategies", &hv_ChangeStrategies);
  hv_Enabled = int((hv_ChangeStrategies.TupleLength())>0);
  SetDictTuple((*hv_ChangeStrategyData), "enabled", hv_Enabled);
  if (0 != (hv_Enabled.TupleNot()))
  {
    return;
  }
  //
  //Sort all epochs in all change strategies.
  {
  HTuple end_val11 = (hv_ChangeStrategies.TupleLength())-1;
  HTuple step_val11 = 1;
  for (hv_Index=0; hv_Index.Continue(end_val11, step_val11); hv_Index += step_val11)
  {
    hv_ChangeStrategy = HTuple(hv_ChangeStrategies[hv_Index]);
    GetDictTuple(hv_ChangeStrategy, "model_param", &hv_ModelParam);
    GetDictTuple(hv_ChangeStrategy, "epochs", &hv_Epochs);
    GetDictTuple(hv_ChangeStrategy, "values", &hv_Values);
    GetDictTuple(hv_ChangeStrategy, "initial_value", &hv_Initial);
    //Check that the length are equal.
    if (0 != (int((hv_Epochs.TupleLength())!=(hv_Values.TupleLength()))))
    {
      throw HException("ChangeStrategy parameter error: 'epochs' and 'values' need to have same length.");
    }
    //We need sorted arrays for faster access.
    TupleSortIndex(hv_Epochs, &hv_Indices);
    SetDictTuple(hv_ChangeStrategy, "epochs", HTuple(hv_Epochs[hv_Indices]));
    SetDictTuple(hv_ChangeStrategy, "values", HTuple(hv_Values[hv_Indices]));
    //
    //For the learning rate, there can be an additional parameter
    //indicating if the momentum should be scaled as well.
    if (0 != (int(hv_ModelParam==HTuple("learning_rate"))))
    {
      GetDictParam(hv_ChangeStrategy, "key_exists", "scale_momentum_threshold", &hv_ScaleThresholdExists);
      if (0 != (hv_ScaleThresholdExists.TupleNot()))
      {
        //If not given, the threshold is set to an empty tuple such that no scaling is performed.
        SetDictTuple(hv_ChangeStrategy, "scale_momentum_threshold", HTuple());
      }
    }
  }
  }
  SetDictTuple((*hv_ChangeStrategyData), "strategies", hv_ChangeStrategies);
  return;
}

// Chapter: Deep Learning / Model
// Short Description: Initializes the dictionary setting the serialization strategies. 
void init_train_dl_model_serialization_strategies (HTuple hv_TrainParam, HTuple *hv_SerializationData)
{

  // Local iconic variables

  // Local control variables
  HTuple  hv_SerializationStrategies, hv_RawData;
  HTuple  hv_Types, hv_SerializeFinal, hv_Index, hv_Strategy;
  HTuple  hv_Type, hv_Data, hv_Epochs, hv_NumEpochs;

  //
  //This procedure initializes the dictionary setting the serialization strategies.
  //
  //Initialize each serialization strategy.
  GetDictTuple(hv_TrainParam, "serialization_strategies", &hv_SerializationStrategies);
  CreateDict(&(*hv_SerializationData));
  SetDictTuple((*hv_SerializationData), "strategies", hv_SerializationStrategies);
  hv_RawData = HTuple();
  hv_Types = HTuple();
  hv_SerializeFinal = 0;
  {
  HTuple end_val10 = (hv_SerializationStrategies.TupleLength())-1;
  HTuple step_val10 = 1;
  for (hv_Index=0; hv_Index.Continue(end_val10, step_val10); hv_Index += step_val10)
  {
    hv_Strategy = HTuple(hv_SerializationStrategies[hv_Index]);
    GetDictTuple(hv_Strategy, "type", &hv_Type);
    CreateDict(&hv_Data);
    if (0 != (int(hv_Type==HTuple("best"))))
    {
      SetDictTuple(hv_Data, "best_value", -1);
    }
    else if (0 != (int(hv_Type==HTuple("epochs"))))
    {
      GetDictTuple(hv_Strategy, "epochs", &hv_Epochs);
      //Store sorted values in order to search faster during updates.
      TupleSort(hv_Epochs, &hv_Epochs);
      SetDictTuple(hv_Data, "epochs", hv_Epochs);
      SetDictTuple(hv_Data, "last_epoch_index", -1);
    }
    else if (0 != (int(hv_Type==HTuple("current"))))
    {
      GetDictTuple(hv_TrainParam, "num_epochs", &hv_NumEpochs);
      SetDictTuple(hv_Data, "epochs", HTuple::TupleGenSequence(1,hv_NumEpochs,1));
      SetDictTuple(hv_Data, "last_epoch_index", -1);
    }
    else if (0 != (int(hv_Type==HTuple("final"))))
    {
      SetDictTuple(hv_Data, "serialize_final", 1);
    }
    else
    {
      throw HException(("Unknown serialization strategy type: '"+hv_Type)+"'");
    }
    hv_Types = hv_Types.TupleConcat(hv_Type);
    hv_RawData = hv_RawData.TupleConcat(hv_Data);
  }
  }
  SetDictTuple((*hv_SerializationData), "raw_data", hv_RawData);
  SetDictTuple((*hv_SerializationData), "types", hv_Types);
  return;
}

// Chapter: Deep Learning / Semantic Segmentation
// Short Description: Generate image list and corresponding annotation file list. 
void list_image_and_annotation_files (HTuple hv_Type, HTuple hv_ImageDir, HTuple hv_AnnotationDir, 
    HTuple hv_ImageListIn, HTuple hv_GenParam, HTuple *hv_ImageListOut, HTuple *hv_LabelListOut, 
    HTuple *hv_AnnotationListOut)
{

  // Local iconic variables

  // Local control variables
  HTuple  hv_ImageSubDirs, hv_AnnotationSubDirs;
  HTuple  hv_FileNameImageOnly, hv_FileNameAnnoOnly, hv_IgnoreUnmatched;
  HTuple  hv_Keys, hv_KeyIndex, hv_ImageDirs, hv_ImageBaseNames;
  HTuple  hv_ImageExtensions, hv_ImageDirectories, hv_CommonBasePathImage;
  HTuple  hv_ImageDirectoriesRel, hv_ImageMatches, hv_MatchIndices;
  HTuple  hv_ImageBaseNamesToMatch, hv_GroundTruthLabels;
  HTuple  hv_AnnotationDirs, hv_AnomalyExtensions, hv_AnnoBaseNames;
  HTuple  hv_AnnoExtensions, hv_AnnoDirectories, hv_CommonBasePathAnno;
  HTuple  hv_AnnoDirectoriesRel, hv_AnnoMatches, hv_AnnoBaseNamesToMatch;
  HTuple  hv_AnnoMatchIndices, hv_ImageMatchIndices, hv_ImageIndex;
  HTuple  hv_AnnoMatchIndex, hv_Match, hv_PathMatch, hv_PathMatchIndex;
  HTuple  hv_ImagePath, hv_ImageIndices, hv_AnnotationList;

  //
  //This procedure generates lists of images and corresponding annotation files,
  //whereby the first one can also be handed over through ImageListIn.
  //The lists are generated by listing the files in the given directory:
  // - ImageDir specifies the base directory for the images,
  // - AnnotationDir specifies the base directory for the annotation files.
  //
  //The file names of images and annotation files must give a unique match
  //in the sense that for each image exactly one annotation is matched.
  //To obtain a match, the following steps are done (in the given order):
  //1) Exact name match.
  //2) Image name is a subpart of a single annotation file name.
  //3) Image name is a subpart of more than one annotation file names and
  //   only one of these matches has the same subfolder structure.
  //With the parameters specified in GenParam, the lists can be
  //limited to specific file names.
  //
  //Set defaults.
  hv_ImageSubDirs = HTuple();
  hv_AnnotationSubDirs = HTuple();
  hv_FileNameImageOnly = "";
  hv_FileNameAnnoOnly = "";
  hv_IgnoreUnmatched = 0;
  //
  //Overwrite defaults.
  if (0 != (int(hv_GenParam!=HTuple())))
  {
    GetDictParam(hv_GenParam, "keys", HTuple(), &hv_Keys);
    {
    HTuple end_val27 = (hv_Keys.TupleLength())-1;
    HTuple step_val27 = 1;
    for (hv_KeyIndex=0; hv_KeyIndex.Continue(end_val27, step_val27); hv_KeyIndex += step_val27)
    {
      if (0 != (int(HTuple(hv_Keys[hv_KeyIndex])==HTuple("image_sub_dirs"))))
      {
        GetDictTuple(hv_GenParam, "image_sub_dirs", &hv_ImageSubDirs);
      }
      else if (0 != (int(HTuple(hv_Keys[hv_KeyIndex])==HTuple("annotation_sub_dirs"))))
      {
        GetDictTuple(hv_GenParam, "annotation_sub_dirs", &hv_AnnotationSubDirs);
      }
      else if (0 != (int(HTuple(hv_Keys[hv_KeyIndex])==HTuple("file_name_image_only"))))
      {
        GetDictTuple(hv_GenParam, "file_name_image_only", &hv_FileNameImageOnly);
      }
      else if (0 != (int(HTuple(hv_Keys[hv_KeyIndex])==HTuple("file_name_annotation_only"))))
      {
        GetDictTuple(hv_GenParam, "file_name_annotation_only", &hv_FileNameAnnoOnly);
      }
      else if (0 != (int(HTuple(hv_Keys[hv_KeyIndex])==HTuple("ignore_unmatched_images"))))
      {
        GetDictTuple(hv_GenParam, "ignore_unmatched_images", &hv_IgnoreUnmatched);
      }
      else
      {
        throw HException(("Unknown generic parameter name : '"+HTuple(hv_Keys[hv_KeyIndex]))+"'");
      }
    }
    }
  }
  //
  //Check generic parameters.
  if (0 != (int(hv_ImageSubDirs!=HTuple())))
  {
    if (0 != (int(((hv_ImageSubDirs.TupleIsStringElem()).TupleMin())!=1)))
    {
      throw HException("'image_sub_dirs' must be a tuple of strings");
    }
  }
  if (0 != (int(hv_AnnotationSubDirs!=HTuple())))
  {
    if (0 != (int(((hv_AnnotationSubDirs.TupleIsStringElem()).TupleMin())!=1)))
    {
      throw HException(("'"+hv_Type)+"_sub_dirs' must be a tuple of strings");
    }
  }
  if (0 != (int(hv_FileNameImageOnly!=HTuple(""))))
  {
    if (0 != (int((hv_FileNameImageOnly.TupleLength())!=1)))
    {
      throw HException("'file_name_image_only' must be a single string");
    }
    else if (0 != ((hv_FileNameImageOnly.TupleIsString()).TupleNot()))
    {
      throw HException("'file_name_image_only' must be a string");
    }
  }
  if (0 != (int(hv_FileNameAnnoOnly!=HTuple(""))))
  {
    if (0 != (int((hv_FileNameAnnoOnly.TupleLength())!=1)))
    {
      throw HException(("'file_name_"+hv_Type)+"_only' must be a single string");
    }
    else if (0 != ((hv_FileNameAnnoOnly.TupleIsString()).TupleNot()))
    {
      throw HException(("'file_name_"+hv_Type)+"_only' must be a string");
    }
  }
  if (0 != (HTuple(int(hv_IgnoreUnmatched!=0)).TupleAnd(int(hv_IgnoreUnmatched!=1))))
  {
    throw HException("Unsupported value for 'ignore_unmatched_images' : "+hv_IgnoreUnmatched);
  }
  //
  //* Prepare image list.
  //
  //Get all image directories.
  if (0 != (int(hv_ImageSubDirs!=HTuple())))
  {
    hv_ImageDirs = (hv_ImageDir+"/")+hv_ImageSubDirs;
  }
  else
  {
    hv_ImageDirs = hv_ImageDir;
  }
  if (0 != (int(hv_ImageListIn==HTuple())))
  {
    //List all image files.
    prepare_image_lists(hv_ImageDirs, "default", &hv_ImageBaseNames, &hv_ImageExtensions, 
        &hv_ImageDirectories);
  }
  else
  {
    parse_filename(hv_ImageListIn, &hv_ImageBaseNames, &hv_ImageExtensions, &hv_ImageDirectories);
  }
  //
  if (0 != (int((hv_ImageBaseNames.TupleLength())==0)))
  {
    throw HException(("Error: Could not find any image files in folder: \""+hv_ImageDir)+"\"");
  }
  //
  //Get file paths of images relative to ImageDir.
  TupleRegexpMatch(HTuple(hv_ImageDirectories[0]), ".*?"+hv_ImageDir, &hv_CommonBasePathImage);
  TupleRegexpReplace(hv_ImageDirectories, hv_CommonBasePathImage, "", &hv_ImageDirectoriesRel);
  //
  if (0 != (int(hv_FileNameImageOnly!=HTuple(""))))
  {
    //Select only the images which include the specified string.
    TupleRegexpMatch(hv_ImageBaseNames, hv_FileNameImageOnly, &hv_ImageMatches);
    TupleFind(hv_ImageMatches.TupleNotEqualElem(""), 1, &hv_MatchIndices);
    hv_ImageBaseNames = HTuple(hv_ImageBaseNames[hv_MatchIndices]);
    hv_ImageExtensions = HTuple(hv_ImageExtensions[hv_MatchIndices]);
    hv_ImageDirectories = HTuple(hv_ImageDirectories[hv_MatchIndices]);
    hv_ImageDirectoriesRel = HTuple(hv_ImageDirectoriesRel[hv_MatchIndices]);
    //Remove the prefix or suffix of the image file name which is not included in the annotation file name.
    TupleRegexpReplace(hv_ImageBaseNames, hv_FileNameImageOnly, "", &hv_ImageBaseNamesToMatch);
  }
  else
  {
    hv_ImageBaseNamesToMatch = hv_ImageBaseNames;
  }
  //
  //Get ground truth labels from the image directory names.
  TupleRegexpMatch("/"+hv_ImageDirectories, ".*/([^/]+)/$", &hv_GroundTruthLabels);
  //
  //
  //* Prepare annotation file list.
  //
  //Get all annotation directories.
  if (0 != (int(hv_AnnotationSubDirs!=HTuple())))
  {
    hv_AnnotationDirs = (hv_AnnotationDir+"/")+hv_AnnotationSubDirs;
  }
  else
  {
    hv_AnnotationDirs = hv_AnnotationDir;
  }
  //
  //List all annotation files.
  if (0 != (int(hv_Type==HTuple("anomaly_detection"))))
  {
    hv_AnomalyExtensions.Clear();
    hv_AnomalyExtensions[0] = "hobj";
    hv_AnomalyExtensions[1] = "ima";
    hv_AnomalyExtensions[2] = "tif";
    hv_AnomalyExtensions[3] = "tiff";
    hv_AnomalyExtensions[4] = "gif";
    hv_AnomalyExtensions[5] = "bmp";
    hv_AnomalyExtensions[6] = "jpg";
    hv_AnomalyExtensions[7] = "jpeg";
    hv_AnomalyExtensions[8] = "jp2";
    hv_AnomalyExtensions[9] = "jxr";
    hv_AnomalyExtensions[10] = "png";
    hv_AnomalyExtensions[11] = "pcx";
    hv_AnomalyExtensions[12] = "ras";
    hv_AnomalyExtensions[13] = "xwd";
    hv_AnomalyExtensions[14] = "pbm";
    hv_AnomalyExtensions[15] = "pnm";
    hv_AnomalyExtensions[16] = "pgm";
    hv_AnomalyExtensions[17] = "ppm";
    prepare_image_lists(hv_AnnotationDirs, hv_AnomalyExtensions, &hv_AnnoBaseNames, 
        &hv_AnnoExtensions, &hv_AnnoDirectories);
  }
  else
  {
    prepare_image_lists(hv_AnnotationDirs, "default", &hv_AnnoBaseNames, &hv_AnnoExtensions, 
        &hv_AnnoDirectories);
  }
  //
  if (0 != (int((hv_AnnoBaseNames.TupleLength())==0)))
  {
    if (0 != (int(hv_Type==HTuple("segmentation"))))
    {
      throw HException("No segmentation images found");
    }
    else if (0 != (int(hv_Type==HTuple("anomaly_detection"))))
    {
      (*hv_ImageListOut) = ((hv_ImageDirectoriesRel+hv_ImageBaseNames)+".")+hv_ImageExtensions;
      TupleRegexpReplace((*hv_ImageListOut), "^/", "", &(*hv_ImageListOut));
      (*hv_LabelListOut) = hv_GroundTruthLabels;
      (*hv_AnnotationListOut) = HTuple(hv_ImageBaseNames.TupleLength(),"");
      return;
    }
    else
    {
      throw HException(("Invalid dataset type '"+hv_Type)+"'");
    }
  }
  //
  //Get paths of annotation files relative to AnnotationDir.
  TupleRegexpMatch(HTuple(hv_AnnoDirectories[0]), ".*?"+hv_AnnotationDir, &hv_CommonBasePathAnno);
  TupleRegexpReplace(hv_AnnoDirectories, hv_CommonBasePathAnno, "", &hv_AnnoDirectoriesRel);
  //
  if (0 != (int(hv_FileNameAnnoOnly!=HTuple(""))))
  {
    //Select only the annotations which include the specified string.
    TupleRegexpMatch(hv_AnnoBaseNames, hv_FileNameAnnoOnly, &hv_AnnoMatches);
    TupleFind(hv_AnnoMatches.TupleNotEqualElem(""), 1, &hv_MatchIndices);
    hv_AnnoBaseNames = HTuple(hv_AnnoBaseNames[hv_MatchIndices]);
    hv_AnnoExtensions = HTuple(hv_AnnoExtensions[hv_MatchIndices]);
    hv_AnnoDirectories = HTuple(hv_AnnoDirectories[hv_MatchIndices]);
    hv_AnnoDirectoriesRel = HTuple(hv_AnnoDirectoriesRel[hv_MatchIndices]);
    //Remove the prefix or suffix of the annotation file name which is not included in the image file name.
    TupleRegexpReplace(hv_AnnoBaseNames, hv_FileNameAnnoOnly, "", &hv_AnnoBaseNamesToMatch);
  }
  else
  {
    hv_AnnoBaseNamesToMatch = hv_AnnoBaseNames;
  }
  //
  //
  //* Match image and annotation lists.
  //
  //Match to every image file an annotation file.
  hv_AnnoMatchIndices = HTuple();
  hv_ImageMatchIndices = HTuple();
  {
  HTuple end_val169 = (hv_ImageBaseNames.TupleLength())-1;
  HTuple step_val169 = 1;
  for (hv_ImageIndex=0; hv_ImageIndex.Continue(end_val169, step_val169); hv_ImageIndex += step_val169)
  {
    //1: Try to match identical file names.
    hv_AnnoMatchIndex = hv_AnnoBaseNamesToMatch.TupleFind(HTuple(hv_ImageBaseNamesToMatch[hv_ImageIndex]));
    if (0 != (HTuple(int(hv_AnnoMatchIndex>=0)).TupleAnd(int((hv_AnnoMatchIndex.TupleLength())==1))))
    {
      //Exactly one match was found.
      hv_AnnoMatchIndices = hv_AnnoMatchIndices.TupleConcat(hv_AnnoMatchIndex);
      hv_ImageMatchIndices = hv_ImageMatchIndices.TupleConcat(hv_ImageIndex);
    }
    else
    {
      //2: Try to match file name subparts.
      TupleRegexpMatch(hv_AnnoBaseNamesToMatch, HTuple(hv_ImageBaseNamesToMatch[hv_ImageIndex]), 
          &hv_Match);
      TupleFind(hv_Match.TupleNotEqualElem(""), 1, &hv_AnnoMatchIndex);
      if (0 != (HTuple(int(hv_AnnoMatchIndex>=0)).TupleAnd(int((hv_AnnoMatchIndex.TupleLength())==1))))
      {
        //Exactly one match was found.
        hv_AnnoMatchIndices = hv_AnnoMatchIndices.TupleConcat(hv_AnnoMatchIndex);
        hv_ImageMatchIndices = hv_ImageMatchIndices.TupleConcat(hv_ImageIndex);
      }
      else if (0 != (int((hv_AnnoMatchIndex.TupleLength())>1)))
      {
        //3: There are several name matches.
        //Check if one of these matches has a common relative path.
        TupleRegexpMatch(HTuple(hv_AnnoDirectoriesRel[hv_AnnoMatchIndex]), HTuple(hv_ImageDirectoriesRel[hv_ImageIndex]), 
            &hv_PathMatch);
        TupleFind(hv_PathMatch.TupleNotEqualElem(""), 1, &hv_PathMatchIndex);
        if (0 != (HTuple(int(hv_PathMatchIndex>=0)).TupleAnd(int((hv_PathMatchIndex.TupleLength())==1))))
        {
          //Exactly one annotation matches path and name.
          hv_AnnoMatchIndices = hv_AnnoMatchIndices.TupleConcat(HTuple(hv_AnnoMatchIndex[hv_PathMatchIndex]));
          hv_ImageMatchIndices = hv_ImageMatchIndices.TupleConcat(hv_ImageIndex);
        }
        else if (0 != (int((hv_PathMatchIndex.TupleLength())>1)))
        {
          //There should be only one annotation file matching each image.
          hv_ImagePath = ((HTuple(hv_ImageDirectoriesRel[hv_ImageIndex])+HTuple(hv_ImageBaseNames[hv_ImageIndex]))+".")+HTuple(hv_ImageExtensions[hv_ImageIndex]);
          throw HException((("Multiple matching "+hv_Type)+" files for image ")+hv_ImagePath);
        }
      }
    }
  }
  }
  //
  //
  //* Final checks.
  //
  //Check if the corresponding matches are collected correctly.
  if (0 != (int((hv_AnnoMatchIndices.TupleLength())!=(hv_ImageMatchIndices.TupleLength()))))
  {
    throw HException("An error occurred while trying to match annotation and image files");
  }
  //Check if each match is unique.
  if (0 != (int(((hv_AnnoMatchIndices.TupleSort()).TupleUniq())!=(hv_AnnoMatchIndices.TupleSort()))))
  {
    throw HException(("A single "+hv_Type)+" file has matched with multiple images");
  }
  //
  hv_ImageIndices = hv_ImageMatchIndices;
  if (0 != (int(hv_Type==HTuple("anomaly_detection"))))
  {
    //For anomaly detection, images without a matching annotation file are okay.
    //So in this case, we use all images and not only the ones with a match.
    hv_ImageIndices = HTuple::TupleGenSequence(0,(hv_ImageBaseNames.TupleLength())-1,1);
  }
  else if (0 != (hv_IgnoreUnmatched.TupleNot()))
  {
    //Check if every image matched with an annotation file.
    if (0 != (int((hv_ImageMatchIndices.TupleLength())!=(hv_ImageBaseNames.TupleLength()))))
    {
      throw HException(("Not every image in ImageDir has a corresponding "+hv_Type)+" file");
    }
  }
  //
  //Create final image list.
  (*hv_ImageListOut) = ((hv_ImageDirectoriesRel+hv_ImageBaseNames)+".")+hv_ImageExtensions;
  (*hv_ImageListOut) = HTuple((*hv_ImageListOut)[hv_ImageIndices]);
  TupleRegexpReplace((*hv_ImageListOut), "^/", "", &(*hv_ImageListOut));
  //
  //Create final ground truth label list.
  (*hv_LabelListOut) = HTuple(hv_GroundTruthLabels[hv_ImageIndices]);
  //
  //Create final annotation list.
  (*hv_AnnotationListOut) = ((hv_AnnoDirectoriesRel+hv_AnnoBaseNames)+".")+hv_AnnoExtensions;
  if (0 != (int(hv_Type==HTuple("anomaly_detection"))))
  {
    hv_AnnotationList = HTuple((*hv_ImageListOut).TupleLength(),"");
    hv_AnnotationList[hv_ImageMatchIndices] = HTuple((*hv_AnnotationListOut)[hv_AnnoMatchIndices]);
    (*hv_AnnotationListOut) = hv_AnnotationList;
  }
  else
  {
    (*hv_AnnotationListOut) = HTuple((*hv_AnnotationListOut)[hv_AnnoMatchIndices]);
  }
  TupleRegexpReplace((*hv_AnnotationListOut), "^/", "", &(*hv_AnnotationListOut));
  //
  return;
}

// Chapter: File / Misc
// Short Description: Get all image files under the given path 
void list_image_files (HTuple hv_ImageDirectory, HTuple hv_Extensions, HTuple hv_Options, 
    HTuple *hv_ImageFiles)
{

  // Local iconic variables

  // Local control variables
  HTuple  hv_ImageDirectoryIndex, hv_ImageFilesTmp;
  HTuple  hv_CurrentImageDirectory, hv_HalconImages, hv_OS;
  HTuple  hv_Directories, hv_Index, hv_Length, hv_NetworkDrive;
  HTuple  hv_Substring, hv_FileExists, hv_AllFiles, hv_i;
  HTuple  hv_Selection;

  //This procedure returns all files in a given directory
  //with one of the suffixes specified in Extensions.
  //
  //Input parameters:
  //ImageDirectory: Directory or a tuple of directories with images.
  //   If a directory is not found locally, the respective directory
  //   is searched under %HALCONIMAGES%/ImageDirectory.
  //   See the Installation Guide for further information
  //   in case %HALCONIMAGES% is not set.
  //Extensions: A string tuple containing the extensions to be found
  //   e.g. ['png','tif',jpg'] or others
  //If Extensions is set to 'default' or the empty string '',
  //   all image suffixes supported by HALCON are used.
  //Options: as in the operator list_files, except that the 'files'
  //   option is always used. Note that the 'directories' option
  //   has no effect but increases runtime, because only files are
  //   returned.
  //
  //Output parameter:
  //ImageFiles: A tuple of all found image file names
  //
  if (0 != (HTuple(HTuple(int(hv_Extensions==HTuple())).TupleOr(int(hv_Extensions==HTuple("")))).TupleOr(int(hv_Extensions==HTuple("default")))))
  {
    hv_Extensions.Clear();
    hv_Extensions[0] = "ima";
    hv_Extensions[1] = "tif";
    hv_Extensions[2] = "tiff";
    hv_Extensions[3] = "gif";
    hv_Extensions[4] = "bmp";
    hv_Extensions[5] = "jpg";
    hv_Extensions[6] = "jpeg";
    hv_Extensions[7] = "jp2";
    hv_Extensions[8] = "jxr";
    hv_Extensions[9] = "png";
    hv_Extensions[10] = "pcx";
    hv_Extensions[11] = "ras";
    hv_Extensions[12] = "xwd";
    hv_Extensions[13] = "pbm";
    hv_Extensions[14] = "pnm";
    hv_Extensions[15] = "pgm";
    hv_Extensions[16] = "ppm";
    //
  }
  (*hv_ImageFiles) = HTuple();
  //Loop through all given image directories.
  {
  HTuple end_val27 = (hv_ImageDirectory.TupleLength())-1;
  HTuple step_val27 = 1;
  for (hv_ImageDirectoryIndex=0; hv_ImageDirectoryIndex.Continue(end_val27, step_val27); hv_ImageDirectoryIndex += step_val27)
  {
    hv_ImageFilesTmp = HTuple();
    hv_CurrentImageDirectory = HTuple(hv_ImageDirectory[hv_ImageDirectoryIndex]);
    if (0 != (int(hv_CurrentImageDirectory==HTuple(""))))
    {
      hv_CurrentImageDirectory = ".";
    }
    GetSystem("image_dir", &hv_HalconImages);
    GetSystem("operating_system", &hv_OS);
    if (0 != (int((hv_OS.TupleSubstr(0,2))==HTuple("Win"))))
    {
      hv_HalconImages = hv_HalconImages.TupleSplit(";");
    }
    else
    {
      hv_HalconImages = hv_HalconImages.TupleSplit(":");
    }
    hv_Directories = hv_CurrentImageDirectory;
    {
    HTuple end_val41 = (hv_HalconImages.TupleLength())-1;
    HTuple step_val41 = 1;
    for (hv_Index=0; hv_Index.Continue(end_val41, step_val41); hv_Index += step_val41)
    {
      hv_Directories = hv_Directories.TupleConcat((HTuple(hv_HalconImages[hv_Index])+"/")+hv_CurrentImageDirectory);
    }
    }
    TupleStrlen(hv_Directories, &hv_Length);
    TupleGenConst(hv_Length.TupleLength(), 0, &hv_NetworkDrive);
    if (0 != (int((hv_OS.TupleSubstr(0,2))==HTuple("Win"))))
    {
      {
      HTuple end_val47 = (hv_Length.TupleLength())-1;
      HTuple step_val47 = 1;
      for (hv_Index=0; hv_Index.Continue(end_val47, step_val47); hv_Index += step_val47)
      {
        if (0 != (int((HTuple(hv_Directories[hv_Index]).TupleStrlen())>1)))
        {
          TupleStrFirstN(HTuple(hv_Directories[hv_Index]), 1, &hv_Substring);
          if (0 != (HTuple(int(hv_Substring==HTuple("//"))).TupleOr(int(hv_Substring==HTuple("\\\\")))))
          {
            hv_NetworkDrive[hv_Index] = 1;
          }
        }
      }
      }
    }
    hv_ImageFilesTmp = HTuple();
    {
    HTuple end_val57 = (hv_Directories.TupleLength())-1;
    HTuple step_val57 = 1;
    for (hv_Index=0; hv_Index.Continue(end_val57, step_val57); hv_Index += step_val57)
    {
      FileExists(HTuple(hv_Directories[hv_Index]), &hv_FileExists);
      if (0 != hv_FileExists)
      {
        ListFiles(HTuple(hv_Directories[hv_Index]), HTuple("files").TupleConcat(hv_Options), 
            &hv_AllFiles);
        hv_ImageFilesTmp = HTuple();
        {
        HTuple end_val62 = (hv_Extensions.TupleLength())-1;
        HTuple step_val62 = 1;
        for (hv_i=0; hv_i.Continue(end_val62, step_val62); hv_i += step_val62)
        {
          TupleRegexpSelect(hv_AllFiles, ((".*"+HTuple(hv_Extensions[hv_i]))+"$").TupleConcat("ignore_case"), 
              &hv_Selection);
          hv_ImageFilesTmp = hv_ImageFilesTmp.TupleConcat(hv_Selection);
        }
        }
        TupleRegexpReplace(hv_ImageFilesTmp, (HTuple("\\\\").Append("replace_all")), 
            "/", &hv_ImageFilesTmp);
        if (0 != (HTuple(hv_NetworkDrive[hv_Index])))
        {
          TupleRegexpReplace(hv_ImageFilesTmp, (HTuple("//").Append("replace_all")), 
              "/", &hv_ImageFilesTmp);
          hv_ImageFilesTmp = "/"+hv_ImageFilesTmp;
        }
        else
        {
          TupleRegexpReplace(hv_ImageFilesTmp, (HTuple("//").Append("replace_all")), 
              "/", &hv_ImageFilesTmp);
        }
        break;
      }
    }
    }
    //Concatenate the output image paths.
    (*hv_ImageFiles) = (*hv_ImageFiles).TupleConcat(hv_ImageFilesTmp);
  }
  }
  return;
}

// Chapter: Deep Learning / Model
// Short Description: shuffles the input colors in a deterministic way 
void make_neighboring_colors_distinguishable (HTuple hv_ColorsRainbow, HTuple *hv_Colors)
{

  // Local iconic variables

  // Local control variables
  HTuple  hv_NumColors, hv_NumChunks, hv_NumLeftOver;
  HTuple  hv_ColorsPerChunk, hv_StartIdx, hv_S, hv_EndIdx;
  HTuple  hv_IdxsLeft, hv_IdxsRight;

  //
  //Shuffle the input colors in a deterministic way
  //to make adjacent colors more distinguishable.
  //Neighboring colors from the input are distributed to every NumChunks
  //position in the output.
  //Depending on the number of colors, increase NumChunks.
  hv_NumColors = hv_ColorsRainbow.TupleLength();
  if (0 != (int(hv_NumColors>=8)))
  {
    hv_NumChunks = 3;
    if (0 != (int(hv_NumColors>=40)))
    {
      hv_NumChunks = 6;
    }
    else if (0 != (int(hv_NumColors>=20)))
    {
      hv_NumChunks = 4;
    }
    (*hv_Colors) = HTuple(hv_NumColors,-1);
    //Check if the Number of Colors is dividable by NumChunks.
    hv_NumLeftOver = hv_NumColors%hv_NumChunks;
    hv_ColorsPerChunk = (hv_NumColors/hv_NumChunks).TupleInt();
    hv_StartIdx = 0;
    {
    HTuple end_val19 = hv_NumChunks-1;
    HTuple step_val19 = 1;
    for (hv_S=0; hv_S.Continue(end_val19, step_val19); hv_S += step_val19)
    {
      hv_EndIdx = (hv_StartIdx+hv_ColorsPerChunk)-1;
      if (0 != (int(hv_S<hv_NumLeftOver)))
      {
        hv_EndIdx += 1;
      }
      hv_IdxsLeft = HTuple::TupleGenSequence(hv_S,hv_NumColors-1,hv_NumChunks);
      hv_IdxsRight = HTuple::TupleGenSequence(hv_StartIdx,hv_EndIdx,1);
      (*hv_Colors)[HTuple::TupleGenSequence(hv_S,hv_NumColors-1,hv_NumChunks)] = hv_ColorsRainbow.TupleSelectRange(hv_StartIdx,hv_EndIdx);
      hv_StartIdx = hv_EndIdx+1;
    }
    }
  }
  else
  {
    (*hv_Colors) = hv_ColorsRainbow;
  }
  return;
}

// Chapter: Graphics / Window
// Short Description: Open a window next to the given WindowHandleFather.  
void open_child_window (HTuple hv_WindowHandleFather, HTuple hv_Font, HTuple hv_FontSize, 
    HTuple hv_Text, HTuple hv_PrevWindowCoordinates, HTuple hv_WindowHandleDict, 
    HTuple hv_WindowHandleKey, HTuple *hv_WindowHandleChild, HTuple *hv_PrevWindowCoordinatesOut)
{

  // Local iconic variables

  // Local control variables
  HTuple  hv_StringWidth, hv_IndexText, hv__, hv_TextWidth;
  HTuple  hv_WindowRow, hv_WindowColumn, hv_WindowWidth, hv_WindowHeight;
  HTuple  hv_MetaInfo;

  //
  //This procedure opens a window next to the given WindowHandleFather.
  //
  //Get the maximum width of the text to be displayed.
  //The width should be at leat 200.
  hv_StringWidth = 150;
  {
  HTuple end_val6 = (hv_Text.TupleLength())-1;
  HTuple step_val6 = 1;
  for (hv_IndexText=0; hv_IndexText.Continue(end_val6, step_val6); hv_IndexText += step_val6)
  {
    GetStringExtents(hv_WindowHandleFather, HTuple(hv_Text[hv_IndexText]), &hv__, 
        &hv__, &hv_TextWidth, &hv__);
    hv_StringWidth = hv_StringWidth.TupleMax2(hv_TextWidth);
  }
  }
  //
  //Define window coordinates.
  hv_WindowRow = ((const HTuple&)hv_PrevWindowCoordinates)[0];
  hv_WindowColumn = (HTuple(hv_PrevWindowCoordinates[1])+HTuple(hv_PrevWindowCoordinates[2]))+5;
  hv_WindowWidth = hv_StringWidth+(2*12.0);
  hv_WindowHeight = ((const HTuple&)hv_PrevWindowCoordinates)[3];
  //
  SetWindowAttr("background_color","black");
  OpenWindow(hv_WindowRow,hv_WindowColumn,hv_WindowWidth,hv_WindowHeight,0,"visible","",&(*hv_WindowHandleChild));
  HDevWindowStack::Push((*hv_WindowHandleChild));
  set_display_font((*hv_WindowHandleChild), hv_FontSize, hv_Font, "true", "false");
  //
  //Return the coordinates of the new window.
  (*hv_PrevWindowCoordinatesOut).Clear();
  (*hv_PrevWindowCoordinatesOut).Append(hv_WindowRow);
  (*hv_PrevWindowCoordinatesOut).Append(hv_WindowColumn);
  (*hv_PrevWindowCoordinatesOut).Append(hv_WindowWidth);
  (*hv_PrevWindowCoordinatesOut).Append(hv_WindowHeight);
  //
  //Set some meta information about the new child window handle.
  GetDictTuple(hv_WindowHandleDict, "meta_information", &hv_MetaInfo);
  SetDictTuple(hv_MetaInfo, hv_WindowHandleKey+"_child_window_coordinates", (*hv_PrevWindowCoordinatesOut));
  SetDictTuple(hv_WindowHandleDict, "meta_information", hv_MetaInfo);
  //
  return;
}

// Chapter: Graphics / Window
// Short Description: Open a new window, either next to the last ones, or in a new row. 
void open_next_window (HTuple hv_Font, HTuple hv_FontSize, HTuple hv_ShowBottomDesc, 
    HTuple hv_WidthImage, HTuple hv_HeightImage, HTuple hv_MapColorBarWidth, HTuple hv_ScaleWindows, 
    HTuple hv_ThresholdWidth, HTuple hv_PrevWindowCoordinates, HTuple hv_WindowHandleDict, 
    HTuple hv_WindowHandleKey, HTuple *hv_WindowHandleNew, HTuple *hv_WindowImageRatioHeight, 
    HTuple *hv_PrevWindowCoordinatesOut)
{

  // Local iconic variables

  // Local control variables
  HTuple  hv_PrevWindowRow, hv_PrevWindowColumn;
  HTuple  hv_PrevWindowWidth, hv_PrevWindowHeight, hv_WindowRow;
  HTuple  hv_WindowColumn, hv_Ascent, hv_Descent, hv__, hv_MarginBottom;
  HTuple  hv_WindowWidth, hv_WindowHeight, hv_WindowImageRatioWidth;
  HTuple  hv_SetPartRow2, hv_SetPartColumn2, hv_MetaInfo;

  //
  //This procedure opens a new window, either next to
  //the last ones, or in a new row.
  //
  //Get coordinates of previous window.
  hv_PrevWindowRow = ((const HTuple&)hv_PrevWindowCoordinates)[0];
  hv_PrevWindowColumn = ((const HTuple&)hv_PrevWindowCoordinates)[1];
  hv_PrevWindowWidth = ((const HTuple&)hv_PrevWindowCoordinates)[2];
  hv_PrevWindowHeight = ((const HTuple&)hv_PrevWindowCoordinates)[3];
  //
  if (0 != (int((hv_PrevWindowColumn+hv_PrevWindowWidth)>hv_ThresholdWidth)))
  {
    //Open window in new row.
    hv_WindowRow = (hv_PrevWindowRow+hv_PrevWindowHeight)+55;
    hv_WindowColumn = 0;
  }
  else
  {
    //Open window in same row.
    hv_WindowRow = hv_PrevWindowRow;
    hv_WindowColumn = hv_PrevWindowColumn+hv_PrevWindowWidth;
    if (0 != (int(hv_WindowColumn!=0)))
    {
      hv_WindowColumn += 5;
    }
  }
  //
  dev_open_window_fit_size(hv_WindowRow, hv_WindowColumn, hv_WidthImage, hv_HeightImage, 
      (HTuple(500).Append(800))*hv_ScaleWindows, (HTuple(400).Append(600))*hv_ScaleWindows, 0,
      &(*hv_WindowHandleNew));
  set_display_font((*hv_WindowHandleNew), hv_FontSize, hv_Font, "true", "false");
  //
  //Add MarginBottom and MapColorBarWidth to window.
  if (0 != hv_ShowBottomDesc)
  {
    GetStringExtents((*hv_WindowHandleNew), "Test_string", &hv_Ascent, &hv_Descent, 
        &hv__, &hv__);
    hv_MarginBottom = ((2*12)+hv_Ascent)+hv_Descent;
  }
  else
  {
    hv_MarginBottom = 0;
  }
  GetWindowExtents((*hv_WindowHandleNew), &hv__, &hv__, &hv_WindowWidth, &hv_WindowHeight);
  if (HDevWindowStack::IsOpen())
    SetWindowExtents(HDevWindowStack::GetActive(),hv_WindowRow, hv_WindowColumn, 
        hv_WindowWidth+hv_MapColorBarWidth, hv_WindowHeight+hv_MarginBottom);
  //
  //Get and set meta information of new window handle.
  update_window_meta_information((*hv_WindowHandleNew), hv_WidthImage, hv_HeightImage, 
      hv_WindowRow, hv_WindowColumn, hv_MapColorBarWidth, hv_MarginBottom, &(*hv_WindowImageRatioHeight), 
      &hv_WindowImageRatioWidth, &hv_SetPartRow2, &hv_SetPartColumn2, &(*hv_PrevWindowCoordinatesOut));
  //
  //Set window handle and some meta information about the new window handle.
  SetDictTuple(hv_WindowHandleDict, hv_WindowHandleKey, (*hv_WindowHandleNew));
  GetDictTuple(hv_WindowHandleDict, "meta_information", &hv_MetaInfo);
  SetDictTuple(hv_MetaInfo, hv_WindowHandleKey+"_window_image_ratio_height", (*hv_WindowImageRatioHeight));
  SetDictTuple(hv_MetaInfo, hv_WindowHandleKey+"_window_image_ratio_width", hv_WindowImageRatioWidth);
  SetDictTuple(hv_MetaInfo, hv_WindowHandleKey+"_set_part_row2", hv_SetPartRow2);
  SetDictTuple(hv_MetaInfo, hv_WindowHandleKey+"_set_part_column2", hv_SetPartColumn2);
  SetDictTuple(hv_MetaInfo, hv_WindowHandleKey+"_margin_bottom", hv_MarginBottom);
  SetDictTuple(hv_MetaInfo, hv_WindowHandleKey+"_map_color_bar_with", hv_MapColorBarWidth);
  SetDictTuple(hv_MetaInfo, hv_WindowHandleKey+"_window_coordinates", (*hv_PrevWindowCoordinatesOut));
  SetDictTuple(hv_WindowHandleDict, "meta_information", hv_MetaInfo);
  //
  return;
}

// Chapter: File / Misc
// Short Description: Parse a filename into directory, base filename, and extension 
void parse_filename (HTuple hv_FileName, HTuple *hv_BaseName, HTuple *hv_Extension, 
    HTuple *hv_Directory)
{

  // Local control variables
  HTuple  hv_DirectoryTmp, hv_Substring;

  //This procedure gets a filename (with full path) as input
  //and returns the directory path, the base filename and the extension
  //in three different strings.
  //
  //In the output path the path separators will be replaced
  //by '/' in all cases.
  //
  //The procedure shows the possibilities of regular expressions in HALCON.
  //
  //Input parameters:
  //FileName: The input filename
  //
  //Output parameters:
  //BaseName: The filename without directory description and file extension
  //Extension: The file extension
  //Directory: The directory path
  //
  //Example:
  //basename('C:/images/part_01.png',...) returns
  //BaseName = 'part_01'
  //Extension = 'png'
  //Directory = 'C:\\images\\' (on Windows systems)
  //
  //Explanation of the regular expressions:
  //
  //'([^\\\\/]*?)(?:\\.[^.]*)?$':
  //To start at the end, the '$' matches the end of the string,
  //so it is best to read the expression from right to left.
  //The part in brackets (?:\\.[^.}*) denotes a non-capturing group.
  //That means, that this part is matched, but not captured
  //in contrast to the first bracketed group ([^\\\\/], see below.)
  //\\.[^.]* matches a dot '.' followed by as many non-dots as possible.
  //So (?:\\.[^.]*)? matches the file extension, if any.
  //The '?' at the end assures, that even if no extension exists,
  //a correct match is returned.
  //The first part in brackets ([^\\\\/]*?) is a capture group,
  //which means, that if a match is found, only the part in
  //brackets is returned as a result.
  //Because both HDevelop strings and regular expressions need a '\\'
  //to describe a backslash, inside regular expressions within HDevelop
  //a backslash has to be written as '\\\\'.
  //[^\\\\/] matches any character but a slash or backslash ('\\' in HDevelop)
  //[^\\\\/]*? matches a string od 0..n characters (except '/' or '\\')
  //where the '?' after the '*' switches the greediness off,
  //that means, that the shortest possible match is returned.
  //This option is necessary to cut off the extension
  //but only if (?:\\.[^.]*)? is able to match one.
  //To summarize, the regular expression matches that part of
  //the input string, that follows after the last '/' or '\\' and
  //cuts off the extension (if any) after the last '.'.
  //
  //'\\.([^.]*)$':
  //This matches everything after the last '.' of the input string.
  //Because ([^.]) is a capturing group,
  //only the part after the dot is returned.
  //
  //'.*[\\\\/]':
  //This matches the longest substring with a '/' or a '\\' at the end.
  //
  TupleRegexpMatch(hv_FileName, ".*[\\\\/]", &hv_DirectoryTmp);
  TupleSubstr(hv_FileName, hv_DirectoryTmp.TupleStrlen(), (hv_FileName.TupleStrlen())-1, 
      &hv_Substring);
  TupleRegexpMatch(hv_Substring, "([^\\\\/]*?)(?:\\.[^.]*)?$", &(*hv_BaseName));
  TupleRegexpMatch(hv_Substring, "\\.([^.]*)$", &(*hv_Extension));
  //
  //
  //Finally all found backslashes ('\\') are converted
  //to a slash to get consistent paths
  TupleRegexpReplace(hv_DirectoryTmp, (HTuple("\\\\").Append("replace_all")), "/", 
      &(*hv_Directory));
  return;
}

// Chapter: Graphics / Output
// Short Description: This procedure plots tuples representing functions or curves in a coordinate system. 
void plot_tuple (HTuple hv_WindowHandle, HTuple hv_XValues, HTuple hv_YValues, HTuple hv_XLabel, 
    HTuple hv_YLabel, HTuple hv_Color, HTuple hv_GenParamName, HTuple hv_GenParamValue)
{

  // Local iconic variables
  HObject  ho_ContourXGrid, ho_ContourYGrid, ho_XArrow;
  HObject  ho_YArrow, ho_ContourXTick, ho_ContourYTick, ho_Contour;
  HObject  ho_Cross, ho_Filled, ho_Stair, ho_StairTmp;

  // Local control variables
  HTuple  hv_PreviousWindowHandle, hv_ClipRegion;
  HTuple  hv_Row, hv_Column, hv_Width, hv_Height, hv_PartRow1;
  HTuple  hv_PartColumn1, hv_PartRow2, hv_PartColumn2, hv_Red;
  HTuple  hv_Green, hv_Blue, hv_DrawMode, hv_OriginStyle;
  HTuple  hv_XAxisEndValue, hv_YAxisEndValue, hv_XAxisStartValue;
  HTuple  hv_YAxisStartValue, hv_XValuesAreStrings, hv_XTickValues;
  HTuple  hv_XTicks, hv_YAxisPosition, hv_XAxisPosition, hv_LeftBorder;
  HTuple  hv_RightBorder, hv_UpperBorder, hv_LowerBorder;
  HTuple  hv_AxesColor, hv_Style, hv_Clip, hv_YTicks, hv_XGrid;
  HTuple  hv_YGrid, hv_GridColor, hv_YPosition, hv_FormatX;
  HTuple  hv_FormatY, hv_NumGenParamNames, hv_NumGenParamValues;
  HTuple  hv_GenParamIndex, hv_XGridTicks, hv_YTickDirection;
  HTuple  hv_XTickDirection, hv_XAxisWidthPx, hv_XAxisWidth;
  HTuple  hv_XScaleFactor, hv_YAxisHeightPx, hv_YAxisHeight;
  HTuple  hv_YScaleFactor, hv_YAxisOffsetPx, hv_XAxisOffsetPx;
  HTuple  hv_DotStyle, hv_XGridValues, hv_XGridStart, hv_XCoord;
  HTuple  hv_IndexGrid, hv_YGridValues, hv_YGridStart, hv_YCoord;
  HTuple  hv_Ascent, hv_Descent, hv_TextWidthXLabel, hv_TextHeightXLabel;
  HTuple  hv_TextWidthYLabel, hv_TextHeightYLabel, hv_XTickStart;
  HTuple  hv_Indices, hv_TypeTicks, hv_IndexTicks, hv_Ascent1;
  HTuple  hv_Descent1, hv_TextWidthXTicks, hv_TextHeightXTicks;
  HTuple  hv_YTickValues, hv_YTickStart, hv_TextWidthYTicks;
  HTuple  hv_TextHeightYTicks, hv_Num, hv_I, hv_YSelected;
  HTuple  hv_Y1Selected, hv_X1Selected, hv_Index, hv_Row1;
  HTuple  hv_Row2, hv_Col1, hv_Col2;

  //This procedure plots tuples representing functions
  //or curves in a coordinate system.
  //
  //Input parameters:
  //
  //XValues: X values of the function to be plotted
  //         If XValues is set to [], it is internally set to 0,1,2,...,|YValues|-1.
  //         If XValues is a tuple of strings, the values are taken as categories.
  //
  //YValues: Y values of the function(s) to be plotted
  //         If YValues is set to [], it is internally set to 0,1,2,...,|XValues|-1.
  //         The number of y values must be equal to the number of x values
  //         or an integral multiple. In the latter case,
  //         multiple functions are plotted, that share the same x values.
  //
  //XLabel: X-axis label
  //
  //YLabel: Y-axis label
  //
  //Color: Color of the plotted function
  //       If [] is given, the currently set display color is used.
  //       If 'none is given, the function is not plotted, but only
  //       the coordinate axes as specified.
  //       If more than one color is given, multiple functions
  //       can be displayed in different colors.
  //
  //GenParamName:  Generic parameters to control the presentation
  //               Possible Values:
  //   'axes_color': coordinate system color
  //                 Default: 'white'
  //                 If 'none' is given, no coordinate system is shown.
  //   'style': Graph style
  //            Possible values: 'line' (default), 'cross', 'step', 'filled'
  //   'clip': Clip graph to coordinate system area
  //           Possible values: 'yes', 'no' (default)
  //   'ticks': Control display of ticks on the axes
  //            If 'min_max_origin' is given (default), ticks are shown
  //            at the minimum and maximum values of the axes and at the
  //            intercept point of x- and y-axis.
  //            If 'none' is given, no ticks are shown.
  //            If any number != 0 is given, it is interpreted as distance
  //            between the ticks.
  //   'ticks_x': Control display of ticks on x-axis only
  //   'ticks_y': Control display of ticks on y-axis only
  //   'format_x': Format of the values next to the ticks of the x-axis
  //               (see tuple_string for more details).
  //   'format_y': Format of the values next to the ticks of the y-axis
  //               (see tuple_string for more details).
  //   'grid': Control display of grid lines within the coordinate system
  //           If 'min_max_origin' is given (default), grid lines are shown
  //           at the minimum and maximum values of the axes.
  //           If 'none' is given, no grid lines are shown.
  //           If any number != 0 is given, it is interpreted as distance
  //           between the grid lines.
  //   'grid_x': Control display of grid lines for the x-axis only
  //   'grid_y': Control display of grid lines for the y-axis only
  //   'grid_color': Color of the grid (default: 'dim gray')
  //   'margin': The distance in pixels of the coordinate system area
  //             to all four window borders.
  //   'margin_left': The distance in pixels of the coordinate system area
  //                  to the left window border.
  //   'margin_right': The distance in pixels of the coordinate system area
  //                   to the right window border.
  //   'margin_top': The distance in pixels of the coordinate system area
  //                 to the upper window border.
  //   'margin_bottom': The distance in pixels of the coordinate system area
  //                    to the lower window border.
  //   'start_x': Lowest x value of the x-axis
  //              Default: min(XValues)
  //   'end_x': Highest x value of the x-axis
  //            Default: max(XValues)
  //   'start_y': Lowest y value of the y-axis
  //              Default: min(YValues)
  //   'end_y': Highest y value of the y-axis
  //            Default: max(YValues)
  //   'axis_location_x': Either 'bottom', 'origin', or 'top'
  //               to position the x-axis conveniently,
  //               or the Y coordinate of the intercept point of x- and y-axis.
  //               Default: 'bottom'
  //               (Used to be called 'origin_y')
  //   'axis_location_y': Either 'left', 'origin', or 'right'
  //               to position the y-axis conveniently,
  //               or the X coordinate of the intercept point of x- and y-axis.
  //               Default: 'left'
  //               (Used to be called 'origin_x')
  //
  //GenParamValue: Values of the generic parameters of GenericParamName
  //
  //
  //Store current display settings
  if (HDevWindowStack::IsOpen())
    hv_PreviousWindowHandle = HDevWindowStack::GetActive();
  HDevWindowStack::SetActive(hv_WindowHandle);
  GetSystem("clip_region", &hv_ClipRegion);
  GetWindowExtents(hv_WindowHandle, &hv_Row, &hv_Column, &hv_Width, &hv_Height);
  GetPart(hv_WindowHandle, &hv_PartRow1, &hv_PartColumn1, &hv_PartRow2, &hv_PartColumn2);
  GetRgb(hv_WindowHandle, &hv_Red, &hv_Green, &hv_Blue);
  GetDraw(hv_WindowHandle, &hv_DrawMode);
  GetLineStyle(hv_WindowHandle, &hv_OriginStyle);
  //
  //Set display parameters
  SetLineStyle(hv_WindowHandle, HTuple());
  SetSystem("clip_region", "false");
  if (HDevWindowStack::IsOpen())
    SetPart(HDevWindowStack::GetActive(),0, 0, hv_Height-1, hv_Width-1);
  //
  //Check input coordinates
  //
  if (0 != (HTuple(int(hv_XValues==HTuple())).TupleAnd(int(hv_YValues==HTuple()))))
  {
    //Neither XValues nor YValues are given:
    //Set axes to interval [0,1]
    hv_XAxisEndValue = 1;
    hv_YAxisEndValue = 1;
    hv_XAxisStartValue = 0;
    hv_YAxisStartValue = 0;
    hv_XValuesAreStrings = 0;
  }
  else
  {
    if (0 != (int(hv_XValues==HTuple())))
    {
      //XValues are omitted:
      //Set equidistant XValues
      hv_XValues = HTuple::TupleGenSequence(0,(hv_YValues.TupleLength())-1,1);
      hv_XValuesAreStrings = 0;
    }
    else if (0 != (int(hv_YValues==HTuple())))
    {
      //YValues are omitted:
      //Set equidistant YValues
      hv_YValues = HTuple::TupleGenSequence(0,(hv_XValues.TupleLength())-1,1);
    }
    if (0 != (int(((hv_YValues.TupleLength())%(hv_XValues.TupleLength()))!=0)))
    {
      //Number of YValues does not match number of XValues
      throw HException("Number of YValues is no multiple of the number of XValues!");
      return;
    }
    hv_XValuesAreStrings = hv_XValues.TupleIsStringElem();
    hv_XValuesAreStrings = int((hv_XValuesAreStrings.TupleSum())==(hv_XValuesAreStrings.TupleLength()));
    if (0 != hv_XValuesAreStrings)
    {
      //XValues are given as strings:
      //Show XValues as ticks
      hv_XTickValues = hv_XValues;
      hv_XTicks = 1;
      //Set x-axis dimensions
      hv_XValues = HTuple::TupleGenSequence(1,hv_XValues.TupleLength(),1);
    }
    //Set default x-axis dimensions
    if (0 != (int((hv_XValues.TupleLength())>1)))
    {
      hv_XAxisStartValue = hv_XValues.TupleMin();
      hv_XAxisEndValue = hv_XValues.TupleMax();
    }
    else
    {
      hv_XAxisEndValue = HTuple(hv_XValues[0])+0.5;
      hv_XAxisStartValue = HTuple(hv_XValues[0])-0.5;
    }
  }
  //Set default y-axis dimensions
  if (0 != (int((hv_YValues.TupleLength())>1)))
  {
    hv_YAxisStartValue = hv_YValues.TupleMin();
    hv_YAxisEndValue = hv_YValues.TupleMax();
  }
  else if (0 != (int((hv_YValues.TupleLength())==1)))
  {
    hv_YAxisStartValue = HTuple(hv_YValues[0])-0.5;
    hv_YAxisEndValue = HTuple(hv_YValues[0])+0.5;
  }
  else
  {
    hv_YAxisStartValue = 0;
    hv_YAxisEndValue = 1;
  }
  //Set default interception point of x- and y- axis
  hv_YAxisPosition = "default";
  hv_XAxisPosition = "default";
  //
  //Set more defaults
  hv_LeftBorder = hv_Width*0.1;
  hv_RightBorder = hv_Width*0.1;
  hv_UpperBorder = hv_Height*0.1;
  hv_LowerBorder = hv_Height*0.1;
  hv_AxesColor = "white";
  hv_Style = "line";
  hv_Clip = "no";
  hv_XTicks = "min_max_origin";
  hv_YTicks = "min_max_origin";
  hv_XGrid = "none";
  hv_YGrid = "none";
  hv_GridColor = "dim gray";
  hv_YPosition = "left";
  hv_FormatX = "default";
  hv_FormatY = "default";
  //
  //Parse generic parameters
  //
  hv_NumGenParamNames = hv_GenParamName.TupleLength();
  hv_NumGenParamValues = hv_GenParamValue.TupleLength();
  if (0 != (int(hv_NumGenParamNames!=hv_NumGenParamValues)))
  {
    throw HException("Number of generic parameter names does not match generic parameter values!");
    return;
  }
  //
  {
  HTuple end_val190 = (hv_GenParamName.TupleLength())-1;
  HTuple step_val190 = 1;
  for (hv_GenParamIndex=0; hv_GenParamIndex.Continue(end_val190, step_val190); hv_GenParamIndex += step_val190)
  {
    //
    //Set 'axes_color'
    if (0 != (int(HTuple(hv_GenParamName[hv_GenParamIndex])==HTuple("axes_color"))))
    {
      hv_AxesColor = HTuple(hv_GenParamValue[hv_GenParamIndex]);
      //
      //Set 'style'
    }
    else if (0 != (int(HTuple(hv_GenParamName[hv_GenParamIndex])==HTuple("style"))))
    {
      hv_Style = HTuple(hv_GenParamValue[hv_GenParamIndex]);
      //
      //Set 'clip'
    }
    else if (0 != (int(HTuple(hv_GenParamName[hv_GenParamIndex])==HTuple("clip"))))
    {
      hv_Clip = HTuple(hv_GenParamValue[hv_GenParamIndex]);
      if (0 != (HTuple(int(hv_Clip!=HTuple("yes"))).TupleAnd(int(hv_Clip!=HTuple("no")))))
      {
        throw HException(("Unsupported clipping option: '"+hv_Clip)+"'");
      }
      //
      //Set 'ticks'
    }
    else if (0 != (int(HTuple(hv_GenParamName[hv_GenParamIndex])==HTuple("ticks"))))
    {
      hv_XTicks = HTuple(hv_GenParamValue[hv_GenParamIndex]);
      hv_YTicks = HTuple(hv_GenParamValue[hv_GenParamIndex]);
      //
      //Set 'ticks_x'
    }
    else if (0 != (int(HTuple(hv_GenParamName[hv_GenParamIndex])==HTuple("ticks_x"))))
    {
      hv_XTicks = HTuple(hv_GenParamValue[hv_GenParamIndex]);
      //
      //Set 'ticks_y'
    }
    else if (0 != (int(HTuple(hv_GenParamName[hv_GenParamIndex])==HTuple("ticks_y"))))
    {
      hv_YTicks = HTuple(hv_GenParamValue[hv_GenParamIndex]);
      //
      //Set 'grid'
    }
    else if (0 != (int(HTuple(hv_GenParamName[hv_GenParamIndex])==HTuple("grid"))))
    {
      hv_XGrid = HTuple(hv_GenParamValue[hv_GenParamIndex]);
      hv_YGrid = HTuple(hv_GenParamValue[hv_GenParamIndex]);
      hv_XGridTicks = hv_XTicks;
      //
      //Set 'grid_x'
    }
    else if (0 != (int(HTuple(hv_GenParamName[hv_GenParamIndex])==HTuple("grid_x"))))
    {
      hv_XGrid = HTuple(hv_GenParamValue[hv_GenParamIndex]);
      //
      //Set 'grid_y'
    }
    else if (0 != (int(HTuple(hv_GenParamName[hv_GenParamIndex])==HTuple("grid_y"))))
    {
      hv_YGrid = HTuple(hv_GenParamValue[hv_GenParamIndex]);
      //
      //Set 'grid_color'
    }
    else if (0 != (int(HTuple(hv_GenParamName[hv_GenParamIndex])==HTuple("grid_color"))))
    {
      hv_GridColor = HTuple(hv_GenParamValue[hv_GenParamIndex]);
      //
      //Set 'start_x'
    }
    else if (0 != (int(HTuple(hv_GenParamName[hv_GenParamIndex])==HTuple("start_x"))))
    {
      hv_XAxisStartValue = HTuple(hv_GenParamValue[hv_GenParamIndex]);
      //
      //Set 'end_x'
    }
    else if (0 != (int(HTuple(hv_GenParamName[hv_GenParamIndex])==HTuple("end_x"))))
    {
      hv_XAxisEndValue = HTuple(hv_GenParamValue[hv_GenParamIndex]);
      //
      //Set 'start_y'
    }
    else if (0 != (int(HTuple(hv_GenParamName[hv_GenParamIndex])==HTuple("start_y"))))
    {
      hv_YAxisStartValue = HTuple(hv_GenParamValue[hv_GenParamIndex]);
      //
      //Set 'end_y'
    }
    else if (0 != (int(HTuple(hv_GenParamName[hv_GenParamIndex])==HTuple("end_y"))))
    {
      hv_YAxisEndValue = HTuple(hv_GenParamValue[hv_GenParamIndex]);
      //
      //Set 'axis_location_y' (old name 'origin_x')
    }
    else if (0 != (HTuple(int(HTuple(hv_GenParamName[hv_GenParamIndex])==HTuple("axis_location_y"))).TupleOr(int(HTuple(hv_GenParamName[hv_GenParamIndex])==HTuple("origin_x")))))
    {
      hv_YAxisPosition = HTuple(hv_GenParamValue[hv_GenParamIndex]);
      //
      //Set 'axis_location_x' (old name: 'origin_y')
    }
    else if (0 != (HTuple(int(HTuple(hv_GenParamName[hv_GenParamIndex])==HTuple("axis_location_x"))).TupleOr(int(HTuple(hv_GenParamName[hv_GenParamIndex])==HTuple("origin_y")))))
    {
      hv_XAxisPosition = HTuple(hv_GenParamValue[hv_GenParamIndex]);
      //
      //Set 'margin'
    }
    else if (0 != (int(HTuple(hv_GenParamName[hv_GenParamIndex])==HTuple("margin"))))
    {
      hv_LeftBorder = HTuple(hv_GenParamValue[hv_GenParamIndex]);
      hv_RightBorder = HTuple(hv_GenParamValue[hv_GenParamIndex]);
      hv_UpperBorder = HTuple(hv_GenParamValue[hv_GenParamIndex]);
      hv_LowerBorder = HTuple(hv_GenParamValue[hv_GenParamIndex]);
      //
      //Set 'margin_left'
    }
    else if (0 != (int(HTuple(hv_GenParamName[hv_GenParamIndex])==HTuple("margin_left"))))
    {
      hv_LeftBorder = HTuple(hv_GenParamValue[hv_GenParamIndex]);
      //
      //Set 'margin_right'
    }
    else if (0 != (int(HTuple(hv_GenParamName[hv_GenParamIndex])==HTuple("margin_right"))))
    {
      hv_RightBorder = HTuple(hv_GenParamValue[hv_GenParamIndex]);
      //
      //Set 'margin_top'
    }
    else if (0 != (int(HTuple(hv_GenParamName[hv_GenParamIndex])==HTuple("margin_top"))))
    {
      hv_UpperBorder = HTuple(hv_GenParamValue[hv_GenParamIndex]);
      //
      //Set 'margin_bottom'
    }
    else if (0 != (int(HTuple(hv_GenParamName[hv_GenParamIndex])==HTuple("margin_bottom"))))
    {
      hv_LowerBorder = HTuple(hv_GenParamValue[hv_GenParamIndex]);
    }
    else if (0 != (int(HTuple(hv_GenParamName[hv_GenParamIndex])==HTuple("format_x"))))
    {
      hv_FormatX = HTuple(hv_GenParamValue[hv_GenParamIndex]);
    }
    else if (0 != (int(HTuple(hv_GenParamName[hv_GenParamIndex])==HTuple("format_y"))))
    {
      hv_FormatY = HTuple(hv_GenParamValue[hv_GenParamIndex]);
    }
    else
    {
      throw HException(("Unknown generic parameter: '"+HTuple(hv_GenParamName[hv_GenParamIndex]))+"'");
    }
  }
  }
  //
  //Check consistency of start and end values
  //of the axes.
  if (0 != (int(hv_XAxisStartValue>hv_XAxisEndValue)))
  {
    throw HException("Value for 'start_x' is greater than value for 'end_x'");
  }
  if (0 != (int(hv_YAxisStartValue>hv_YAxisEndValue)))
  {
    throw HException("Value for 'start_y' is greater than value for 'end_y'");
  }
  //
  //Set the position of the y-axis.
  if (0 != (int(hv_YAxisPosition==HTuple("default"))))
  {
    hv_YAxisPosition = hv_XAxisStartValue;
  }
  if (0 != (int((hv_YAxisPosition.TupleIsString())==1)))
  {
    if (0 != (int(hv_YAxisPosition==HTuple("left"))))
    {
      hv_YAxisPosition = hv_XAxisStartValue;
    }
    else if (0 != (int(hv_YAxisPosition==HTuple("right"))))
    {
      hv_YAxisPosition = hv_XAxisEndValue;
    }
    else if (0 != (int(hv_YAxisPosition==HTuple("origin"))))
    {
      hv_YAxisPosition = 0;
    }
    else
    {
      throw HException(("Unsupported axis_location_y: '"+hv_YAxisPosition)+"'");
    }
  }
  //Set the position of the ticks on the y-axis
  //depending of the location of the y-axis.
  if (0 != (int(((hv_XAxisStartValue.TupleConcat(hv_XAxisEndValue)).TupleMean())>hv_YAxisPosition)))
  {
    hv_YTickDirection = "right";
  }
  else
  {
    hv_YTickDirection = "left";
  }
  //
  //Set the position of the x-axis.
  if (0 != (int(hv_XAxisPosition==HTuple("default"))))
  {
    hv_XAxisPosition = hv_YAxisStartValue;
  }
  if (0 != (int((hv_XAxisPosition.TupleIsString())==1)))
  {
    if (0 != (int(hv_XAxisPosition==HTuple("bottom"))))
    {
      hv_XAxisPosition = hv_YAxisStartValue;
    }
    else if (0 != (int(hv_XAxisPosition==HTuple("top"))))
    {
      hv_XAxisPosition = hv_YAxisEndValue;
    }
    else if (0 != (int(hv_XAxisPosition==HTuple("origin"))))
    {
      hv_XAxisPosition = 0;
    }
    else
    {
      throw HException(("Unsupported axis_location_x: '"+hv_XAxisPosition)+"'");
    }
  }
  //Set the position of the ticks on the y-axis
  //depending of the location of the y-axis.
  if (0 != (int(((hv_YAxisStartValue.TupleConcat(hv_YAxisEndValue)).TupleMean())>hv_XAxisPosition)))
  {
    hv_XTickDirection = "up";
  }
  else
  {
    hv_XTickDirection = "down";
  }
  //
  //Calculate basic pixel coordinates and scale factors
  //
  hv_XAxisWidthPx = (hv_Width-hv_LeftBorder)-hv_RightBorder;
  hv_XAxisWidth = hv_XAxisEndValue-hv_XAxisStartValue;
  if (0 != (int(hv_XAxisWidth==0)))
  {
    hv_XAxisStartValue = hv_XAxisStartValue-0.5;
    hv_XAxisEndValue += 0.5;
    hv_XAxisWidth = 1;
  }
  hv_XScaleFactor = hv_XAxisWidthPx/(hv_XAxisWidth.TupleReal());
  hv_YAxisHeightPx = (hv_Height-hv_LowerBorder)-hv_UpperBorder;
  hv_YAxisHeight = hv_YAxisEndValue-hv_YAxisStartValue;
  if (0 != (int(hv_YAxisHeight==0)))
  {
    hv_YAxisStartValue = hv_YAxisStartValue-0.5;
    hv_YAxisEndValue += 0.5;
    hv_YAxisHeight = 1;
  }
  hv_YScaleFactor = hv_YAxisHeightPx/(hv_YAxisHeight.TupleReal());
  hv_YAxisOffsetPx = (hv_YAxisPosition-hv_XAxisStartValue)*hv_XScaleFactor;
  hv_XAxisOffsetPx = (hv_XAxisPosition-hv_YAxisStartValue)*hv_YScaleFactor;
  //
  //Display grid lines
  //
  if (0 != (int(hv_GridColor!=HTuple("none"))))
  {
    hv_DotStyle.Clear();
    hv_DotStyle[0] = 5;
    hv_DotStyle[1] = 7;
    SetLineStyle(hv_WindowHandle, hv_DotStyle);
    if (HDevWindowStack::IsOpen())
      SetColor(HDevWindowStack::GetActive(),hv_GridColor);
    //
    //Display x grid lines
    if (0 != (int(hv_XGrid!=HTuple("none"))))
    {
      if (0 != (int(hv_XGrid==HTuple("min_max_origin"))))
      {
        //Calculate 'min_max_origin' grid line coordinates
        if (0 != (int(hv_YAxisPosition==hv_XAxisStartValue)))
        {
          hv_XGridValues.Clear();
          hv_XGridValues.Append(hv_XAxisStartValue);
          hv_XGridValues.Append(hv_XAxisEndValue);
        }
        else
        {
          hv_XGridValues.Clear();
          hv_XGridValues.Append(hv_XAxisStartValue);
          hv_XGridValues.Append(hv_YAxisPosition);
          hv_XGridValues.Append(hv_XAxisEndValue);
        }
      }
      else
      {
        //Calculate equidistant grid line coordinates
        hv_XGridStart = ((hv_XAxisStartValue/hv_XGrid).TupleCeil())*hv_XGrid;
        hv_XGridValues = HTuple::TupleGenSequence(hv_XGridStart,hv_XAxisEndValue,hv_XGrid);
      }
      hv_XCoord = (hv_XGridValues-hv_XAxisStartValue)*hv_XScaleFactor;
      //Generate and display grid lines
      {
      HTuple end_val392 = (hv_XGridValues.TupleLength())-1;
      HTuple step_val392 = 1;
      for (hv_IndexGrid=0; hv_IndexGrid.Continue(end_val392, step_val392); hv_IndexGrid += step_val392)
      {
        GenContourPolygonXld(&ho_ContourXGrid, (hv_Height-hv_LowerBorder).TupleConcat(hv_UpperBorder), 
            (hv_LeftBorder+HTuple(hv_XCoord[hv_IndexGrid])).TupleConcat(hv_LeftBorder+HTuple(hv_XCoord[hv_IndexGrid])));
        if (HDevWindowStack::IsOpen())
          DispObj(ho_ContourXGrid, HDevWindowStack::GetActive());
      }
      }
    }
    //
    //Display y grid lines
    if (0 != (int(hv_YGrid!=HTuple("none"))))
    {
      if (0 != (int(hv_YGrid==HTuple("min_max_origin"))))
      {
        //Calculate 'min_max_origin' grid line coordinates
        if (0 != (int(hv_XAxisPosition==hv_YAxisStartValue)))
        {
          hv_YGridValues.Clear();
          hv_YGridValues.Append(hv_YAxisStartValue);
          hv_YGridValues.Append(hv_YAxisEndValue);
        }
        else
        {
          hv_YGridValues.Clear();
          hv_YGridValues.Append(hv_YAxisStartValue);
          hv_YGridValues.Append(hv_XAxisPosition);
          hv_YGridValues.Append(hv_YAxisEndValue);
        }
      }
      else
      {
        //Calculate equidistant grid line coordinates
        hv_YGridStart = ((hv_YAxisStartValue/hv_YGrid).TupleCeil())*hv_YGrid;
        hv_YGridValues = HTuple::TupleGenSequence(hv_YGridStart,hv_YAxisEndValue,hv_YGrid);
      }
      hv_YCoord = (hv_YGridValues-hv_YAxisStartValue)*hv_YScaleFactor;
      //Generate and display grid lines
      {
      HTuple end_val414 = (hv_YGridValues.TupleLength())-1;
      HTuple step_val414 = 1;
      for (hv_IndexGrid=0; hv_IndexGrid.Continue(end_val414, step_val414); hv_IndexGrid += step_val414)
      {
        GenContourPolygonXld(&ho_ContourYGrid, ((hv_Height-hv_LowerBorder)-HTuple(hv_YCoord[hv_IndexGrid])).TupleConcat((hv_Height-hv_LowerBorder)-HTuple(hv_YCoord[hv_IndexGrid])), 
            hv_LeftBorder.TupleConcat(hv_Width-hv_RightBorder));
        if (HDevWindowStack::IsOpen())
          DispObj(ho_ContourYGrid, HDevWindowStack::GetActive());
      }
      }
    }
  }
  SetLineStyle(hv_WindowHandle, HTuple());
  //
  //
  //Display the coordinate system axes
  if (0 != (int(hv_AxesColor!=HTuple("none"))))
  {
    //Display axes
    if (HDevWindowStack::IsOpen())
      SetColor(HDevWindowStack::GetActive(),hv_AxesColor);
    gen_arrow_contour_xld(&ho_XArrow, (hv_Height-hv_LowerBorder)-hv_XAxisOffsetPx, 
        hv_LeftBorder, (hv_Height-hv_LowerBorder)-hv_XAxisOffsetPx, hv_Width-hv_RightBorder, 
        0, 0);
    if (HDevWindowStack::IsOpen())
      DispObj(ho_XArrow, HDevWindowStack::GetActive());
    gen_arrow_contour_xld(&ho_YArrow, hv_Height-hv_LowerBorder, hv_LeftBorder+hv_YAxisOffsetPx, 
        hv_UpperBorder, hv_LeftBorder+hv_YAxisOffsetPx, 0, 0);
    if (HDevWindowStack::IsOpen())
      DispObj(ho_YArrow, HDevWindowStack::GetActive());
    //Display labels
    GetStringExtents(hv_WindowHandle, hv_XLabel, &hv_Ascent, &hv_Descent, &hv_TextWidthXLabel, 
        &hv_TextHeightXLabel);
    GetStringExtents(hv_WindowHandle, hv_YLabel, &hv_Ascent, &hv_Descent, &hv_TextWidthYLabel, 
        &hv_TextHeightYLabel);
    if (0 != (int(hv_YTickDirection==HTuple("right"))))
    {
      if (0 != (int(hv_XTickDirection==HTuple("up"))))
      {
        if (HDevWindowStack::IsOpen())
          DispText(HDevWindowStack::GetActive(),hv_XLabel, "image", ((hv_Height-hv_LowerBorder)-hv_TextHeightXLabel)-3, 
              ((hv_Width-hv_RightBorder)-hv_TextWidthXLabel)-3, hv_AxesColor, "box", 
              "false");
        if (HDevWindowStack::IsOpen())
          DispText(HDevWindowStack::GetActive()," "+hv_YLabel, "image", hv_UpperBorder, 
              (hv_LeftBorder+3)+hv_YAxisOffsetPx, hv_AxesColor, "box", "false");
      }
      else
      {
        if (HDevWindowStack::IsOpen())
          DispText(HDevWindowStack::GetActive(),hv_XLabel, "image", ((hv_Height-hv_LowerBorder)+3)-hv_XAxisOffsetPx, 
              ((hv_Width-hv_RightBorder)-hv_TextWidthXLabel)-3, hv_AxesColor, "box", 
              "false");
        if (HDevWindowStack::IsOpen())
          DispText(HDevWindowStack::GetActive()," "+hv_YLabel, "image", ((hv_Height-hv_LowerBorder)-hv_TextHeightXLabel)-3, 
              (hv_LeftBorder+3)+hv_YAxisOffsetPx, hv_AxesColor, "box", "false");
      }
    }
    else
    {
      if (0 != (int(hv_XTickDirection==HTuple("up"))))
      {
        if (HDevWindowStack::IsOpen())
          DispText(HDevWindowStack::GetActive(),hv_XLabel, "image", ((hv_Height-hv_LowerBorder)-(2*hv_TextHeightXLabel))+3, 
              hv_LeftBorder-3, hv_AxesColor, "box", "false");
        if (HDevWindowStack::IsOpen())
          DispText(HDevWindowStack::GetActive()," "+hv_YLabel, "image", hv_UpperBorder, 
              ((hv_Width-hv_RightBorder)-hv_TextWidthYLabel)-13, hv_AxesColor, "box", 
              "false");
      }
      else
      {
        if (HDevWindowStack::IsOpen())
          DispText(HDevWindowStack::GetActive(),hv_XLabel, "image", ((hv_Height-hv_LowerBorder)+3)-hv_XAxisOffsetPx, 
              hv_LeftBorder-3, hv_AxesColor, "box", "false");
        if (HDevWindowStack::IsOpen())
          DispText(HDevWindowStack::GetActive()," "+hv_YLabel, "image", ((hv_Height-hv_LowerBorder)-hv_TextHeightXLabel)-3, 
              ((hv_Width-hv_RightBorder)-(2*hv_TextWidthYLabel))-3, hv_AxesColor, 
              "box", "false");
      }
    }
  }
  //
  //Display ticks
  //
  if (0 != (int(hv_AxesColor!=HTuple("none"))))
  {
    if (HDevWindowStack::IsOpen())
      SetColor(HDevWindowStack::GetActive(),hv_AxesColor);
    if (0 != (int(hv_XTicks!=HTuple("none"))))
    {
      //
      //Display x ticks
      if (0 != hv_XValuesAreStrings)
      {
        //Display string XValues as categories
        hv_XTicks = (hv_XValues.TupleLength())/(hv_XTickValues.TupleLength());
        hv_XCoord = (hv_XValues-hv_XAxisStartValue)*hv_XScaleFactor;
      }
      else
      {
        //Display tick values
        if (0 != (int(hv_XTicks==HTuple("min_max_origin"))))
        {
          //Calculate 'min_max_origin' tick coordinates
          if (0 != (int(hv_YAxisPosition==hv_XAxisStartValue)))
          {
            hv_XTickValues.Clear();
            hv_XTickValues.Append(hv_XAxisStartValue);
            hv_XTickValues.Append(hv_XAxisEndValue);
          }
          else
          {
            hv_XTickValues.Clear();
            hv_XTickValues.Append(hv_XAxisStartValue);
            hv_XTickValues.Append(hv_YAxisPosition);
            hv_XTickValues.Append(hv_XAxisEndValue);
          }
        }
        else
        {
          //Calculate equidistant tick coordinates
          hv_XTickStart = ((hv_XAxisStartValue/hv_XTicks).TupleCeil())*hv_XTicks;
          hv_XTickValues = HTuple::TupleGenSequence(hv_XTickStart,hv_XAxisEndValue,hv_XTicks);
        }
        //Remove ticks that are smaller than the x-axis start.
        hv_Indices = (hv_XTickValues.TupleLessElem(hv_XAxisStartValue)).TupleFind(1);
        hv_XCoord = (hv_XTickValues-hv_XAxisStartValue)*hv_XScaleFactor;
        hv_XCoord = hv_XCoord.TupleRemove(hv_Indices);
        hv_XTickValues = hv_XTickValues.TupleRemove(hv_Indices);
        //
        if (0 != (int(hv_FormatX==HTuple("default"))))
        {
          hv_TypeTicks = hv_XTicks.TupleType();
          if (0 != (int(hv_TypeTicks==4)))
          {
            //String ('min_max_origin')
            //Format depends on actual values
            hv_TypeTicks = hv_XTickValues.TupleType();
          }
          if (0 != (int(hv_TypeTicks==1)))
          {
            //Round to integer
            hv_XTickValues = hv_XTickValues.TupleInt();
          }
          else
          {
            //Use floating point numbers
            hv_XTickValues = hv_XTickValues.TupleString(".2f");
          }
        }
        else
        {
          hv_XTickValues = hv_XTickValues.TupleString(hv_FormatX);
        }
      }
      //Generate and display ticks
      {
      HTuple end_val503 = (hv_XTickValues.TupleLength())-1;
      HTuple step_val503 = 1;
      for (hv_IndexTicks=0; hv_IndexTicks.Continue(end_val503, step_val503); hv_IndexTicks += step_val503)
      {
        GetStringExtents(hv_WindowHandle, HTuple(hv_XTickValues[hv_IndexTicks]), 
            &hv_Ascent1, &hv_Descent1, &hv_TextWidthXTicks, &hv_TextHeightXTicks);
        if (0 != (int(hv_XTickDirection==HTuple("up"))))
        {
          GenContourPolygonXld(&ho_ContourXTick, ((hv_Height-hv_LowerBorder)-hv_XAxisOffsetPx).TupleConcat(((hv_Height-hv_LowerBorder)-hv_XAxisOffsetPx)-5), 
              (hv_LeftBorder+HTuple(hv_XCoord[hv_IndexTicks])).TupleConcat(hv_LeftBorder+HTuple(hv_XCoord[hv_IndexTicks])));
          if (HDevWindowStack::IsOpen())
            DispText(HDevWindowStack::GetActive(),HTuple(hv_XTickValues[hv_IndexTicks]), 
                "image", ((hv_Height-hv_LowerBorder)+2)-hv_XAxisOffsetPx, hv_LeftBorder+HTuple(hv_XCoord[hv_IndexTicks]), 
                hv_AxesColor, "box", "false");
        }
        else
        {
          GenContourPolygonXld(&ho_ContourXTick, (((hv_Height-hv_LowerBorder)-hv_XAxisOffsetPx)+5).TupleConcat((hv_Height-hv_LowerBorder)-hv_XAxisOffsetPx), 
              (hv_LeftBorder+HTuple(hv_XCoord[hv_IndexTicks])).TupleConcat(hv_LeftBorder+HTuple(hv_XCoord[hv_IndexTicks])));
          if (HDevWindowStack::IsOpen())
            DispText(HDevWindowStack::GetActive(),HTuple(hv_XTickValues[hv_IndexTicks]), 
                "image", ((hv_Height-hv_LowerBorder)-(2*hv_TextHeightXTicks))-hv_XAxisOffsetPx, 
                hv_LeftBorder+HTuple(hv_XCoord[hv_IndexTicks]), hv_AxesColor, "box", 
                "false");
        }
        if (HDevWindowStack::IsOpen())
          DispObj(ho_ContourXTick, HDevWindowStack::GetActive());
      }
      }
    }
    //
    if (0 != (int(hv_YTicks!=HTuple("none"))))
    {
      //
      //Display y ticks
      if (0 != (int(hv_YTicks==HTuple("min_max_origin"))))
      {
        //Calculate 'min_max_origin' tick coordinates
        if (0 != (int(hv_XAxisPosition==hv_YAxisStartValue)))
        {
          hv_YTickValues.Clear();
          hv_YTickValues.Append(hv_YAxisStartValue);
          hv_YTickValues.Append(hv_YAxisEndValue);
        }
        else
        {
          hv_YTickValues.Clear();
          hv_YTickValues.Append(hv_YAxisStartValue);
          hv_YTickValues.Append(hv_XAxisPosition);
          hv_YTickValues.Append(hv_YAxisEndValue);
        }
      }
      else
      {
        //Calculate equidistant tick coordinates
        hv_YTickStart = ((hv_YAxisStartValue/hv_YTicks).TupleCeil())*hv_YTicks;
        hv_YTickValues = HTuple::TupleGenSequence(hv_YTickStart,hv_YAxisEndValue,hv_YTicks);
      }
      //Remove ticks that are smaller than the y-axis start.
      hv_Indices = (hv_YTickValues.TupleLessElem(hv_YAxisStartValue)).TupleFind(1);
      hv_YCoord = (hv_YTickValues-hv_YAxisStartValue)*hv_YScaleFactor;
      hv_YCoord = hv_YCoord.TupleRemove(hv_Indices);
      hv_YTickValues = hv_YTickValues.TupleRemove(hv_Indices);
      //
      if (0 != (int(hv_FormatY==HTuple("default"))))
      {
        hv_TypeTicks = hv_YTicks.TupleType();
        if (0 != (int(hv_TypeTicks==4)))
        {
          //String ('min_max_origin')
          //Format depends on actual values
          hv_TypeTicks = hv_YTickValues.TupleType();
        }
        if (0 != (int(hv_TypeTicks==1)))
        {
          //Round to integer
          hv_YTickValues = hv_YTickValues.TupleInt();
        }
        else
        {
          //Use floating point numbers
          hv_YTickValues = hv_YTickValues.TupleString(".2f");
        }
      }
      else
      {
        hv_YTickValues = hv_YTickValues.TupleString(hv_FormatY);
      }
      //Generate and display ticks
      {
      HTuple end_val555 = (hv_YTickValues.TupleLength())-1;
      HTuple step_val555 = 1;
      for (hv_IndexTicks=0; hv_IndexTicks.Continue(end_val555, step_val555); hv_IndexTicks += step_val555)
      {
        GetStringExtents(hv_WindowHandle, HTuple(hv_YTickValues[hv_IndexTicks]), 
            &hv_Ascent1, &hv_Descent1, &hv_TextWidthYTicks, &hv_TextHeightYTicks);
        if (0 != (int(hv_YTickDirection==HTuple("right"))))
        {
          GenContourPolygonXld(&ho_ContourYTick, ((hv_Height-hv_LowerBorder)-HTuple(hv_YCoord[hv_IndexTicks])).TupleConcat((hv_Height-hv_LowerBorder)-HTuple(hv_YCoord[hv_IndexTicks])), 
              (hv_LeftBorder+hv_YAxisOffsetPx).TupleConcat((hv_LeftBorder+hv_YAxisOffsetPx)+5));
          if (HDevWindowStack::IsOpen())
            DispText(HDevWindowStack::GetActive(),HTuple(hv_YTickValues[hv_IndexTicks]), 
                "image", (((hv_Height-hv_LowerBorder)-hv_TextHeightYTicks)+3)-HTuple(hv_YCoord[hv_IndexTicks]), 
                ((hv_LeftBorder-hv_TextWidthYTicks)-2)+hv_YAxisOffsetPx, hv_AxesColor, 
                "box", "false");
        }
        else
        {
          GenContourPolygonXld(&ho_ContourYTick, ((hv_Height-hv_LowerBorder)-HTuple(hv_YCoord[hv_IndexTicks])).TupleConcat((hv_Height-hv_LowerBorder)-HTuple(hv_YCoord[hv_IndexTicks])), 
              ((hv_LeftBorder+hv_YAxisOffsetPx)-5).TupleConcat(hv_LeftBorder+hv_YAxisOffsetPx));
          if (HDevWindowStack::IsOpen())
            DispText(HDevWindowStack::GetActive(),HTuple(hv_YTickValues[hv_IndexTicks]), 
                "image", (((hv_Height-hv_LowerBorder)-hv_TextHeightYTicks)+3)-HTuple(hv_YCoord[hv_IndexTicks]), 
                (hv_LeftBorder+2)+hv_YAxisOffsetPx, hv_AxesColor, "box", "false");
        }
        if (HDevWindowStack::IsOpen())
          DispObj(ho_ContourYTick, HDevWindowStack::GetActive());
      }
      }
    }
  }
  //
  //Display function plot
  //
  if (0 != (int(hv_Color!=HTuple("none"))))
  {
    if (0 != (HTuple(int(hv_XValues!=HTuple())).TupleAnd(int(hv_YValues!=HTuple()))))
    {
      hv_Num = (hv_YValues.TupleLength())/(hv_XValues.TupleLength());
      //
      //Iterate over all functions to be displayed
      {
      HTuple end_val576 = hv_Num-1;
      HTuple step_val576 = 1;
      for (hv_I=0; hv_I.Continue(end_val576, step_val576); hv_I += step_val576)
      {
        //Select y values for current function
        hv_YSelected = hv_YValues.TupleSelectRange(hv_I*(hv_XValues.TupleLength()),((hv_I+1)*(hv_XValues.TupleLength()))-1);
        //Set color
        if (0 != (int(hv_Color==HTuple())))
        {
          SetRgb(hv_WindowHandle, hv_Red, hv_Green, hv_Blue);
        }
        else
        {
          if (HDevWindowStack::IsOpen())
            SetColor(HDevWindowStack::GetActive(),HTuple(hv_Color[hv_I%(hv_Color.TupleLength())]));
        }
        //
        //Display in different styles
        //
        if (0 != (HTuple(int(hv_Style==HTuple("line"))).TupleOr(int(hv_Style==HTuple()))))
        {
          //Line
          GenContourPolygonXld(&ho_Contour, ((hv_Height-hv_LowerBorder)-(hv_YSelected*hv_YScaleFactor))+(hv_YAxisStartValue*hv_YScaleFactor), 
              ((hv_XValues*hv_XScaleFactor)+hv_LeftBorder)-(hv_XAxisStartValue*hv_XScaleFactor));
          //Clip, if necessary
          if (0 != (int(hv_Clip==HTuple("yes"))))
          {
            ClipContoursXld(ho_Contour, &ho_Contour, hv_UpperBorder, hv_LeftBorder, 
                hv_Height-hv_LowerBorder, hv_Width-hv_RightBorder);
          }
          if (HDevWindowStack::IsOpen())
            DispObj(ho_Contour, HDevWindowStack::GetActive());
        }
        else if (0 != (int(hv_Style==HTuple("cross"))))
        {
          //Cross
          GenCrossContourXld(&ho_Cross, ((hv_Height-hv_LowerBorder)-(hv_YSelected*hv_YScaleFactor))+(hv_YAxisStartValue*hv_YScaleFactor), 
              ((hv_XValues*hv_XScaleFactor)+hv_LeftBorder)-(hv_XAxisStartValue*hv_XScaleFactor), 
              6, 0.785398);
          //Clip, if necessary
          if (0 != (int(hv_Clip==HTuple("yes"))))
          {
            ClipContoursXld(ho_Cross, &ho_Cross, hv_UpperBorder, hv_LeftBorder, hv_Height-hv_LowerBorder, 
                hv_Width-hv_RightBorder);
          }
          if (HDevWindowStack::IsOpen())
            DispObj(ho_Cross, HDevWindowStack::GetActive());
        }
        else if (0 != (int(hv_Style==HTuple("filled"))))
        {
          //Filled
          hv_Y1Selected.Clear();
          hv_Y1Selected.Append(0+hv_XAxisPosition);
          hv_Y1Selected.Append(hv_YSelected);
          hv_Y1Selected.Append(0+hv_XAxisPosition);
          hv_X1Selected.Clear();
          hv_X1Selected.Append(hv_XValues.TupleMin());
          hv_X1Selected.Append(hv_XValues);
          hv_X1Selected.Append(hv_XValues.TupleMax());
          if (HDevWindowStack::IsOpen())
            SetDraw(HDevWindowStack::GetActive(),"fill");
          GenRegionPolygonFilled(&ho_Filled, ((hv_Height-hv_LowerBorder)-(hv_Y1Selected*hv_YScaleFactor))+(hv_YAxisStartValue*hv_YScaleFactor), 
              ((hv_X1Selected*hv_XScaleFactor)+hv_LeftBorder)-(hv_XAxisStartValue*hv_XScaleFactor));
          //Clip, if necessary
          if (0 != (int(hv_Clip==HTuple("yes"))))
          {
            ClipRegion(ho_Filled, &ho_Filled, hv_UpperBorder, hv_LeftBorder, hv_Height-hv_LowerBorder, 
                hv_Width-hv_RightBorder);
          }
          if (HDevWindowStack::IsOpen())
            DispObj(ho_Filled, HDevWindowStack::GetActive());
        }
        else if (0 != (int(hv_Style==HTuple("step"))))
        {
          GenEmptyObj(&ho_Stair);
          {
          HTuple end_val617 = (hv_XValues.TupleLength())-2;
          HTuple step_val617 = 1;
          for (hv_Index=0; hv_Index.Continue(end_val617, step_val617); hv_Index += step_val617)
          {
            hv_Row1 = ((hv_Height-hv_LowerBorder)-(HTuple(hv_YSelected[hv_Index])*hv_YScaleFactor))+(hv_YAxisStartValue*hv_YScaleFactor);
            hv_Row2 = ((hv_Height-hv_LowerBorder)-(HTuple(hv_YSelected[hv_Index+1])*hv_YScaleFactor))+(hv_YAxisStartValue*hv_YScaleFactor);
            hv_Col1 = ((HTuple(hv_XValues[hv_Index])*hv_XScaleFactor)+hv_LeftBorder)-(hv_XAxisStartValue*hv_XScaleFactor);
            hv_Col2 = ((HTuple(hv_XValues[hv_Index+1])*hv_XScaleFactor)+hv_LeftBorder)-(hv_XAxisStartValue*hv_XScaleFactor);
            GenContourPolygonXld(&ho_StairTmp, (hv_Row1.TupleConcat(hv_Row1)).TupleConcat(hv_Row2), 
                (hv_Col1.TupleConcat(hv_Col2)).TupleConcat(hv_Col2));
            ConcatObj(ho_Stair, ho_StairTmp, &ho_Stair);
          }
          }
          UnionAdjacentContoursXld(ho_Stair, &ho_Stair, 0.1, 0.1, "attr_keep");
          if (0 != (int(hv_Clip==HTuple("yes"))))
          {
            ClipContoursXld(ho_Stair, &ho_Stair, hv_UpperBorder, hv_LeftBorder, hv_Height-hv_LowerBorder, 
                hv_Width-hv_RightBorder);
          }
          if (HDevWindowStack::IsOpen())
            DispObj(ho_Stair, HDevWindowStack::GetActive());
        }
        else
        {
          throw HException("Unsupported style: "+hv_Style);
        }
      }
      }
    }
  }
  //
  //
  //Reset original display settings
  if (HDevWindowStack::IsOpen())
    SetPart(HDevWindowStack::GetActive(),hv_PartRow1, hv_PartColumn1, hv_PartRow2, 
        hv_PartColumn2);
  HDevWindowStack::SetActive(hv_PreviousWindowHandle);
  SetRgb(hv_WindowHandle, hv_Red, hv_Green, hv_Blue);
  if (HDevWindowStack::IsOpen())
    SetDraw(HDevWindowStack::GetActive(),hv_DrawMode);
  SetLineStyle(hv_WindowHandle, hv_OriginStyle);
  SetSystem("clip_region", hv_ClipRegion);
  return;
}

// Chapter: Graphics / Output
// Short Description: This procedure plots tuples representing functions or curves in a coordinate system. 
void plot_tuple_no_window_handling (HTuple hv_WindowHandle, HTuple hv_XValues, HTuple hv_YValues, 
    HTuple hv_XLabel, HTuple hv_YLabel, HTuple hv_Color, HTuple hv_GenParamName, 
    HTuple hv_GenParamValue)
{

  // Local iconic variables
  HObject  ho_ContourXGrid, ho_ContourYGrid, ho_XArrow;
  HObject  ho_YArrow, ho_ContourXTick, ho_ContourYTick, ho_Contour;
  HObject  ho_Cross, ho_Circle, ho_Filled, ho_Stair, ho_StairTmp;

  // Local control variables
  HTuple  hv_ClipRegion, hv_Row, hv_Column, hv_Width;
  HTuple  hv_Height, hv_PartRow1, hv_PartColumn1, hv_PartRow2;
  HTuple  hv_PartColumn2, hv_Red, hv_Green, hv_Blue, hv_DrawMode;
  HTuple  hv_OriginStyle, hv_PartDiffers, hv_PlotYLog, hv_YLogIndices;
  HTuple  hv_PlotYLogUser, hv_IsString, hv_YInd, hv_Indices1;
  HTuple  hv_XAxisEndValue, hv_YAxisEndValue, hv_XAxisStartValue;
  HTuple  hv_YAxisStartValue, hv_XValuesAreStrings, hv_XTickValues;
  HTuple  hv_XTicks, hv_YAxisPosition, hv_XAxisPosition, hv_LeftBorder;
  HTuple  hv_RightBorder, hv_UpperBorder, hv_LowerBorder;
  HTuple  hv_AxesColor, hv_Style, hv_Clip, hv_YTicks, hv_XGrid;
  HTuple  hv_YGrid, hv_GridColor, hv_YPosition, hv_FormatX;
  HTuple  hv_FormatY, hv_LineWidth, hv_NumGenParamNames, hv_NumGenParamValues;
  HTuple  hv_GenParamIndex, hv_XGridTicks, hv_YTickDirection;
  HTuple  hv_XTickDirection, hv_XAxisWidthPx, hv_XAxisWidth;
  HTuple  hv_XScaleFactor, hv_YAxisHeightPx, hv_YAxisHeight;
  HTuple  hv_YScaleFactor, hv_YAxisOffsetPx, hv_XAxisOffsetPx;
  HTuple  hv_DotStyle, hv_XGridValues, hv_XGridStart, hv_XCoord;
  HTuple  hv_IndexGrid, hv_YGridValues, hv_YGridStart, hv_YCoord;
  HTuple  hv_Ascent, hv_Descent, hv_TextWidthXLabel, hv_TextHeightXLabel;
  HTuple  hv_TextWidthYLabel, hv_TextHeightYLabel, hv_XTickStart;
  HTuple  hv_Indices, hv_TypeTicks, hv_IndexTicks, hv_Ascent1;
  HTuple  hv_Descent1, hv_TextWidthXTicks, hv_TextHeightXTicks;
  HTuple  hv_YTickValues, hv_YTickStart, hv_TextWidthYTicks;
  HTuple  hv_TextHeightYTicks, hv_Num, hv_I, hv_YSelected;
  HTuple  hv_StyleOriginal, hv_OldLineWidth, hv_Radii, hv_OldContourStyle;
  HTuple  hv_Y1Selected, hv_X1Selected, hv_Index, hv_Row1;
  HTuple  hv_Row2, hv_Col1, hv_Col2;

  //
  //This procedure plots tuples representing functions
  //or curves in a coordinate system.

  //In the following, the possible values are listed for the parameters:
  //
  //- XValues: X values of the function to be plotted. Thereby you have the following options:
  //  -- []: XValues are internally set to 0,1,2,...,|YValues|-1.
  //  -- a tuple of strings: These values are taken as categories.
  //
  //- YValues: Y values of the function(s) to be plotted. Thereby you have the following options:
  //  -- []: YValues are internally set to 0,1,2,...,|XValues|-1.
  //  -- a tuple of values: The number of y values must be equal to the number of x values or an integral multiple.
  //     In the latter case, multiple functions are plotted, that share the same x values.
  //
  //- XLabel: X-axis label.
  //
  //- YLabel: Y-axis label.
  //
  //- Color: Color of the plotted function. Thereby you have the following options:
  //  -- []: The currently set display color is used.
  //  -- 'none': The function is not plotted, but only the coordinate axes as specified.
  //  -- string: Defining the color of the plotted function.
  //  -- tuple of strings: -nultiple functions can be displayed in different colors.
  //
  //- GenParamName: Generic parameter names to control the presentation.
  // The corresponding values are taken from GenParamValue. Possible Values string/value pairs:
  //  -- 'axes_color': Color of the coordinate axes. The default value is 'white'.
  //     If 'none' is given, no coordinate axes are shown.
  //  -- 'style': Graph style. Possible values:
  //     --- 'line' (default)
  //     --- 'cross'
  //     --- 'circle'
  //     --- 'step'
  //     --- 'filled'
  //  -- 'clip': Clip graph to coordinate system area. Possible values:
  //     --- 'no' (default)
  //     --- 'yes''
  //  -- 'ticks': Control display of ticks on the axes. Thereby you have the following options:
  //     --- 'min_max_origin' (default): Ticks are shown at the minimum and maximum values
  //         of the axes and at the intercept point of x- and y-axis.
  //     --- 'none': No ticks are shown.
  //     --- any number != 0: This number specifies the distance between the ticks.
  //  -- 'ticks_x': Control display of ticks on x-axis only. You have the same options as for 'ticks'.
  //  -- 'ticks_y': Control display of ticks on y-axis only. You have the same options as for 'ticks'.
  //  -- 'format_x': Format of the values next to the ticks of the x-axis (see tuple_string for more details).
  //  -- 'format_y': Format of the values next to the ticks of the y-axis (see tuple_string for more details).
  //  -- 'grid': Control display of grid lines within the coordinate system.
  //     Thereby you have the following options:
  //    --- 'min_max_origin' (default): Grid lines are shown at the minimum and maximum values of the axes.
  //    --- 'none': No grid lines are shown.
  //    --- If any number != 0: This number specifies the distance between the grid lines.
  //  -- 'grid_x': Control display of grid lines for the x-axis only.
  //  -- 'grid_y': Control display of grid lines for the y-axis only.
  //  -- 'grid_color': Color of the grid (default: 'dim gray').
  //  -- 'margin': The distance in pixels of the plot area to all four window borders.
  //  -- 'margin_left': The distance in pixels of the plot area to the left window border.
  //  -- 'margin_right': The distance in pixels of the plot area to the right window border.
  //  -- 'margin_top': The distance in pixels of the plot area to the upper window border.
  //  -- 'margin_bottom'': The distance in pixels of the plot area to the lower window border.
  //  -- 'start_x': Lowest x value of the x-axis. The default value is min(XValues).
  //  -- 'end_x': Highest x value of the x-axis. The default value is max(XValues).
  //  -- 'start_y': Lowest y value of the y-axis. The default value is min(YValues).
  //  -- 'end_y': Highest y value of the y-axis. The default value is max(YValues).
  //  -- 'axis_location_x': Position of the x-axis (Used to be called 'origin_y').
  //     Thereby you have the following options:
  //     --- 'bottom' (default)
  //     --- 'origin'
  //     --- 'top'
  //     --- Y coordinate of the intercept point of x- and y-axis.
  //  -- 'axis_location_y': Position of the y-axis (Used to be called 'origin_x').
  //     Thereby you have the following options:
  //     --- 'left' (default)
  //     --- 'right'
  //     --- 'origin'
  //     --- X coordinate of the intercept point of x- and y-axis.
  //  -- 'line_width': Line width of the plot.
  //  -- 'log_y': If 'true', plot the YValue in logarithmic scale. Default is 'false'.
  //
  //
  GetSystem("clip_region", &hv_ClipRegion);
  GetWindowExtents(hv_WindowHandle, &hv_Row, &hv_Column, &hv_Width, &hv_Height);
  GetPart(hv_WindowHandle, &hv_PartRow1, &hv_PartColumn1, &hv_PartRow2, &hv_PartColumn2);
  hv_Width = (hv_PartColumn2-hv_PartColumn1)+1;
  hv_Height = (hv_PartRow2-hv_PartRow1)+1;
  GetRgb(hv_WindowHandle, &hv_Red, &hv_Green, &hv_Blue);
  GetDraw(hv_WindowHandle, &hv_DrawMode);
  GetLineStyle(hv_WindowHandle, &hv_OriginStyle);
  //
  //Set the display parameters.
  SetLineStyle(hv_WindowHandle, HTuple());
  SetSystem("clip_region", "false");

  hv_PartDiffers = HTuple(HTuple(HTuple(int(0!=hv_PartRow1)).TupleOr(int(0!=hv_Column))).TupleOr(int((hv_Width-1)!=hv_PartColumn2))).TupleOr(int((hv_Height-1)!=hv_PartRow2));
  //Only use set part if it differs.
  if (0 != hv_PartDiffers)
  {
    if (HDevWindowStack::IsOpen())
      SetPart(HDevWindowStack::GetActive(),0, 0, hv_Height-1, hv_Width-1);
  }
  //
  //Check if we need to plot y-values logarithmically.
  //It is checked here because we want to convert the YValues tuple
  //immediately so that derived values will be correct.
  hv_PlotYLog = 0;
  if (0 != (HTuple(int((hv_GenParamName.TupleLength())>0)).TupleAnd(int((hv_GenParamName.TupleLength())==(hv_GenParamValue.TupleLength())))))
  {
    TupleFind(hv_GenParamName, "log_y", &hv_YLogIndices);
    if (0 != (int(hv_YLogIndices>=0)))
    {
      hv_PlotYLogUser = HTuple(hv_GenParamValue[HTuple(hv_YLogIndices[0])]);
      TupleIsString(hv_PlotYLogUser, &hv_IsString);
      if (0 != hv_IsString)
      {
        if (0 != (int(hv_PlotYLogUser==HTuple("true"))))
        {
          hv_PlotYLog = 1;
        }
        else if (0 != (int(hv_PlotYLogUser==HTuple("false"))))
        {
          hv_PlotYLog = 0;
        }
        else
        {
          throw HException(("Unknown generic param value: '"+hv_PlotYLogUser)+"' for value: 'log_y'");
        }
        hv_PlotYLog = int(hv_PlotYLogUser==HTuple("true"));
      }
      else
      {
        hv_PlotYLog = int(hv_PlotYLogUser==1);
      }
    }
  }
  if (0 != hv_PlotYLog)
  {
    //Clamp values to be >= 0.00001.
    hv_YInd = hv_YValues.TupleLessEqualElem(0);
    TupleFind(hv_YInd, 1, &hv_Indices1);
    if (0 != (int(hv_Indices1>=0)))
    {
      hv_YValues[hv_Indices1] = 0.00001;
    }
    hv_YValues = hv_YValues.TupleLog10();
  }
  //
  //Check input coordinate values.
  //
  if (0 != (HTuple(int(hv_XValues==HTuple())).TupleAnd(int(hv_YValues==HTuple()))))
  {
    //Neither XValues nor YValues are given:
    //Set axes to interval [0,1]
    hv_XAxisEndValue = 1;
    hv_YAxisEndValue = 1;
    hv_XAxisStartValue = 0;
    hv_YAxisStartValue = 0;
    hv_XValuesAreStrings = 0;
  }
  else
  {
    if (0 != (int(hv_XValues==HTuple())))
    {
      //XValues are omitted: Set equidistant XValues.
      hv_XValues = HTuple::TupleGenSequence(0,(hv_YValues.TupleLength())-1,1);
      hv_XValuesAreStrings = 0;
    }
    else if (0 != (int(hv_YValues==HTuple())))
    {
      //YValues are omitted: Set equidistant YValues.
      hv_YValues = HTuple::TupleGenSequence(0,(hv_XValues.TupleLength())-1,1);
    }
    if (0 != (int(((hv_YValues.TupleLength())%(hv_XValues.TupleLength()))!=0)))
    {
      //Number of YValues does not match number of XValues.
      throw HException("Number of YValues is no multiple of the number of XValues.");
      return;
    }

    hv_XValuesAreStrings = hv_XValues.TupleIsStringElem();
    hv_XValuesAreStrings = int((hv_XValuesAreStrings.TupleSum())==(hv_XValuesAreStrings.TupleLength()));
    if (0 != hv_XValuesAreStrings)
    {
      //XValues are given as strings: Show XValues as ticks.
      hv_XTickValues = hv_XValues;
      hv_XTicks = 1;
      //Set x-axis dimensions.
      hv_XValues = HTuple::TupleGenSequence(1,hv_XValues.TupleLength(),1);
    }
    //Set default x-axis dimensions.
    if (0 != (int((hv_XValues.TupleLength())>1)))
    {
      hv_XAxisStartValue = hv_XValues.TupleMin();
      hv_XAxisEndValue = hv_XValues.TupleMax();
    }
    else
    {
      hv_XAxisEndValue = HTuple(hv_XValues[0])+0.5;
      hv_XAxisStartValue = HTuple(hv_XValues[0])-0.5;
    }
  }
  //Set default y-axis dimensions.
  if (0 != (int((hv_YValues.TupleLength())>1)))
  {
    hv_YAxisStartValue = hv_YValues.TupleMin();
    hv_YAxisEndValue = hv_YValues.TupleMax();
  }
  else if (0 != (int((hv_YValues.TupleLength())==1)))
  {
    hv_YAxisStartValue = HTuple(hv_YValues[0])-0.5;
    hv_YAxisEndValue = HTuple(hv_YValues[0])+0.5;
  }
  else
  {
    hv_YAxisStartValue = 0;
    hv_YAxisEndValue = 1;
  }
  //Set default interception point of x- and y- axis.
  hv_YAxisPosition = "default";
  hv_XAxisPosition = "default";
  //
  //Set further default values:
  hv_LeftBorder = hv_Width*0.1;
  hv_RightBorder = hv_Width*0.1;
  hv_UpperBorder = hv_Height*0.1;
  hv_LowerBorder = hv_Height*0.1;
  hv_AxesColor = "white";
  hv_Style = "line";
  hv_Clip = "no";
  hv_XTicks = "min_max_origin";
  hv_YTicks = "min_max_origin";
  hv_XGrid = "none";
  hv_YGrid = "none";
  hv_GridColor = "dim gray";
  hv_YPosition = "left";
  hv_FormatX = "default";
  hv_FormatY = "default";
  hv_LineWidth = 1;
  //
  //Parse generic parameters.
  //
  hv_NumGenParamNames = hv_GenParamName.TupleLength();
  hv_NumGenParamValues = hv_GenParamValue.TupleLength();
  if (0 != (int(hv_NumGenParamNames!=hv_NumGenParamValues)))
  {
    throw HException("Number of generic parameter names does not match generic parameter values.");
    return;
  }
  //
  {
  HTuple end_val217 = (hv_GenParamName.TupleLength())-1;
  HTuple step_val217 = 1;
  for (hv_GenParamIndex=0; hv_GenParamIndex.Continue(end_val217, step_val217); hv_GenParamIndex += step_val217)
  {
    //
    //Set 'axes_color'.
    if (0 != (int(HTuple(hv_GenParamName[hv_GenParamIndex])==HTuple("axes_color"))))
    {
      hv_AxesColor = HTuple(hv_GenParamValue[hv_GenParamIndex]);
      //
      //Set 'style'.
    }
    else if (0 != (int(HTuple(hv_GenParamName[hv_GenParamIndex])==HTuple("style"))))
    {
      hv_Style = HTuple(hv_GenParamValue[hv_GenParamIndex]);
      //
      //Set 'clip'.
    }
    else if (0 != (int(HTuple(hv_GenParamName[hv_GenParamIndex])==HTuple("clip"))))
    {
      hv_Clip = HTuple(hv_GenParamValue[hv_GenParamIndex]);
      if (0 != (HTuple(int(hv_Clip!=HTuple("yes"))).TupleAnd(int(hv_Clip!=HTuple("no")))))
      {
        throw HException(("Unsupported clipping option: '"+hv_Clip)+"'");
      }
      //
      //Set 'ticks'.
    }
    else if (0 != (int(HTuple(hv_GenParamName[hv_GenParamIndex])==HTuple("ticks"))))
    {
      hv_XTicks = HTuple(hv_GenParamValue[hv_GenParamIndex]);
      hv_YTicks = HTuple(hv_GenParamValue[hv_GenParamIndex]);
      //
      //Set 'ticks_x'.
    }
    else if (0 != (int(HTuple(hv_GenParamName[hv_GenParamIndex])==HTuple("ticks_x"))))
    {
      hv_XTicks = HTuple(hv_GenParamValue[hv_GenParamIndex]);
      //
      //Set 'ticks_y'.
    }
    else if (0 != (int(HTuple(hv_GenParamName[hv_GenParamIndex])==HTuple("ticks_y"))))
    {
      hv_YTicks = HTuple(hv_GenParamValue[hv_GenParamIndex]);
      //
      //Set 'grid'.
    }
    else if (0 != (int(HTuple(hv_GenParamName[hv_GenParamIndex])==HTuple("grid"))))
    {
      hv_XGrid = HTuple(hv_GenParamValue[hv_GenParamIndex]);
      hv_YGrid = HTuple(hv_GenParamValue[hv_GenParamIndex]);
      hv_XGridTicks = hv_XTicks;
      //
      //Set 'grid_x'
    }
    else if (0 != (int(HTuple(hv_GenParamName[hv_GenParamIndex])==HTuple("grid_x"))))
    {
      hv_XGrid = HTuple(hv_GenParamValue[hv_GenParamIndex]);
      //
      //Set 'grid_y'.
    }
    else if (0 != (int(HTuple(hv_GenParamName[hv_GenParamIndex])==HTuple("grid_y"))))
    {
      hv_YGrid = HTuple(hv_GenParamValue[hv_GenParamIndex]);
      //
      //Set 'grid_color'.
    }
    else if (0 != (int(HTuple(hv_GenParamName[hv_GenParamIndex])==HTuple("grid_color"))))
    {
      hv_GridColor = HTuple(hv_GenParamValue[hv_GenParamIndex]);
      //
      //Set 'start_x'.
    }
    else if (0 != (int(HTuple(hv_GenParamName[hv_GenParamIndex])==HTuple("start_x"))))
    {
      hv_XAxisStartValue = HTuple(hv_GenParamValue[hv_GenParamIndex]);
      //
      //Set 'end_x'.
    }
    else if (0 != (int(HTuple(hv_GenParamName[hv_GenParamIndex])==HTuple("end_x"))))
    {
      hv_XAxisEndValue = HTuple(hv_GenParamValue[hv_GenParamIndex]);
      //
      //Set 'start_y'.
    }
    else if (0 != (int(HTuple(hv_GenParamName[hv_GenParamIndex])==HTuple("start_y"))))
    {
      hv_YAxisStartValue = HTuple(hv_GenParamValue[hv_GenParamIndex]);
      if (0 != hv_PlotYLog)
      {
        hv_YAxisStartValue = (HTuple(0.00000001).TupleMax2(hv_YAxisStartValue)).TupleLog10();
      }
      //
      //Set 'end_y'.
    }
    else if (0 != (int(HTuple(hv_GenParamName[hv_GenParamIndex])==HTuple("end_y"))))
    {
      hv_YAxisEndValue = HTuple(hv_GenParamValue[hv_GenParamIndex]);
      if (0 != hv_PlotYLog)
      {
        hv_YAxisEndValue = (HTuple(0.00000001).TupleMax2(hv_YAxisEndValue)).TupleLog10();
      }
      //
      //Set 'axis_location_y' (old name 'origin_x').
    }
    else if (0 != (HTuple(int(HTuple(hv_GenParamName[hv_GenParamIndex])==HTuple("axis_location_y"))).TupleOr(int(HTuple(hv_GenParamName[hv_GenParamIndex])==HTuple("origin_x")))))
    {
      hv_YAxisPosition = HTuple(hv_GenParamValue[hv_GenParamIndex]);
      //
      //Set 'axis_location_x' (old name: 'origin_y').
    }
    else if (0 != (HTuple(int(HTuple(hv_GenParamName[hv_GenParamIndex])==HTuple("axis_location_x"))).TupleOr(int(HTuple(hv_GenParamName[hv_GenParamIndex])==HTuple("origin_y")))))
    {
      hv_XAxisPosition = HTuple(hv_GenParamValue[hv_GenParamIndex]);
      //
      //Set 'margin'.
    }
    else if (0 != (int(HTuple(hv_GenParamName[hv_GenParamIndex])==HTuple("margin"))))
    {
      hv_LeftBorder = HTuple(hv_GenParamValue[hv_GenParamIndex]);
      hv_RightBorder = HTuple(hv_GenParamValue[hv_GenParamIndex]);
      hv_UpperBorder = HTuple(hv_GenParamValue[hv_GenParamIndex]);
      hv_LowerBorder = HTuple(hv_GenParamValue[hv_GenParamIndex]);
      //
      //Set 'margin_left'.
    }
    else if (0 != (int(HTuple(hv_GenParamName[hv_GenParamIndex])==HTuple("margin_left"))))
    {
      hv_LeftBorder = HTuple(hv_GenParamValue[hv_GenParamIndex]);
      //
      //Set 'margin_right'.
    }
    else if (0 != (int(HTuple(hv_GenParamName[hv_GenParamIndex])==HTuple("margin_right"))))
    {
      hv_RightBorder = HTuple(hv_GenParamValue[hv_GenParamIndex]);
      //
      //Set 'margin_top'.
    }
    else if (0 != (int(HTuple(hv_GenParamName[hv_GenParamIndex])==HTuple("margin_top"))))
    {
      hv_UpperBorder = HTuple(hv_GenParamValue[hv_GenParamIndex]);
      //
      //Set 'margin_bottom'.
    }
    else if (0 != (int(HTuple(hv_GenParamName[hv_GenParamIndex])==HTuple("margin_bottom"))))
    {
      hv_LowerBorder = HTuple(hv_GenParamValue[hv_GenParamIndex]);
    }
    else if (0 != (int(HTuple(hv_GenParamName[hv_GenParamIndex])==HTuple("format_x"))))
    {
      hv_FormatX = HTuple(hv_GenParamValue[hv_GenParamIndex]);
    }
    else if (0 != (int(HTuple(hv_GenParamName[hv_GenParamIndex])==HTuple("format_y"))))
    {
      hv_FormatY = HTuple(hv_GenParamValue[hv_GenParamIndex]);
    }
    else if (0 != (int(HTuple(hv_GenParamName[hv_GenParamIndex])==HTuple("line_width"))))
    {
      hv_LineWidth = HTuple(hv_GenParamValue[hv_GenParamIndex]);
    }
    else if (0 != (int(HTuple(hv_GenParamName[hv_GenParamIndex])==HTuple("log_y"))))
    {
      //log_y already checked before because some other values depend on it.
    }
    else
    {
      throw HException(("Unknown generic parameter: '"+HTuple(hv_GenParamName[hv_GenParamIndex]))+"'");
    }
  }
  }
  //
  //Check consistency of start and end values of the axes.
  if (0 != (int(hv_XAxisStartValue>hv_XAxisEndValue)))
  {
    throw HException("Value for 'start_x' is greater than value for 'end_x'");
  }
  if (0 != (int(hv_YAxisStartValue>hv_YAxisEndValue)))
  {
    throw HException("Value for 'start_y' is greater than value for 'end_y'");
  }
  //
  //Set the position of the y-axis.
  if (0 != (int(hv_YAxisPosition==HTuple("default"))))
  {
    hv_YAxisPosition = hv_XAxisStartValue;
  }
  if (0 != (int((hv_YAxisPosition.TupleIsString())==1)))
  {
    if (0 != (int(hv_YAxisPosition==HTuple("left"))))
    {
      hv_YAxisPosition = hv_XAxisStartValue;
    }
    else if (0 != (int(hv_YAxisPosition==HTuple("right"))))
    {
      hv_YAxisPosition = hv_XAxisEndValue;
    }
    else if (0 != (int(hv_YAxisPosition==HTuple("origin"))))
    {
      hv_YAxisPosition = 0;
    }
    else
    {
      throw HException(("Unsupported axis_location_y: '"+hv_YAxisPosition)+"'");
    }
  }
  //Set the position of the ticks on the y-axis
  //depending of the location of the y-axis.
  if (0 != (int(((hv_XAxisStartValue.TupleConcat(hv_XAxisEndValue)).TupleMean())>hv_YAxisPosition)))
  {
    hv_YTickDirection = "right";
  }
  else
  {
    hv_YTickDirection = "left";
  }
  //
  //Set the position of the x-axis.
  if (0 != (int(hv_XAxisPosition==HTuple("default"))))
  {
    hv_XAxisPosition = hv_YAxisStartValue;
  }
  if (0 != (int((hv_XAxisPosition.TupleIsString())==1)))
  {
    if (0 != (int(hv_XAxisPosition==HTuple("bottom"))))
    {
      hv_XAxisPosition = hv_YAxisStartValue;
    }
    else if (0 != (int(hv_XAxisPosition==HTuple("top"))))
    {
      hv_XAxisPosition = hv_YAxisEndValue;
    }
    else if (0 != (int(hv_XAxisPosition==HTuple("origin"))))
    {
      hv_XAxisPosition = 0;
    }
    else
    {
      throw HException(("Unsupported axis_location_x: '"+hv_XAxisPosition)+"'");
    }
  }
  //Set the position of the ticks on the y-axis
  //depending of the location of the y-axis.
  if (0 != (int(((hv_YAxisStartValue.TupleConcat(hv_YAxisEndValue)).TupleMean())>hv_XAxisPosition)))
  {
    hv_XTickDirection = "up";
  }
  else
  {
    hv_XTickDirection = "down";
  }
  //
  //Calculate basic pixel coordinates and scale factors.
  //
  hv_XAxisWidthPx = (hv_Width-hv_LeftBorder)-hv_RightBorder;
  hv_XAxisWidth = hv_XAxisEndValue-hv_XAxisStartValue;
  if (0 != (int(hv_XAxisWidth==0)))
  {
    hv_XAxisStartValue = hv_XAxisStartValue-0.5;
    hv_XAxisEndValue += 0.5;
    hv_XAxisWidth = 1;
  }
  hv_XScaleFactor = hv_XAxisWidthPx/(hv_XAxisWidth.TupleReal());
  hv_YAxisHeightPx = (hv_Height-hv_LowerBorder)-hv_UpperBorder;
  hv_YAxisHeight = hv_YAxisEndValue-hv_YAxisStartValue;
  if (0 != (int(hv_YAxisHeight==0)))
  {
    hv_YAxisStartValue = hv_YAxisStartValue-0.5;
    hv_YAxisEndValue += 0.5;
    hv_YAxisHeight = 1;
  }
  hv_YScaleFactor = hv_YAxisHeightPx/(hv_YAxisHeight.TupleReal());
  hv_YAxisOffsetPx = (hv_YAxisPosition-hv_XAxisStartValue)*hv_XScaleFactor;
  hv_XAxisOffsetPx = (hv_XAxisPosition-hv_YAxisStartValue)*hv_YScaleFactor;
  //
  //Display grid lines.
  //
  if (0 != (int(hv_GridColor!=HTuple("none"))))
  {
    hv_DotStyle.Clear();
    hv_DotStyle[0] = 5;
    hv_DotStyle[1] = 7;
    SetLineStyle(hv_WindowHandle, hv_DotStyle);
    if (HDevWindowStack::IsOpen())
      SetColor(HDevWindowStack::GetActive(),hv_GridColor);
    //
    //Display x grid lines.
    if (0 != (int(hv_XGrid!=HTuple("none"))))
    {
      if (0 != (int(hv_XGrid==HTuple("min_max_origin"))))
      {
        //Calculate 'min_max_origin' grid line coordinates.
        if (0 != (int(hv_YAxisPosition==hv_XAxisStartValue)))
        {
          hv_XGridValues.Clear();
          hv_XGridValues.Append(hv_XAxisStartValue);
          hv_XGridValues.Append(hv_XAxisEndValue);
        }
        else
        {
          hv_XGridValues.Clear();
          hv_XGridValues.Append(hv_XAxisStartValue);
          hv_XGridValues.Append(hv_YAxisPosition);
          hv_XGridValues.Append(hv_XAxisEndValue);
        }
      }
      else
      {
        //Calculate equidistant grid line coordinates.
        hv_XGridStart = ((hv_XAxisStartValue/hv_XGrid).TupleCeil())*hv_XGrid;
        hv_XGridValues = HTuple::TupleGenSequence(hv_XGridStart,hv_XAxisEndValue,hv_XGrid);
      }
      hv_XCoord = (hv_XGridValues-hv_XAxisStartValue)*hv_XScaleFactor;
      //Generate and display grid lines.
      {
      HTuple end_val428 = (hv_XGridValues.TupleLength())-1;
      HTuple step_val428 = 1;
      for (hv_IndexGrid=0; hv_IndexGrid.Continue(end_val428, step_val428); hv_IndexGrid += step_val428)
      {
        GenContourPolygonXld(&ho_ContourXGrid, (hv_Height-hv_LowerBorder).TupleConcat(hv_UpperBorder), 
            (hv_LeftBorder+HTuple(hv_XCoord[hv_IndexGrid])).TupleConcat(hv_LeftBorder+HTuple(hv_XCoord[hv_IndexGrid])));
        if (HDevWindowStack::IsOpen())
          DispObj(ho_ContourXGrid, HDevWindowStack::GetActive());
      }
      }
    }
    //
    //Display y grid lines.
    if (0 != (int(hv_YGrid!=HTuple("none"))))
    {
      if (0 != (int(hv_YGrid==HTuple("min_max_origin"))))
      {
        //Calculate 'min_max_origin' grid line coordinates.
        if (0 != (int(hv_XAxisPosition==hv_YAxisStartValue)))
        {
          hv_YGridValues.Clear();
          hv_YGridValues.Append(hv_YAxisStartValue);
          hv_YGridValues.Append(hv_YAxisEndValue);
        }
        else
        {
          hv_YGridValues.Clear();
          hv_YGridValues.Append(hv_YAxisStartValue);
          hv_YGridValues.Append(hv_XAxisPosition);
          hv_YGridValues.Append(hv_YAxisEndValue);
        }
      }
      else
      {
        //Calculate equidistant grid line coordinates.
        hv_YGridStart = ((hv_YAxisStartValue/hv_YGrid).TupleCeil())*hv_YGrid;
        hv_YGridValues = HTuple::TupleGenSequence(hv_YGridStart,hv_YAxisEndValue,hv_YGrid);
      }
      hv_YCoord = (hv_YGridValues-hv_YAxisStartValue)*hv_YScaleFactor;
      //Generate and display grid lines.
      {
      HTuple end_val450 = (hv_YGridValues.TupleLength())-1;
      HTuple step_val450 = 1;
      for (hv_IndexGrid=0; hv_IndexGrid.Continue(end_val450, step_val450); hv_IndexGrid += step_val450)
      {
        GenContourPolygonXld(&ho_ContourYGrid, ((hv_Height-hv_LowerBorder)-HTuple(hv_YCoord[hv_IndexGrid])).TupleConcat((hv_Height-hv_LowerBorder)-HTuple(hv_YCoord[hv_IndexGrid])), 
            hv_LeftBorder.TupleConcat(hv_Width-hv_RightBorder));
        if (HDevWindowStack::IsOpen())
          DispObj(ho_ContourYGrid, HDevWindowStack::GetActive());
      }
      }
    }
  }
  SetLineStyle(hv_WindowHandle, HTuple());
  //
  //
  //Display the coordinate system axes.
  if (0 != (int(hv_AxesColor!=HTuple("none"))))
  {
    //Display axes.
    if (HDevWindowStack::IsOpen())
      SetColor(HDevWindowStack::GetActive(),hv_AxesColor);
    gen_arrow_contour_xld(&ho_XArrow, (hv_Height-hv_LowerBorder)-hv_XAxisOffsetPx, 
        hv_LeftBorder, (hv_Height-hv_LowerBorder)-hv_XAxisOffsetPx, hv_Width-hv_RightBorder, 
        0, 0);
    if (HDevWindowStack::IsOpen())
      DispObj(ho_XArrow, HDevWindowStack::GetActive());
    gen_arrow_contour_xld(&ho_YArrow, hv_Height-hv_LowerBorder, hv_LeftBorder+hv_YAxisOffsetPx, 
        hv_UpperBorder, hv_LeftBorder+hv_YAxisOffsetPx, 0, 0);
    if (HDevWindowStack::IsOpen())
      DispObj(ho_YArrow, HDevWindowStack::GetActive());
    //Display labels.
    GetStringExtents(hv_WindowHandle, hv_XLabel, &hv_Ascent, &hv_Descent, &hv_TextWidthXLabel, 
        &hv_TextHeightXLabel);
    GetStringExtents(hv_WindowHandle, hv_YLabel, &hv_Ascent, &hv_Descent, &hv_TextWidthYLabel, 
        &hv_TextHeightYLabel);
    if (0 != (int(hv_YTickDirection==HTuple("right"))))
    {
      if (0 != (int(hv_XTickDirection==HTuple("up"))))
      {
        if (HDevWindowStack::IsOpen())
          DispText(HDevWindowStack::GetActive(),hv_XLabel, "image", ((hv_Height-hv_LowerBorder)-hv_TextHeightXLabel)-3, 
              ((hv_Width-hv_RightBorder)-hv_TextWidthXLabel)-3, hv_AxesColor, "box", 
              "false");
        if (HDevWindowStack::IsOpen())
          DispText(HDevWindowStack::GetActive()," "+hv_YLabel, "image", hv_UpperBorder, 
              (hv_LeftBorder+3)+hv_YAxisOffsetPx, hv_AxesColor, "box", "false");
      }
      else
      {
        if (HDevWindowStack::IsOpen())
          DispText(HDevWindowStack::GetActive(),hv_XLabel, "image", ((hv_Height-hv_LowerBorder)+3)-hv_XAxisOffsetPx, 
              ((hv_Width-hv_RightBorder)-hv_TextWidthXLabel)-3, hv_AxesColor, "box", 
              "false");
        if (HDevWindowStack::IsOpen())
          DispText(HDevWindowStack::GetActive()," "+hv_YLabel, "image", ((hv_Height-hv_LowerBorder)-hv_TextHeightXLabel)-3, 
              (hv_LeftBorder+3)+hv_YAxisOffsetPx, hv_AxesColor, "box", "false");
      }
    }
    else
    {
      if (0 != (int(hv_XTickDirection==HTuple("up"))))
      {
        if (HDevWindowStack::IsOpen())
          DispText(HDevWindowStack::GetActive(),hv_XLabel, "image", ((hv_Height-hv_LowerBorder)-(2*hv_TextHeightXLabel))+3, 
              hv_LeftBorder-3, hv_AxesColor, "box", "false");
        if (HDevWindowStack::IsOpen())
          DispText(HDevWindowStack::GetActive()," "+hv_YLabel, "image", hv_UpperBorder, 
              ((hv_Width-hv_RightBorder)-hv_TextWidthYLabel)-13, hv_AxesColor, "box", 
              "false");
      }
      else
      {
        if (HDevWindowStack::IsOpen())
          DispText(HDevWindowStack::GetActive(),hv_XLabel, "image", ((hv_Height-hv_LowerBorder)+3)-hv_XAxisOffsetPx, 
              hv_LeftBorder-3, hv_AxesColor, "box", "false");
        if (HDevWindowStack::IsOpen())
          DispText(HDevWindowStack::GetActive()," "+hv_YLabel, "image", ((hv_Height-hv_LowerBorder)-hv_TextHeightXLabel)-3, 
              ((hv_Width-hv_RightBorder)-(2*hv_TextWidthYLabel))-3, hv_AxesColor, 
              "box", "false");
      }
    }
  }
  //
  //Display ticks.
  //
  if (0 != (int(hv_AxesColor!=HTuple("none"))))
  {
    if (HDevWindowStack::IsOpen())
      SetColor(HDevWindowStack::GetActive(),hv_AxesColor);
    if (0 != (int(hv_XTicks!=HTuple("none"))))
    {
      //
      //Display x ticks.
      if (0 != hv_XValuesAreStrings)
      {
        //Display string XValues as categories.
        hv_XTicks = (hv_XValues.TupleLength())/(hv_XTickValues.TupleLength());
        hv_XCoord = (hv_XValues-hv_XAxisStartValue)*hv_XScaleFactor;
      }
      else
      {
        //Display tick values.
        if (0 != (int(hv_XTicks==HTuple("min_max_origin"))))
        {
          //Calculate 'min_max_origin' tick coordinates.
          if (0 != (int(hv_YAxisPosition==hv_XAxisStartValue)))
          {
            hv_XTickValues.Clear();
            hv_XTickValues.Append(hv_XAxisStartValue);
            hv_XTickValues.Append(hv_XAxisEndValue);
          }
          else
          {
            hv_XTickValues.Clear();
            hv_XTickValues.Append(hv_XAxisStartValue);
            hv_XTickValues.Append(hv_YAxisPosition);
            hv_XTickValues.Append(hv_XAxisEndValue);
          }
        }
        else
        {
          //Calculate equidistant tick coordinates.
          hv_XTickStart = ((hv_XAxisStartValue/hv_XTicks).TupleCeil())*hv_XTicks;
          hv_XTickValues = HTuple::TupleGenSequence(hv_XTickStart,hv_XAxisEndValue,hv_XTicks);
        }
        //Remove ticks that are smaller than the x-axis start.
        hv_Indices = (hv_XTickValues.TupleLessElem(hv_XAxisStartValue)).TupleFind(1);
        hv_XCoord = (hv_XTickValues-hv_XAxisStartValue)*hv_XScaleFactor;
        hv_XCoord = hv_XCoord.TupleRemove(hv_Indices);
        hv_XTickValues = hv_XTickValues.TupleRemove(hv_Indices);
        //
        if (0 != (int(hv_FormatX==HTuple("default"))))
        {
          hv_TypeTicks = hv_XTicks.TupleType();
          if (0 != (int(hv_TypeTicks==4)))
          {
            //String ('min_max_origin').
            //Format depends on actual values.
            hv_TypeTicks = hv_XTickValues.TupleType();
          }
          if (0 != (int(hv_TypeTicks==1)))
          {
            //Round to integer.
            hv_XTickValues = hv_XTickValues.TupleInt();
          }
          else
          {
            //Use floating point numbers.
            hv_XTickValues = hv_XTickValues.TupleString(".2f");
          }
        }
        else
        {
          hv_XTickValues = hv_XTickValues.TupleString(hv_FormatX);
        }
      }
      //Generate and display ticks.
      {
      HTuple end_val539 = (hv_XTickValues.TupleLength())-1;
      HTuple step_val539 = 1;
      for (hv_IndexTicks=0; hv_IndexTicks.Continue(end_val539, step_val539); hv_IndexTicks += step_val539)
      {
        GetStringExtents(hv_WindowHandle, HTuple(hv_XTickValues[hv_IndexTicks]), 
            &hv_Ascent1, &hv_Descent1, &hv_TextWidthXTicks, &hv_TextHeightXTicks);
        if (0 != (int(hv_XTickDirection==HTuple("up"))))
        {
          GenContourPolygonXld(&ho_ContourXTick, ((hv_Height-hv_LowerBorder)-hv_XAxisOffsetPx).TupleConcat(((hv_Height-hv_LowerBorder)-hv_XAxisOffsetPx)-5), 
              (hv_LeftBorder+HTuple(hv_XCoord[hv_IndexTicks])).TupleConcat(hv_LeftBorder+HTuple(hv_XCoord[hv_IndexTicks])));
          if (HDevWindowStack::IsOpen())
            DispText(HDevWindowStack::GetActive(),HTuple(hv_XTickValues[hv_IndexTicks]), 
                "image", ((hv_Height-hv_LowerBorder)+2)-hv_XAxisOffsetPx, (hv_LeftBorder+HTuple(hv_XCoord[hv_IndexTicks]))-(0.5*hv_TextWidthXTicks), 
                hv_AxesColor, "box", "false");
        }
        else
        {
          GenContourPolygonXld(&ho_ContourXTick, (((hv_Height-hv_LowerBorder)-hv_XAxisOffsetPx)+5).TupleConcat((hv_Height-hv_LowerBorder)-hv_XAxisOffsetPx), 
              (hv_LeftBorder+HTuple(hv_XCoord[hv_IndexTicks])).TupleConcat(hv_LeftBorder+HTuple(hv_XCoord[hv_IndexTicks])));
          if (HDevWindowStack::IsOpen())
            DispText(HDevWindowStack::GetActive(),HTuple(hv_XTickValues[hv_IndexTicks]), 
                "image", ((hv_Height-hv_LowerBorder)-(2*hv_TextHeightXTicks))-hv_XAxisOffsetPx, 
                (hv_LeftBorder+HTuple(hv_XCoord[hv_IndexTicks]))-(0.5*hv_TextWidthXTicks), 
                hv_AxesColor, "box", "false");
        }
        if (HDevWindowStack::IsOpen())
          DispObj(ho_ContourXTick, HDevWindowStack::GetActive());
      }
      }
    }
    //
    if (0 != (int(hv_YTicks!=HTuple("none"))))
    {
      //
      //Display y ticks.

      if (0 != (int(hv_YTicks==HTuple("min_max_origin"))))
      {
        //Calculate 'min_max_origin' tick coordinates.
        if (0 != (int(hv_XAxisPosition==hv_YAxisStartValue)))
        {
          hv_YTickValues.Clear();
          hv_YTickValues.Append(hv_YAxisStartValue);
          hv_YTickValues.Append(hv_YAxisEndValue);
        }
        else
        {
          hv_YTickValues.Clear();
          hv_YTickValues.Append(hv_YAxisStartValue);
          hv_YTickValues.Append(hv_XAxisPosition);
          hv_YTickValues.Append(hv_YAxisEndValue);
        }
      }
      else
      {
        //Calculate equidistant tick coordinates.
        hv_YTickStart = ((hv_YAxisStartValue/hv_YTicks).TupleCeil())*hv_YTicks;
        hv_YTickValues = HTuple::TupleGenSequence(hv_YTickStart,hv_YAxisEndValue,hv_YTicks);
      }

      //Remove ticks that are smaller than the y-axis start.
      hv_Indices = (hv_YTickValues.TupleLessElem(hv_YAxisStartValue)).TupleFind(1);
      hv_YCoord = (hv_YTickValues-hv_YAxisStartValue)*hv_YScaleFactor;
      hv_YCoord = hv_YCoord.TupleRemove(hv_Indices);
      hv_YTickValues = hv_YTickValues.TupleRemove(hv_Indices);
      //
      if (0 != hv_PlotYLog)
      {
        hv_YTickValues = HTuple(10).TuplePow(hv_YTickValues);
      }
      if (0 != (int(hv_FormatY==HTuple("default"))))
      {
        hv_TypeTicks = hv_YTicks.TupleType();
        if (0 != (int(hv_TypeTicks==4)))
        {
          //String ('min_max_origin').
          //Format depends on actual values.
          hv_TypeTicks = hv_YTickValues.TupleType();
        }
        if (0 != (int(hv_TypeTicks==1)))
        {
          //Round to integer.
          hv_YTickValues = hv_YTickValues.TupleInt();
        }
        else
        {
          //Use floating point numbers.
          hv_YTickValues = hv_YTickValues.TupleString(".2f");
        }
      }
      else
      {
        hv_YTickValues = hv_YTickValues.TupleString(hv_FormatY);
      }
      //Generate and display ticks.
      {
      HTuple end_val596 = (hv_YTickValues.TupleLength())-1;
      HTuple step_val596 = 1;
      for (hv_IndexTicks=0; hv_IndexTicks.Continue(end_val596, step_val596); hv_IndexTicks += step_val596)
      {
        GetStringExtents(hv_WindowHandle, HTuple(hv_YTickValues[hv_IndexTicks]), 
            &hv_Ascent1, &hv_Descent1, &hv_TextWidthYTicks, &hv_TextHeightYTicks);
        //Since we only deal with numbers, use the Ascent as text height.
        hv_TextHeightYTicks = hv_Ascent;
        if (0 != (int(hv_YTickDirection==HTuple("right"))))
        {
          GenContourPolygonXld(&ho_ContourYTick, ((hv_Height-hv_LowerBorder)-HTuple(hv_YCoord[hv_IndexTicks])).TupleConcat((hv_Height-hv_LowerBorder)-HTuple(hv_YCoord[hv_IndexTicks])), 
              (hv_LeftBorder+hv_YAxisOffsetPx).TupleConcat((hv_LeftBorder+hv_YAxisOffsetPx)+5));
          if (HDevWindowStack::IsOpen())
            DispText(HDevWindowStack::GetActive(),HTuple(hv_YTickValues[hv_IndexTicks]), 
                "image", (((hv_Height-hv_LowerBorder)-hv_TextHeightYTicks)+3)-HTuple(hv_YCoord[hv_IndexTicks]), 
                ((hv_LeftBorder-hv_TextWidthYTicks)-4)+hv_YAxisOffsetPx, hv_Color, 
                "box", "false");
        }
        else
        {
          GenContourPolygonXld(&ho_ContourYTick, ((hv_Height-hv_LowerBorder)-HTuple(hv_YCoord[hv_IndexTicks])).TupleConcat((hv_Height-hv_LowerBorder)-HTuple(hv_YCoord[hv_IndexTicks])), 
              ((hv_LeftBorder+hv_YAxisOffsetPx)-5).TupleConcat(hv_LeftBorder+hv_YAxisOffsetPx));
          if (HDevWindowStack::IsOpen())
            DispText(HDevWindowStack::GetActive(),HTuple(hv_YTickValues[hv_IndexTicks]), 
                "image", (((hv_Height-hv_LowerBorder)-hv_TextHeightYTicks)+3)-HTuple(hv_YCoord[hv_IndexTicks]), 
                (hv_LeftBorder+4)+hv_YAxisOffsetPx, hv_Color, "box", "false");
        }
        if (HDevWindowStack::IsOpen())
          DispObj(ho_ContourYTick, HDevWindowStack::GetActive());
      }
      }
    }
  }
  //
  //Display function plot.
  //
  if (0 != (int(hv_Color!=HTuple("none"))))
  {
    if (0 != (HTuple(int(hv_XValues!=HTuple())).TupleAnd(int(hv_YValues!=HTuple()))))
    {
      hv_Num = (hv_YValues.TupleLength())/(hv_XValues.TupleLength());
      //
      //Iterate over all functions to be displayed.
      {
      HTuple end_val619 = hv_Num-1;
      HTuple step_val619 = 1;
      for (hv_I=0; hv_I.Continue(end_val619, step_val619); hv_I += step_val619)
      {
        //Select y values for current function.
        hv_YSelected = hv_YValues.TupleSelectRange(hv_I*(hv_XValues.TupleLength()),((hv_I+1)*(hv_XValues.TupleLength()))-1);
        //Set color
        if (0 != (int(hv_Color==HTuple())))
        {
          SetRgb(hv_WindowHandle, hv_Red, hv_Green, hv_Blue);
        }
        else
        {
          if (HDevWindowStack::IsOpen())
            SetColor(HDevWindowStack::GetActive(),HTuple(hv_Color[hv_I%(hv_Color.TupleLength())]));
        }
        //
        //Display in different styles.
        //
        if (0 != (HTuple(HTuple(HTuple(int(hv_Style==HTuple("line"))).TupleOr(int(hv_Style==HTuple()))).TupleOr(hv_Style.TupleIsReal())).TupleOr(hv_Style.TupleIsInt())))
        {
          //Style = Line. For real value, the line is plotted dashed.
          if (0 != ((hv_Style.TupleIsReal()).TupleOr(hv_Style.TupleIsInt())))
          {
            GetLineStyle(hv_WindowHandle, &hv_StyleOriginal);
            SetLineStyle(hv_WindowHandle, hv_Style.TupleConcat(hv_Style/2.0));
          }
          GenContourPolygonXld(&ho_Contour, ((hv_Height-hv_LowerBorder)-(hv_YSelected*hv_YScaleFactor))+(hv_YAxisStartValue*hv_YScaleFactor), 
              ((hv_XValues*hv_XScaleFactor)+hv_LeftBorder)-(hv_XAxisStartValue*hv_XScaleFactor));
          //Clip, if necessary.
          if (0 != (int(hv_Clip==HTuple("yes"))))
          {
            ClipContoursXld(ho_Contour, &ho_Contour, hv_UpperBorder, hv_LeftBorder, 
                hv_Height-hv_LowerBorder, hv_Width-hv_RightBorder);
          }
          GetLineWidth(hv_WindowHandle, &hv_OldLineWidth);
          if (HDevWindowStack::IsOpen())
            SetLineWidth(HDevWindowStack::GetActive(),hv_LineWidth.TupleInt());
          if (HDevWindowStack::IsOpen())
            DispObj(ho_Contour, HDevWindowStack::GetActive());
          if (HDevWindowStack::IsOpen())
            SetLineWidth(HDevWindowStack::GetActive(),hv_OldLineWidth.TupleInt());
          if (0 != ((hv_Style.TupleIsReal()).TupleOr(hv_Style.TupleIsInt())))
          {
            SetLineStyle(hv_WindowHandle, hv_StyleOriginal);
          }
        }
        else if (0 != (int(hv_Style==HTuple("cross"))))
        {
          //Style = Cross.
          GetLineWidth(hv_WindowHandle, &hv_LineWidth);
          GenCrossContourXld(&ho_Cross, ((hv_Height-hv_LowerBorder)-(hv_YSelected*hv_YScaleFactor))+(hv_YAxisStartValue*hv_YScaleFactor), 
              ((hv_XValues*hv_XScaleFactor)+hv_LeftBorder)-(hv_XAxisStartValue*hv_XScaleFactor), 
              6, 0.785398);
          //Clip, if necessary.
          if (0 != (int(hv_Clip==HTuple("yes"))))
          {
            ClipContoursXld(ho_Cross, &ho_Cross, hv_UpperBorder, hv_LeftBorder, hv_Height-hv_LowerBorder, 
                hv_Width-hv_RightBorder);
          }
          if (HDevWindowStack::IsOpen())
            DispObj(ho_Cross, HDevWindowStack::GetActive());
        }
        else if (0 != (int(hv_Style==HTuple("circle"))))
        {
          //Style = Circle.
          GetLineWidth(hv_WindowHandle, &hv_LineWidth);
          TupleGenConst(hv_YSelected.TupleLength(), 3*hv_LineWidth, &hv_Radii);
          GenCircleContourXld(&ho_Circle, ((hv_Height-hv_LowerBorder)-(hv_YSelected*hv_YScaleFactor))+(hv_YAxisStartValue*hv_YScaleFactor), 
              ((hv_XValues*hv_XScaleFactor)+hv_LeftBorder)-(hv_XAxisStartValue*hv_XScaleFactor), 
              hv_Radii, 0, 6.28318, "positive", 1);
          //Clip, if necessary.
          if (0 != (int(hv_Clip==HTuple("yes"))))
          {
            ClipContoursXld(ho_Circle, &ho_Circle, hv_UpperBorder, hv_LeftBorder, 
                hv_Height-hv_LowerBorder, hv_Width-hv_RightBorder);
          }
          GetContourStyle(hv_WindowHandle, &hv_OldContourStyle);
          SetContourStyle(hv_WindowHandle, "stroke_and_fill");
          if (HDevWindowStack::IsOpen())
            DispObj(ho_Circle, HDevWindowStack::GetActive());
          SetContourStyle(hv_WindowHandle, hv_OldContourStyle);
        }
        else if (0 != (int(hv_Style==HTuple("filled"))))
        {
          //Style = Filled.
          hv_Y1Selected.Clear();
          hv_Y1Selected.Append(0+hv_XAxisPosition);
          hv_Y1Selected.Append(hv_YSelected);
          hv_Y1Selected.Append(0+hv_XAxisPosition);
          hv_X1Selected.Clear();
          hv_X1Selected.Append(hv_XValues.TupleMin());
          hv_X1Selected.Append(hv_XValues);
          hv_X1Selected.Append(hv_XValues.TupleMax());
          if (HDevWindowStack::IsOpen())
            SetDraw(HDevWindowStack::GetActive(),"fill");
          GenRegionPolygonFilled(&ho_Filled, ((hv_Height-hv_LowerBorder)-(hv_Y1Selected*hv_YScaleFactor))+(hv_YAxisStartValue*hv_YScaleFactor), 
              ((hv_X1Selected*hv_XScaleFactor)+hv_LeftBorder)-(hv_XAxisStartValue*hv_XScaleFactor));
          //Clip, if necessary.
          if (0 != (int(hv_Clip==HTuple("yes"))))
          {
            ClipRegion(ho_Filled, &ho_Filled, hv_UpperBorder, hv_LeftBorder, hv_Height-hv_LowerBorder, 
                hv_Width-hv_RightBorder);
          }
          if (HDevWindowStack::IsOpen())
            DispObj(ho_Filled, HDevWindowStack::GetActive());
        }
        else if (0 != (int(hv_Style==HTuple("step"))))
        {
          GenEmptyObj(&ho_Stair);
          {
          HTuple end_val684 = (hv_XValues.TupleLength())-2;
          HTuple step_val684 = 1;
          for (hv_Index=0; hv_Index.Continue(end_val684, step_val684); hv_Index += step_val684)
          {
            hv_Row1 = ((hv_Height-hv_LowerBorder)-(HTuple(hv_YSelected[hv_Index])*hv_YScaleFactor))+(hv_YAxisStartValue*hv_YScaleFactor);
            hv_Row2 = ((hv_Height-hv_LowerBorder)-(HTuple(hv_YSelected[hv_Index+1])*hv_YScaleFactor))+(hv_YAxisStartValue*hv_YScaleFactor);
            hv_Col1 = ((HTuple(hv_XValues[hv_Index])*hv_XScaleFactor)+hv_LeftBorder)-(hv_XAxisStartValue*hv_XScaleFactor);
            hv_Col2 = ((HTuple(hv_XValues[hv_Index+1])*hv_XScaleFactor)+hv_LeftBorder)-(hv_XAxisStartValue*hv_XScaleFactor);
            GenContourPolygonXld(&ho_StairTmp, (hv_Row1.TupleConcat(hv_Row1)).TupleConcat(hv_Row2), 
                (hv_Col1.TupleConcat(hv_Col2)).TupleConcat(hv_Col2));
            ConcatObj(ho_Stair, ho_StairTmp, &ho_Stair);
          }
          }
          UnionAdjacentContoursXld(ho_Stair, &ho_Stair, 0.1, 0.1, "attr_keep");
          if (0 != (int(hv_Clip==HTuple("yes"))))
          {
            ClipRegion(ho_Stair, &ho_Stair, hv_UpperBorder, hv_LeftBorder, hv_Height-hv_LowerBorder, 
                hv_Width-hv_RightBorder);
          }
          if (HDevWindowStack::IsOpen())
            DispObj(ho_Stair, HDevWindowStack::GetActive());
        }
        else
        {
          throw HException("Unsupported style: "+hv_Style);
        }
      }
      }
    }
  }
  //
  //
  SetRgb(hv_WindowHandle, hv_Red, hv_Green, hv_Blue);
  if (HDevWindowStack::IsOpen())
    SetDraw(HDevWindowStack::GetActive(),hv_DrawMode);
  SetLineStyle(hv_WindowHandle, hv_OriginStyle);
  SetSystem("clip_region", hv_ClipRegion);
  return;
}

// Chapter: File / Misc
void prepare_image_lists (HTuple hv_ImageDirs, HTuple hv_Extensions, HTuple *hv_ImageBaseNames, 
    HTuple *hv_ImageExtensions, HTuple *hv_ImageDirectories)
{

  // Local iconic variables

  // Local control variables
  HTuple  hv_ImageDirIndex, hv_ImageFilesTmp, hv_ImageBaseNamesTmp;
  HTuple  hv_ImageExtensionsTmp, hv_ImageDirectoriesTmp;

  //
  //This procedure returns for all images in ImageDirs:
  //- the base names,
  //- the extensions,
  //- the directories.

  (*hv_ImageBaseNames) = HTuple();
  (*hv_ImageExtensions) = HTuple();
  (*hv_ImageDirectories) = HTuple();
  {
  HTuple end_val9 = (hv_ImageDirs.TupleLength())-1;
  HTuple step_val9 = 1;
  for (hv_ImageDirIndex=0; hv_ImageDirIndex.Continue(end_val9, step_val9); hv_ImageDirIndex += step_val9)
  {
    list_image_files(HTuple(hv_ImageDirs[hv_ImageDirIndex]), hv_Extensions, (HTuple("recursive").Append("follow_links")), 
        &hv_ImageFilesTmp);
    parse_filename(hv_ImageFilesTmp, &hv_ImageBaseNamesTmp, &hv_ImageExtensionsTmp, 
        &hv_ImageDirectoriesTmp);
    (*hv_ImageBaseNames) = (*hv_ImageBaseNames).TupleConcat(hv_ImageBaseNamesTmp);
    (*hv_ImageExtensions) = (*hv_ImageExtensions).TupleConcat(hv_ImageExtensionsTmp);
    (*hv_ImageDirectories) = (*hv_ImageDirectories).TupleConcat(hv_ImageDirectoriesTmp);
  }
  }
  return;
}

// Chapter: Deep Learning / Model
// Short Description: Standard preprocessing on the entire dataset declared in DLDataset. 
void preprocess_dl_dataset (HTuple hv_DLDataset, HTuple hv_DataDirectory, HTuple hv_DLPreprocessParam, 
    HTuple hv_GenParam, HTuple *hv_DLDatasetFileName)
{

  // Local iconic variables

  // Local control variables
  HTuple  hv_OverwriteFiles, hv_ShowProgress, hv_ClassWeightsSegmentation;
  HTuple  hv_MaxWeight, hv_DLModelType, hv_GenParamGenDLSamples;
  HTuple  hv_ClassIDsDataset, hv_SetBackgroundID, hv_Indices;
  HTuple  hv_KeyExists, hv_InstanceType, hv_GenParamName;
  HTuple  hv_GenParamIndex, hv_FileExists, hv_DLSampleDir;
  HTuple  hv_DLDatasetSamples, hv_Progress, hv_SecondsStart;
  HTuple  hv_SampleIndex, hv_DLSampleBatch, hv_SecondsElapsed;
  HTuple  hv_SecondsRemaining, hv_ProgressPercent, hv_ProgressPerSecond;
  HTuple  hv_TimeElapsedString, hv_TimeRemainingString, hv_IgnoreClassIDs;
  HTuple  hv_NormType, hv_DLDatasetPreprocessParam;

  //
  //This procedure preprocesses the samples in the dictionary DLDataset.
  //
  //** Parameters values: ***
  //
  //Set the default values.
  //Overwrite existing DLDataset file and DLSample directory.
  hv_OverwriteFiles = 0;
  //By default we show the progress of preprocessing.
  hv_ShowProgress = 1;
  //Class weights specified by user (needed for segmentation)
  hv_ClassWeightsSegmentation = HTuple();
  //Set max weight. Parameter for calculating the weights (needed for segmentation).
  hv_MaxWeight = 1000;
  //
  //Check the validity of the preprocessing parameters.
  check_dl_preprocess_param(hv_DLPreprocessParam);
  //
  //Preprocessing parameters have been checked. Therefore, avoid possible subsequent checks.
  SetDictTuple(hv_DLPreprocessParam, "check_params", 0);
  //
  //Get the model type.
  GetDictTuple(hv_DLPreprocessParam, "model_type", &hv_DLModelType);
  //
  //Initialize the generic parameters for gen_dl_samples.
  hv_GenParamGenDLSamples = HTuple();
  //Check if the background class ID is part of the DLDataset class IDs.
  if (0 != (int(hv_DLModelType==HTuple("segmentation"))))
  {
    GetDictTuple(hv_DLDataset, "class_ids", &hv_ClassIDsDataset);
    GetDictTuple(hv_DLPreprocessParam, "set_background_id", &hv_SetBackgroundID);
    if (0 != (int((hv_SetBackgroundID.TupleLength())>0)))
    {
      TupleFind(hv_ClassIDsDataset, hv_SetBackgroundID, &hv_Indices);
      if (0 != (int(hv_Indices==-1)))
      {
        throw HException(("The 'set_background_id':'"+hv_SetBackgroundID)+"' needs to be part of the DLDataset 'class_ids' tuple.");
      }
    }
  }
  else if (0 != (int(hv_DLModelType==HTuple("detection"))))
  {
    CreateDict(&hv_GenParamGenDLSamples);
    GetDictParam(hv_DLPreprocessParam, "key_exists", "instance_type", &hv_KeyExists);
    if (0 != hv_KeyExists)
    {
      GetDictTuple(hv_DLPreprocessParam, "instance_type", &hv_InstanceType);
      SetDictTuple(hv_GenParamGenDLSamples, "instance_type", hv_InstanceType);
    }
    else
    {
      SetDictTuple(hv_GenParamGenDLSamples, "instance_type", "rectangle1");
    }
  }
  //
  //Set the parameters for preprocess_dl_samples.
  SetDictTuple(hv_DLDataset, "preprocess_param", hv_DLPreprocessParam);
  //
  //Transfer generic parameters.
  if (0 != (int(hv_GenParam!=HTuple())))
  {
    GetDictParam(hv_GenParam, "keys", HTuple(), &hv_GenParamName);
    {
    HTuple end_val53 = (hv_GenParamName.TupleLength())-1;
    HTuple step_val53 = 1;
    for (hv_GenParamIndex=0; hv_GenParamIndex.Continue(end_val53, step_val53); hv_GenParamIndex += step_val53)
    {
      if (0 != (int(HTuple(hv_GenParamName[hv_GenParamIndex])==HTuple("overwrite_files"))))
      {
        GetDictTuple(hv_GenParam, "overwrite_files", &hv_OverwriteFiles);
      }
      else if (0 != (int(HTuple(hv_GenParamName[hv_GenParamIndex])==HTuple("show_progress"))))
      {
        GetDictTuple(hv_GenParam, "show_progress", &hv_ShowProgress);
        hv_ShowProgress = HTuple(int(hv_ShowProgress==HTuple("true"))).TupleOr(int(hv_ShowProgress==1));
      }
      else if (0 != (int(HTuple(hv_GenParamName[hv_GenParamIndex])==HTuple("max_weight"))))
      {
        GetDictTuple(hv_GenParam, "max_weight", &hv_MaxWeight);
        if (0 != (int(hv_DLModelType!=HTuple("segmentation"))))
        {
          throw HException("The preprocessing parameter 'max_weight' only applies for segmentation models.");
        }
      }
      else if (0 != (int(HTuple(hv_GenParamName[hv_GenParamIndex])==HTuple("class_weights"))))
      {
        GetDictTuple(hv_GenParam, "class_weights", &hv_ClassWeightsSegmentation);
        if (0 != (int(hv_DLModelType!=HTuple("segmentation"))))
        {
          throw HException("The preprocessing parameter 'class_weights' only applies for segmentation models.");
        }
      }
      else
      {
        throw HException(("Unknown generic parameter: '"+HTuple(hv_GenParamName[hv_GenParamIndex]))+"'");
      }
    }
    }
  }
  //
  //** Clean/Create data directory: ***
  TupleRegexpReplace(hv_DataDirectory, "/+$", "", &hv_DataDirectory);
  FileExists(hv_DataDirectory, &hv_FileExists);
  if (0 != (hv_FileExists.TupleAnd(hv_OverwriteFiles.TupleNot())))
  {
    throw HException(("The folder "+hv_DataDirectory)+" already exists. Either give a different directory or force overwriting using the parameter 'overwrite_files'.");
  }
  if (0 != hv_FileExists)
  {
    remove_dir_recursively(hv_DataDirectory);
  }
  MakeDir(hv_DataDirectory);
  //
  //Create the directory for the DLSamples, if it does not exist.
  //
  //Sample directory name.
  hv_DLSampleDir = hv_DataDirectory+"/samples";
  MakeDir(hv_DLSampleDir);
  //
  //Set the output path.
  SetDictTuple(hv_DLDataset, "dlsample_dir", hv_DLSampleDir);
  //
  //** Preprocess all images in the dataset: ***
  //During training/validation and testing those preprocessed images
  //will be used for performance reasons.
  //
  //Get the samples to be preprocessed.
  GetDictTuple(hv_DLDataset, "samples", &hv_DLDatasetSamples);
  //
  //Initialize progress variables.
  if (0 != hv_ShowProgress)
  {
    hv_Progress.Clear();
    hv_Progress[0] = "Procedure: preprocess_dl_dataset";
    hv_Progress[1] = "";
    hv_Progress[2] = "";
    hv_Progress[3] = "";
    if (0 != (int(hv_DLModelType==HTuple("segmentation"))))
    {
      hv_Progress = hv_Progress.TupleConcat("Task: 1/2: image preprocessing");
    }
    CountSeconds(&hv_SecondsStart);
    // dev_inspect_ctrl(...); only in hdevelop
  }
  //
  //Loop over all samples.
  {
  HTuple end_val113 = (hv_DLDatasetSamples.TupleLength())-1;
  HTuple step_val113 = 1;
  for (hv_SampleIndex=0; hv_SampleIndex.Continue(end_val113, step_val113); hv_SampleIndex += step_val113)
  {
    //
    //Generate the dictionary DLSample.
    gen_dl_samples(hv_DLDataset, hv_SampleIndex, hv_DLModelType, hv_GenParamGenDLSamples, 
        &hv_DLSampleBatch);
    //
    //Preprocess the DLSample.
    preprocess_dl_samples(hv_DLSampleBatch, hv_DLPreprocessParam);
    //
    //Write the preprocessed images.
    write_dl_samples(hv_DLDataset, hv_SampleIndex, hv_DLSampleBatch, HTuple(), HTuple());
    //
    //Provide progress information.
    if (0 != hv_ShowProgress)
    {
      if (0 != (HTuple(int((hv_SampleIndex%10)==1)).TupleOr(int(hv_SampleIndex==((hv_DLDatasetSamples.TupleLength())-1)))))
      {
        estimate_progress(hv_SecondsStart, 0, hv_SampleIndex, (hv_DLDatasetSamples.TupleLength())-1, 
            &hv_SecondsElapsed, &hv_SecondsRemaining, &hv_ProgressPercent, &hv_ProgressPerSecond);
        timespan_string(hv_SecondsElapsed, "auto", &hv_TimeElapsedString);
        timespan_string(hv_SecondsRemaining, "top2", &hv_TimeRemainingString);
        hv_Progress[1] = ("Progress: "+(hv_ProgressPercent.TupleRound()))+" %";
        hv_Progress[2] = "Time elapsed: "+hv_TimeElapsedString;
        hv_Progress[3] = "Time left: "+hv_TimeRemainingString;
      }
    }
  }
  }
  //
  //If the model is of type segmentation, generate weight images.
  if (0 != (int(hv_DLModelType==HTuple("segmentation"))))
  {
    //
    if (0 != hv_ShowProgress)
    {
      hv_Progress.Clear();
      hv_Progress[0] = "Procedure: preprocess_dl_dataset";
      hv_Progress[1] = "";
      hv_Progress[2] = "";
      hv_Progress[1] = "Please wait...";
      hv_Progress[2] = "Task: 2/2: calculating class weights";
    }
    if (0 != (int((hv_ClassWeightsSegmentation.TupleLength())==0)))
    {
      //Calculate the class weights for segmentation.
      GetDictTuple(hv_DLPreprocessParam, "ignore_class_ids", &hv_IgnoreClassIDs);
      calculate_dl_segmentation_class_weights(hv_DLDataset, hv_MaxWeight, hv_IgnoreClassIDs, 
          &hv_ClassWeightsSegmentation);
    }
    //
    //Generate the weight images.
    gen_dl_segmentation_weight_images(hv_DLDataset, hv_DLPreprocessParam, hv_ClassWeightsSegmentation, 
        HTuple());
  }
  if (0 != hv_ShowProgress)
  {
    hv_Progress = "Done.";
    // dev_close_inspect_ctrl(...); only in hdevelop
  }
  //
  //In case of normalization type 'constant_values' save proper image_range_min/max to DLDataset dict 'preprocess_param'
  GetDictParam(hv_DLPreprocessParam, "key_exists", "normalization_type", &hv_KeyExists);
  if (0 != hv_KeyExists)
  {
    GetDictTuple(hv_DLPreprocessParam, "normalization_type", &hv_NormType);
    if (0 != (int(hv_NormType==HTuple("constant_values"))))
    {
      GetDictParam(hv_DLDataset, "key_exists", "preprocess_param", &hv_KeyExists);
      if (0 != hv_KeyExists)
      {
        GetDictTuple(hv_DLDataset, "preprocess_param", &hv_DLDatasetPreprocessParam);
        SetDictTuple(hv_DLDatasetPreprocessParam, "image_range_min", -2.0);
        SetDictTuple(hv_DLDatasetPreprocessParam, "image_range_max", 2.0);
      }
    }
  }
  //
  //Write the DLDataset dict.
  (*hv_DLDatasetFileName) = hv_DataDirectory+"/dl_dataset.hdict";
  WriteDict(hv_DLDataset, (*hv_DLDatasetFileName), HTuple(), HTuple());
  return;
}

// Chapter: Deep Learning / Model
// Short Description: Preprocess anomaly images for evaluation and visualization of the deep-learning-based anomaly detection. 
void preprocess_dl_model_anomaly (HObject ho_AnomalyImages, HObject *ho_AnomalyImagesPreprocessed, 
    HTuple hv_DLPreprocessParam)
{

  // Local iconic variables

  // Local control variables
  HTuple  hv_ImageWidth, hv_ImageHeight, hv_ImageRangeMin;
  HTuple  hv_ImageRangeMax, hv_DomainHandling, hv_ModelType;
  HTuple  hv_ImageNumChannels, hv_Min, hv_Max, hv_Range, hv_ImageWidthInput;
  HTuple  hv_ImageHeightInput, hv_EqualWidth, hv_EqualHeight;
  HTuple  hv_Type, hv_NumMatches, hv_NumImages, hv_EqualByte;
  HTuple  hv_NumChannelsAllImages, hv_ImageNumChannelsTuple;
  HTuple  hv_IndicesWrongChannels;

  //
  //This procedure preprocesses the anomaly images given by AnomalyImages
  //according to the parameters in the dictionary DLPreprocessParam.
  //Note that depending on the images,
  //additional preprocessing steps might be beneficial.
  //
  //Check the validity of the preprocessing parameters.
  check_dl_preprocess_param(hv_DLPreprocessParam);
  //
  //Get the preprocessing parameters.
  GetDictTuple(hv_DLPreprocessParam, "image_width", &hv_ImageWidth);
  GetDictTuple(hv_DLPreprocessParam, "image_height", &hv_ImageHeight);
  GetDictTuple(hv_DLPreprocessParam, "image_range_min", &hv_ImageRangeMin);
  GetDictTuple(hv_DLPreprocessParam, "image_range_max", &hv_ImageRangeMax);
  GetDictTuple(hv_DLPreprocessParam, "domain_handling", &hv_DomainHandling);
  GetDictTuple(hv_DLPreprocessParam, "model_type", &hv_ModelType);
  //
  hv_ImageNumChannels = 1;
  //
  //Preprocess the images.
  //
  if (0 != (int(hv_DomainHandling==HTuple("full_domain"))))
  {
    FullDomain(ho_AnomalyImages, &ho_AnomalyImages);
  }
  else if (0 != (int(hv_DomainHandling==HTuple("crop_domain"))))
  {
    CropDomain(ho_AnomalyImages, &ho_AnomalyImages);
  }
  else if (0 != (HTuple(int(hv_DomainHandling==HTuple("keep_domain"))).TupleAnd(int(hv_ModelType==HTuple("anomaly_detection")))))
  {
    //Anomaly detection models accept the additional option 'keep_domain'.
  }
  else
  {
    throw HException("Unsupported parameter value for 'domain_handling'");
  }
  //
  MinMaxGray(ho_AnomalyImages, ho_AnomalyImages, 0, &hv_Min, &hv_Max, &hv_Range);
  if (0 != (int(hv_Min<0.0)))
  {
    throw HException("Values of anomaly image must not be smaller than 0.0.");
  }
  //
  //Zoom images only if they have a different size than the specified size.
  GetImageSize(ho_AnomalyImages, &hv_ImageWidthInput, &hv_ImageHeightInput);
  hv_EqualWidth = hv_ImageWidth.TupleEqualElem(hv_ImageWidthInput);
  hv_EqualHeight = hv_ImageHeight.TupleEqualElem(hv_ImageHeightInput);
  if (0 != (HTuple(int((hv_EqualWidth.TupleMin())==0)).TupleOr(int((hv_EqualHeight.TupleMin())==0))))
  {
    ZoomImageSize(ho_AnomalyImages, &ho_AnomalyImages, hv_ImageWidth, hv_ImageHeight, 
        "nearest_neighbor");
  }
  //
  //Check the type of the input images.
  GetImageType(ho_AnomalyImages, &hv_Type);
  TupleRegexpTest(hv_Type, "byte|real", &hv_NumMatches);
  CountObj(ho_AnomalyImages, &hv_NumImages);
  if (0 != (int(hv_NumMatches!=hv_NumImages)))
  {
    throw HException("Please provide only images of type 'byte' or 'real'.");
  }
  //
  //If the type is 'byte', convert it to 'real' and scale it.
  //The gray value scaling does not work on 'byte' images.
  //For 'real' images it is assumed that the range is already correct.
  hv_EqualByte = hv_Type.TupleEqualElem("byte");
  if (0 != (int((hv_EqualByte.TupleMax())==1)))
  {
    if (0 != (int((hv_EqualByte.TupleMin())==0)))
    {
      throw HException("Passing mixed type images is not supported.");
    }
    //Convert the image type from 'byte' to 'real',
    //because the model expects 'real' images.
    ConvertImageType(ho_AnomalyImages, &ho_AnomalyImages, "real");
  }
  //
  //Check the number of channels.
  CountObj(ho_AnomalyImages, &hv_NumImages);
  //Check all images for number of channels.
  CountChannels(ho_AnomalyImages, &hv_NumChannelsAllImages);
  TupleGenConst(hv_NumImages, hv_ImageNumChannels, &hv_ImageNumChannelsTuple);
  TupleFind(hv_NumChannelsAllImages.TupleNotEqualElem(hv_ImageNumChannelsTuple), 
      1, &hv_IndicesWrongChannels);
  //
  //Check for anomaly image channels.
  //Only single channel images are accepted.
  if (0 != (int(hv_IndicesWrongChannels!=-1)))
  {
    throw HException("Number of channels in anomaly image is not supported. Please check for anomaly images with a number of channels different from 1.");
  }
  //
  //Write preprocessed image to output variable.
  (*ho_AnomalyImagesPreprocessed) = ho_AnomalyImages;
  //
  return;
}

// Chapter: Deep Learning / Object Detection
// Short Description: This procedure preprocesses the bounding boxes of type 'rectangle1' for a given sample. 
void preprocess_dl_model_bbox_rect1 (HObject ho_ImageRaw, HTuple hv_DLSample, HTuple hv_DLPreprocessParam)
{

  // Local iconic variables
  HObject  ho_DomainRaw;

  // Local control variables
  HTuple  hv_ImageWidth, hv_ImageHeight, hv_DomainHandling;
  HTuple  hv_BBoxCol1, hv_BBoxCol2, hv_BBoxRow1, hv_BBoxRow2;
  HTuple  hv_BBoxLabel, hv_Exception, hv_ImageId, hv_ExceptionMessage;
  HTuple  hv_BoxesInvalid, hv_RowDomain1, hv_ColumnDomain1;
  HTuple  hv_RowDomain2, hv_ColumnDomain2, hv_WidthRaw, hv_HeightRaw;
  HTuple  hv_Row1, hv_Col1, hv_Row2, hv_Col2, hv_MaskDelete;
  HTuple  hv_MaskNewBbox, hv_BBoxCol1New, hv_BBoxCol2New;
  HTuple  hv_BBoxRow1New, hv_BBoxRow2New, hv_BBoxLabelNew;
  HTuple  hv_FactorResampleWidth, hv_FactorResampleHeight;

  //
  //This procedure preprocesses the bounding boxes of type 'rectangle1' for a given sample.
  //
  //Check the validity of the preprocessing parameters.
  check_dl_preprocess_param(hv_DLPreprocessParam);
  //
  //Get the preprocessing parameters.
  GetDictTuple(hv_DLPreprocessParam, "image_width", &hv_ImageWidth);
  GetDictTuple(hv_DLPreprocessParam, "image_height", &hv_ImageHeight);
  GetDictTuple(hv_DLPreprocessParam, "domain_handling", &hv_DomainHandling);
  //
  //Get bounding box coordinates and labels.
  try
  {
    GetDictTuple(hv_DLSample, "bbox_col1", &hv_BBoxCol1);
    GetDictTuple(hv_DLSample, "bbox_col2", &hv_BBoxCol2);
    GetDictTuple(hv_DLSample, "bbox_row1", &hv_BBoxRow1);
    GetDictTuple(hv_DLSample, "bbox_row2", &hv_BBoxRow2);
    GetDictTuple(hv_DLSample, "bbox_label_id", &hv_BBoxLabel);
  }
  // catch (Exception) 
  catch (HException &HDevExpDefaultException)
  {
    HDevExpDefaultException.ToHTuple(&hv_Exception);
    GetDictTuple(hv_DLSample, "image_id", &hv_ImageId);
    if (0 != (int(HTuple(hv_Exception[0])==1302)))
    {
      hv_ExceptionMessage = "A bounding box coordinate key is missing.";
    }
    else
    {
      hv_ExceptionMessage = ((const HTuple&)hv_Exception)[2];
    }
    throw HException((("An error has occurred during preprocessing image_id "+hv_ImageId)+" when getting bounding box coordinates : ")+hv_ExceptionMessage);
  }
  //
  //Check that there are no invalid boxes.
  if (0 != (int((hv_BBoxRow1.TupleLength())>0)))
  {
    hv_BoxesInvalid = (hv_BBoxRow1.TupleGreaterEqualElem(hv_BBoxRow2)).TupleOr(hv_BBoxCol1.TupleGreaterEqualElem(hv_BBoxCol2));
    if (0 != (int((hv_BoxesInvalid.TupleSum())>0)))
    {
      GetDictTuple(hv_DLSample, "image_id", &hv_ImageId);
      throw HException(("An error has occurred during preprocessing image_id "+hv_ImageId)+HTuple(": Sample contains at least one box with zero-area, i.e. bbox_col1 >= bbox_col2 or bbox_row1 >= bbox_row2."));
    }
  }
  else
  {
    //There are no bounding boxes, hence nothing to do.
    return;
  }
  //
  //If the domain is cropped, crop bounding boxes.
  if (0 != (int(hv_DomainHandling==HTuple("crop_domain"))))
  {
    //
    //Get domain.
    GetDomain(ho_ImageRaw, &ho_DomainRaw);
    //
    //Set the size of the raw image to the domain extensions.
    SmallestRectangle1(ho_DomainRaw, &hv_RowDomain1, &hv_ColumnDomain1, &hv_RowDomain2, 
        &hv_ColumnDomain2);
    //The domain is always given as a pixel-precise region.
    hv_WidthRaw = (hv_ColumnDomain2-hv_ColumnDomain1)+1.0;
    hv_HeightRaw = (hv_RowDomain2-hv_RowDomain1)+1.0;
    //
    //Crop the bounding boxes.
    hv_Row1 = hv_BBoxRow1.TupleMax2(hv_RowDomain1-.5);
    hv_Col1 = hv_BBoxCol1.TupleMax2(hv_ColumnDomain1-.5);
    hv_Row2 = hv_BBoxRow2.TupleMin2(hv_RowDomain2+.5);
    hv_Col2 = hv_BBoxCol2.TupleMin2(hv_ColumnDomain2+.5);
    hv_MaskDelete = (hv_Row1.TupleGreaterEqualElem(hv_Row2)).TupleOr(hv_Col1.TupleGreaterEqualElem(hv_Col2));
    hv_MaskNewBbox = 1-hv_MaskDelete;
    //Store the preprocessed bounding box entries.
    hv_BBoxCol1New = (hv_Col1.TupleSelectMask(hv_MaskNewBbox))-hv_ColumnDomain1;
    hv_BBoxCol2New = (hv_Col2.TupleSelectMask(hv_MaskNewBbox))-hv_ColumnDomain1;
    hv_BBoxRow1New = (hv_Row1.TupleSelectMask(hv_MaskNewBbox))-hv_RowDomain1;
    hv_BBoxRow2New = (hv_Row2.TupleSelectMask(hv_MaskNewBbox))-hv_RowDomain1;
    hv_BBoxLabelNew = hv_BBoxLabel.TupleSelectMask(hv_MaskNewBbox);
    //
  }
  else if (0 != (int(hv_DomainHandling==HTuple("full_domain"))))
  {
    //If the entire image is used, set the variables accordingly.
    //Get the original size.
    GetImageSize(ho_ImageRaw, &hv_WidthRaw, &hv_HeightRaw);
    //Set new coordinates to input coordinates.
    hv_BBoxCol1New = hv_BBoxCol1;
    hv_BBoxCol2New = hv_BBoxCol2;
    hv_BBoxRow1New = hv_BBoxRow1;
    hv_BBoxRow2New = hv_BBoxRow2;
    hv_BBoxLabelNew = hv_BBoxLabel;
  }
  else
  {
    throw HException("Unsupported parameter value for 'domain_handling'");
  }
  //
  //Rescale the bounding boxes.
  //
  //Get required images width and height.
  //
  //Only rescale bounding boxes if the required image dimensions are not the raw dimensions.
  if (0 != (HTuple(int(hv_ImageHeight!=hv_HeightRaw)).TupleOr(int(hv_ImageWidth!=hv_WidthRaw))))
  {
    //Calculate rescaling factor.
    hv_FactorResampleWidth = (hv_ImageWidth.TupleReal())/hv_WidthRaw;
    hv_FactorResampleHeight = (hv_ImageHeight.TupleReal())/hv_HeightRaw;
    //Rescale the bounding box coordinates.
    //As we use XLD-coordinates we temporarily move the boxes by (.5,.5) for rescaling.
    //Doing so, the center of the XLD-coordinate system (-0.5,-0.5) is used
    //for scaling, hence the scaling is performed w.r.t. the pixel coordinate system.
    hv_BBoxCol1New = ((hv_BBoxCol1New+.5)*hv_FactorResampleWidth)-.5;
    hv_BBoxCol2New = ((hv_BBoxCol2New+.5)*hv_FactorResampleWidth)-.5;
    hv_BBoxRow1New = ((hv_BBoxRow1New+.5)*hv_FactorResampleHeight)-.5;
    hv_BBoxRow2New = ((hv_BBoxRow2New+.5)*hv_FactorResampleHeight)-.5;
    //
  }
  //
  //Make a final check and remove bounding boxes that have zero area.
  if (0 != (int((hv_BBoxRow1New.TupleLength())>0)))
  {
    hv_MaskDelete = (hv_BBoxRow1New.TupleGreaterEqualElem(hv_BBoxRow2New)).TupleOr(hv_BBoxCol1New.TupleGreaterEqualElem(hv_BBoxCol2New));
    hv_BBoxCol1New = hv_BBoxCol1New.TupleSelectMask(1-hv_MaskDelete);
    hv_BBoxCol2New = hv_BBoxCol2New.TupleSelectMask(1-hv_MaskDelete);
    hv_BBoxRow1New = hv_BBoxRow1New.TupleSelectMask(1-hv_MaskDelete);
    hv_BBoxRow2New = hv_BBoxRow2New.TupleSelectMask(1-hv_MaskDelete);
    hv_BBoxLabelNew = hv_BBoxLabelNew.TupleSelectMask(1-hv_MaskDelete);
  }
  //
  //Set new bounding box coordinates in the dictionary.
  SetDictTuple(hv_DLSample, "bbox_col1", hv_BBoxCol1New);
  SetDictTuple(hv_DLSample, "bbox_col2", hv_BBoxCol2New);
  SetDictTuple(hv_DLSample, "bbox_row1", hv_BBoxRow1New);
  SetDictTuple(hv_DLSample, "bbox_row2", hv_BBoxRow2New);
  SetDictTuple(hv_DLSample, "bbox_label_id", hv_BBoxLabelNew);
  //
  return;
}

// Chapter: Deep Learning / Object Detection
// Short Description: This procedure preprocesses the bounding boxes of type 'rectangle2' for a given sample. 
void preprocess_dl_model_bbox_rect2 (HObject ho_ImageRaw, HTuple hv_DLSample, HTuple hv_DLPreprocessParam)
{

  // Local iconic variables
  HObject  ho_DomainRaw, ho_Rectangle2XLD, ho_Rectangle2XLDSheared;

  // Local control variables
  HTuple  hv_ImageWidth, hv_ImageHeight, hv_DomainHandling;
  HTuple  hv_IgnoreDirection, hv_ClassIDsNoOrientation, hv_KeyExists;
  HTuple  hv_BBoxRow, hv_BBoxCol, hv_BBoxLength1, hv_BBoxLength2;
  HTuple  hv_BBoxPhi, hv_BBoxLabel, hv_Exception, hv_ImageId;
  HTuple  hv_ExceptionMessage, hv_BoxesInvalid, hv_RowDomain1;
  HTuple  hv_ColumnDomain1, hv_RowDomain2, hv_ColumnDomain2;
  HTuple  hv_WidthRaw, hv_HeightRaw, hv_MaskDelete, hv_MaskNewBbox;
  HTuple  hv_BBoxRowNew, hv_BBoxColNew, hv_BBoxLength1New;
  HTuple  hv_BBoxLength2New, hv_BBoxPhiNew, hv_BBoxLabelNew;
  HTuple  hv_ClassIDsNoOrientationIndices, hv_Index, hv_ClassIDsNoOrientationIndicesTmp;
  HTuple  hv_DirectionLength1Row, hv_DirectionLength1Col;
  HTuple  hv_DirectionLength2Row, hv_DirectionLength2Col;
  HTuple  hv_Corner1Row, hv_Corner1Col, hv_Corner2Row, hv_Corner2Col;
  HTuple  hv_FactorResampleWidth, hv_FactorResampleHeight;
  HTuple  hv_BBoxCol1, hv_BBoxCol1New, hv_BBoxCol2, hv_BBoxCol2New;
  HTuple  hv_BBoxCol3, hv_BBoxCol3New, hv_BBoxCol4, hv_BBoxCol4New;
  HTuple  hv_BBoxRow1, hv_BBoxRow1New, hv_BBoxRow2, hv_BBoxRow2New;
  HTuple  hv_BBoxRow3, hv_BBoxRow3New, hv_BBoxRow4, hv_BBoxRow4New;
  HTuple  hv_HomMat2DIdentity, hv_HomMat2DScale, hv_BBoxPhiTmp;
  HTuple  hv_PhiDelta, hv_PhiDeltaNegativeIndices, hv_IndicesRot90;
  HTuple  hv_IndicesRot180, hv_IndicesRot270, hv_SwapIndices;
  HTuple  hv_Tmp, hv_BBoxPhiNewIndices, hv_PhiThreshold, hv_PhiToCorrect;
  HTuple  hv_NumCorrections, hv__;

  //This procedure preprocesses the bounding boxes of type 'rectangle2' for a given sample.
  //
  check_dl_preprocess_param(hv_DLPreprocessParam);
  //
  //Get preprocess parameters.
  GetDictTuple(hv_DLPreprocessParam, "image_width", &hv_ImageWidth);
  GetDictTuple(hv_DLPreprocessParam, "image_height", &hv_ImageHeight);
  GetDictTuple(hv_DLPreprocessParam, "domain_handling", &hv_DomainHandling);
  //The keys 'ignore_direction' and 'class_ids_no_orientation' are optional.
  hv_IgnoreDirection = 0;
  hv_ClassIDsNoOrientation = HTuple();
  GetDictParam(hv_DLPreprocessParam, "key_exists", (HTuple("ignore_direction").Append("class_ids_no_orientation")), 
      &hv_KeyExists);
  if (0 != (HTuple(hv_KeyExists[0])))
  {
    GetDictTuple(hv_DLPreprocessParam, "ignore_direction", &hv_IgnoreDirection);
    if (0 != (int(hv_IgnoreDirection==HTuple("true"))))
    {
      hv_IgnoreDirection = 1;
    }
    else if (0 != (int(hv_IgnoreDirection==HTuple("false"))))
    {
      hv_IgnoreDirection = 0;
    }
  }
  if (0 != (HTuple(hv_KeyExists[1])))
  {
    GetDictTuple(hv_DLPreprocessParam, "class_ids_no_orientation", &hv_ClassIDsNoOrientation);
  }
  //
  //Get bounding box coordinates and labels.
  try
  {
    GetDictTuple(hv_DLSample, "bbox_row", &hv_BBoxRow);
    GetDictTuple(hv_DLSample, "bbox_col", &hv_BBoxCol);
    GetDictTuple(hv_DLSample, "bbox_length1", &hv_BBoxLength1);
    GetDictTuple(hv_DLSample, "bbox_length2", &hv_BBoxLength2);
    GetDictTuple(hv_DLSample, "bbox_phi", &hv_BBoxPhi);
    GetDictTuple(hv_DLSample, "bbox_label_id", &hv_BBoxLabel);
  }
  // catch (Exception) 
  catch (HException &HDevExpDefaultException)
  {
    HDevExpDefaultException.ToHTuple(&hv_Exception);
    GetDictTuple(hv_DLSample, "image_id", &hv_ImageId);
    if (0 != (int(HTuple(hv_Exception[0])==1302)))
    {
      hv_ExceptionMessage = "A bounding box coordinate key is missing.";
    }
    else
    {
      hv_ExceptionMessage = ((const HTuple&)hv_Exception)[2];
    }
    throw HException((("An error has occurred during preprocessing image_id "+hv_ImageId)+" when getting bounding box coordinates : ")+hv_ExceptionMessage);
  }
  //
  //Check that there are no invalid boxes.
  if (0 != (int((hv_BBoxRow.TupleLength())>0)))
  {
    hv_BoxesInvalid = ((hv_BBoxLength1.TupleEqualElem(0)).TupleSum())+((hv_BBoxLength2.TupleEqualElem(0)).TupleSum());
    if (0 != (int(hv_BoxesInvalid>0)))
    {
      GetDictTuple(hv_DLSample, "image_id", &hv_ImageId);
      throw HException(("An error has occurred during preprocessing image_id "+hv_ImageId)+HTuple(": Sample contains at least one bounding box with zero-area, i.e. bbox_length1 == 0 or bbox_length2 == 0!"));
    }
  }
  else
  {
    //There are no bounding boxes, hence nothing to do.
    return;
  }
  //
  //If the domain is cropped, crop bounding boxes.
  if (0 != (int(hv_DomainHandling==HTuple("crop_domain"))))
  {
    //
    //Get domain.
    GetDomain(ho_ImageRaw, &ho_DomainRaw);
    //
    //Set the size of the raw image to the domain extensions.
    SmallestRectangle1(ho_DomainRaw, &hv_RowDomain1, &hv_ColumnDomain1, &hv_RowDomain2, 
        &hv_ColumnDomain2);
    hv_WidthRaw = (hv_ColumnDomain2-hv_ColumnDomain1)+1;
    hv_HeightRaw = (hv_RowDomain2-hv_RowDomain1)+1;
    //
    //Crop the bounding boxes.
    //Remove the boxes with center outside of the domain.
    hv_MaskDelete = HTuple(HTuple((hv_BBoxRow.TupleLessElem(hv_RowDomain1)).TupleOr(hv_BBoxCol.TupleLessElem(hv_ColumnDomain1))).TupleOr(hv_BBoxRow.TupleGreaterElem(hv_RowDomain2))).TupleOr(hv_BBoxCol.TupleGreaterElem(hv_ColumnDomain2));
    hv_MaskNewBbox = 1-hv_MaskDelete;
    //Store the preprocessed bounding box entries.
    hv_BBoxRowNew = (hv_BBoxRow.TupleSelectMask(hv_MaskNewBbox))-hv_RowDomain1;
    hv_BBoxColNew = (hv_BBoxCol.TupleSelectMask(hv_MaskNewBbox))-hv_ColumnDomain1;
    hv_BBoxLength1New = hv_BBoxLength1.TupleSelectMask(hv_MaskNewBbox);
    hv_BBoxLength2New = hv_BBoxLength2.TupleSelectMask(hv_MaskNewBbox);
    hv_BBoxPhiNew = hv_BBoxPhi.TupleSelectMask(hv_MaskNewBbox);
    hv_BBoxLabelNew = hv_BBoxLabel.TupleSelectMask(hv_MaskNewBbox);
    //
  }
  else if (0 != (int(hv_DomainHandling==HTuple("full_domain"))))
  {
    //If the entire image is used, set the variables accordingly.
    //Get the original size.
    GetImageSize(ho_ImageRaw, &hv_WidthRaw, &hv_HeightRaw);
    //Set new coordinates to input coordinates.
    hv_BBoxRowNew = hv_BBoxRow;
    hv_BBoxColNew = hv_BBoxCol;
    hv_BBoxLength1New = hv_BBoxLength1;
    hv_BBoxLength2New = hv_BBoxLength2;
    hv_BBoxPhiNew = hv_BBoxPhi;
    hv_BBoxLabelNew = hv_BBoxLabel;
  }
  else
  {
    throw HException("Unsupported parameter value for 'domain_handling'");
  }
  //
  //Generate smallest enclosing axis-aligned bounding box for classes in ClassIDsNoOrientation.
  hv_ClassIDsNoOrientationIndices = HTuple();
  {
  HTuple end_val94 = (hv_ClassIDsNoOrientation.TupleLength())-1;
  HTuple step_val94 = 1;
  for (hv_Index=0; hv_Index.Continue(end_val94, step_val94); hv_Index += step_val94)
  {
    hv_ClassIDsNoOrientationIndicesTmp = (hv_BBoxLabelNew.TupleEqualElem(HTuple(hv_ClassIDsNoOrientation[hv_Index]))).TupleFind(1);
    if (0 != (int(hv_ClassIDsNoOrientationIndicesTmp!=-1)))
    {
      hv_ClassIDsNoOrientationIndices = hv_ClassIDsNoOrientationIndices.TupleConcat(hv_ClassIDsNoOrientationIndicesTmp);
    }
  }
  }
  if (0 != (int((hv_ClassIDsNoOrientationIndices.TupleLength())>0)))
  {
    //Calculate length1 and length2 using position of corners.
    hv_DirectionLength1Row = -(HTuple(hv_BBoxPhiNew[hv_ClassIDsNoOrientationIndices]).TupleSin());
    hv_DirectionLength1Col = HTuple(hv_BBoxPhiNew[hv_ClassIDsNoOrientationIndices]).TupleCos();
    hv_DirectionLength2Row = -hv_DirectionLength1Col;
    hv_DirectionLength2Col = hv_DirectionLength1Row;
    hv_Corner1Row = (HTuple(hv_BBoxLength1New[hv_ClassIDsNoOrientationIndices])*hv_DirectionLength1Row)+(HTuple(hv_BBoxLength2New[hv_ClassIDsNoOrientationIndices])*hv_DirectionLength2Row);
    hv_Corner1Col = (HTuple(hv_BBoxLength1New[hv_ClassIDsNoOrientationIndices])*hv_DirectionLength1Col)+(HTuple(hv_BBoxLength2New[hv_ClassIDsNoOrientationIndices])*hv_DirectionLength2Col);
    hv_Corner2Row = (HTuple(hv_BBoxLength1New[hv_ClassIDsNoOrientationIndices])*hv_DirectionLength1Row)-(HTuple(hv_BBoxLength2New[hv_ClassIDsNoOrientationIndices])*hv_DirectionLength2Row);
    hv_Corner2Col = (HTuple(hv_BBoxLength1New[hv_ClassIDsNoOrientationIndices])*hv_DirectionLength1Col)-(HTuple(hv_BBoxLength2New[hv_ClassIDsNoOrientationIndices])*hv_DirectionLength2Col);
    //
    hv_BBoxPhiNew[hv_ClassIDsNoOrientationIndices] = 0.0;
    hv_BBoxLength1New[hv_ClassIDsNoOrientationIndices] = (hv_Corner1Col.TupleAbs()).TupleMax2(hv_Corner2Col.TupleAbs());
    hv_BBoxLength2New[hv_ClassIDsNoOrientationIndices] = (hv_Corner1Row.TupleAbs()).TupleMax2(hv_Corner2Row.TupleAbs());
  }
  //
  //Rescale bounding boxes.
  //
  //Get required images width and height.
  //
  //Only rescale bounding boxes if the required image dimensions are not the raw dimensions.
  if (0 != (HTuple(int(hv_ImageHeight!=hv_HeightRaw)).TupleOr(int(hv_ImageWidth!=hv_WidthRaw))))
  {
    //Calculate rescaling factor.
    hv_FactorResampleWidth = (hv_ImageWidth.TupleReal())/hv_WidthRaw;
    hv_FactorResampleHeight = (hv_ImageHeight.TupleReal())/hv_HeightRaw;
    if (0 != (int(hv_FactorResampleHeight!=hv_FactorResampleWidth)))
    {
      //In order to preserve the correct orientation we have to transform the points individually.
      //Get the coordinates of the four corner points.
      convert_rect2_5to8param(hv_BBoxRowNew, hv_BBoxColNew, hv_BBoxLength1New, hv_BBoxLength2New, 
          hv_BBoxPhiNew, &hv_BBoxRow1, &hv_BBoxCol1, &hv_BBoxRow2, &hv_BBoxCol2, 
          &hv_BBoxRow3, &hv_BBoxCol3, &hv_BBoxRow4, &hv_BBoxCol4);
      //
      //Rescale the coordinates.
      hv_BBoxCol1New = hv_BBoxCol1*hv_FactorResampleWidth;
      hv_BBoxCol2New = hv_BBoxCol2*hv_FactorResampleWidth;
      hv_BBoxCol3New = hv_BBoxCol3*hv_FactorResampleWidth;
      hv_BBoxCol4New = hv_BBoxCol4*hv_FactorResampleWidth;
      hv_BBoxRow1New = hv_BBoxRow1*hv_FactorResampleHeight;
      hv_BBoxRow2New = hv_BBoxRow2*hv_FactorResampleHeight;
      hv_BBoxRow3New = hv_BBoxRow3*hv_FactorResampleHeight;
      hv_BBoxRow4New = hv_BBoxRow4*hv_FactorResampleHeight;
      //
      //The rectangles will get sheared, that is why new rectangles have to be found.
      //Generate homography to scale rectangles.
      HomMat2dIdentity(&hv_HomMat2DIdentity);
      HomMat2dScale(hv_HomMat2DIdentity, hv_FactorResampleHeight, hv_FactorResampleWidth, 
          0, 0, &hv_HomMat2DScale);
      //Generate XLD contours for the rectangles.
      GenRectangle2ContourXld(&ho_Rectangle2XLD, hv_BBoxRowNew, hv_BBoxColNew, hv_BBoxPhiNew, 
          hv_BBoxLength1New, hv_BBoxLength2New);
      //Scale the XLD contours --> results in sheared regions.
      AffineTransContourXld(ho_Rectangle2XLD, &ho_Rectangle2XLDSheared, hv_HomMat2DScale);
      SmallestRectangle2Xld(ho_Rectangle2XLDSheared, &hv_BBoxRowNew, &hv_BBoxColNew, 
          &hv_BBoxPhiNew, &hv_BBoxLength1New, &hv_BBoxLength2New);
      //
      //smallest_rectangle2_xld might change the orientation of the bounding box.
      //Hence, take the orientation that is closest to the one obtained out of the 4 corner points.
      convert_rect2_8to5param(hv_BBoxRow1New, hv_BBoxCol1New, hv_BBoxRow2New, hv_BBoxCol2New, 
          hv_BBoxRow3New, hv_BBoxCol3New, hv_BBoxRow4New, hv_BBoxCol4New, hv_IgnoreDirection, 
          &hv__, &hv__, &hv__, &hv__, &hv_BBoxPhiTmp);
      hv_PhiDelta = (hv_BBoxPhiTmp-hv_BBoxPhiNew).TupleFmod(HTuple(360).TupleRad());
      //Guarantee that angles are positive.
      hv_PhiDeltaNegativeIndices = (hv_PhiDelta.TupleLessElem(0.0)).TupleFind(1);
      if (0 != (int(hv_PhiDeltaNegativeIndices!=-1)))
      {
        hv_PhiDelta[hv_PhiDeltaNegativeIndices] = HTuple(hv_PhiDelta[hv_PhiDeltaNegativeIndices])+(HTuple(360).TupleRad());
      }
      hv_IndicesRot90 = HTuple((hv_PhiDelta.TupleGreaterElem(HTuple(45).TupleRad())).TupleAnd(hv_PhiDelta.TupleLessEqualElem(HTuple(135).TupleRad()))).TupleFind(1);
      hv_IndicesRot180 = HTuple((hv_PhiDelta.TupleGreaterElem(HTuple(135).TupleRad())).TupleAnd(hv_PhiDelta.TupleLessEqualElem(HTuple(225).TupleRad()))).TupleFind(1);
      hv_IndicesRot270 = HTuple((hv_PhiDelta.TupleGreaterElem(HTuple(225).TupleRad())).TupleAnd(hv_PhiDelta.TupleLessEqualElem(HTuple(315).TupleRad()))).TupleFind(1);
      hv_SwapIndices = HTuple();
      if (0 != (int(hv_IndicesRot90!=-1)))
      {
        hv_BBoxPhiNew[hv_IndicesRot90] = HTuple(hv_BBoxPhiNew[hv_IndicesRot90])+(HTuple(90).TupleRad());
        hv_SwapIndices = hv_SwapIndices.TupleConcat(hv_IndicesRot90);
      }
      if (0 != (int(hv_IndicesRot180!=-1)))
      {
        hv_BBoxPhiNew[hv_IndicesRot180] = HTuple(hv_BBoxPhiNew[hv_IndicesRot180])+(HTuple(180).TupleRad());
      }
      if (0 != (int(hv_IndicesRot270!=-1)))
      {
        hv_BBoxPhiNew[hv_IndicesRot270] = HTuple(hv_BBoxPhiNew[hv_IndicesRot270])+(HTuple(270).TupleRad());
        hv_SwapIndices = hv_SwapIndices.TupleConcat(hv_IndicesRot270);
      }
      if (0 != (int(hv_SwapIndices!=HTuple())))
      {
        hv_Tmp = HTuple(hv_BBoxLength1New[hv_SwapIndices]);
        hv_BBoxLength1New[hv_SwapIndices] = HTuple(hv_BBoxLength2New[hv_SwapIndices]);
        hv_BBoxLength2New[hv_SwapIndices] = hv_Tmp;
      }
      //Change angles such that they lie in the range (-180бу, 180бу].
      hv_BBoxPhiNewIndices = (hv_BBoxPhiNew.TupleGreaterElem(HTuple(180).TupleRad())).TupleFind(1);
      if (0 != (int(hv_BBoxPhiNewIndices!=-1)))
      {
        hv_BBoxPhiNew[hv_BBoxPhiNewIndices] = HTuple(hv_BBoxPhiNew[hv_BBoxPhiNewIndices])-(HTuple(360).TupleRad());
      }
      //
    }
    else
    {
      hv_BBoxColNew = hv_BBoxColNew*hv_FactorResampleWidth;
      hv_BBoxRowNew = hv_BBoxRowNew*hv_FactorResampleWidth;
      hv_BBoxLength1New = hv_BBoxLength1New*hv_FactorResampleWidth;
      hv_BBoxLength2New = hv_BBoxLength2New*hv_FactorResampleWidth;
      //Phi stays the same.
    }
    //
  }
  //
  //Adapt the bounding box angles such that they are within the correct range,
  //which is (-180бу,180бу] for 'ignore_direction'==false and (-90бу,90бу] else.
  hv_PhiThreshold = (HTuple(180).TupleRad())-(hv_IgnoreDirection*(HTuple(90).TupleRad()));
  hv_PhiDelta = 2*hv_PhiThreshold;
  //Correct angles that are too large.
  hv_PhiToCorrect = (hv_BBoxPhiNew.TupleGreaterElem(hv_PhiThreshold)).TupleFind(1);
  if (0 != (HTuple(int(hv_PhiToCorrect!=-1)).TupleAnd(int(hv_PhiToCorrect!=HTuple()))))
  {
    hv_NumCorrections = (((HTuple(hv_BBoxPhiNew[hv_PhiToCorrect])-hv_PhiThreshold)/hv_PhiDelta).TupleInt())+1;
    hv_BBoxPhiNew[hv_PhiToCorrect] = HTuple(hv_BBoxPhiNew[hv_PhiToCorrect])-(hv_NumCorrections*hv_PhiDelta);
  }
  //Correct angles that are too small.
  hv_PhiToCorrect = (hv_BBoxPhiNew.TupleLessEqualElem(-hv_PhiThreshold)).TupleFind(1);
  if (0 != (HTuple(int(hv_PhiToCorrect!=-1)).TupleAnd(int(hv_PhiToCorrect!=HTuple()))))
  {
    hv_NumCorrections = ((((HTuple(hv_BBoxPhiNew[hv_PhiToCorrect])+hv_PhiThreshold).TupleAbs())/hv_PhiDelta).TupleInt())+1;
    hv_BBoxPhiNew[hv_PhiToCorrect] = HTuple(hv_BBoxPhiNew[hv_PhiToCorrect])+(hv_NumCorrections*hv_PhiDelta);
  }
  //
  //Check that there are no invalid boxes.
  if (0 != (int((hv_BBoxRowNew.TupleLength())>0)))
  {
    hv_BoxesInvalid = ((hv_BBoxLength1New.TupleEqualElem(0)).TupleSum())+((hv_BBoxLength2New.TupleEqualElem(0)).TupleSum());
    if (0 != (int(hv_BoxesInvalid>0)))
    {
      GetDictTuple(hv_DLSample, "image_id", &hv_ImageId);
      throw HException(("An error has occurred during preprocessing image_id "+hv_ImageId)+HTuple(": Sample contains at least one box with zero-area, i.e. bbox_length1 == 0 or bbox_length2 == 0!"));
    }
  }
  SetDictTuple(hv_DLSample, "bbox_row", hv_BBoxRowNew);
  SetDictTuple(hv_DLSample, "bbox_col", hv_BBoxColNew);
  SetDictTuple(hv_DLSample, "bbox_length1", hv_BBoxLength1New);
  SetDictTuple(hv_DLSample, "bbox_length2", hv_BBoxLength2New);
  SetDictTuple(hv_DLSample, "bbox_phi", hv_BBoxPhiNew);
  SetDictTuple(hv_DLSample, "bbox_label_id", hv_BBoxLabelNew);
  //
  return;

}

// Chapter: Deep Learning / Model
// Short Description: Preprocess images for deep-learning-based training and inference. 
void preprocess_dl_model_images (HObject ho_Images, HObject *ho_ImagesPreprocessed, 
    HTuple hv_DLPreprocessParam)
{

  // Local iconic variables
  HObject  ho_ImagesScaled, ho_ImageSelected, ho_ImageScaled;
  HObject  ho_Channel, ho_ChannelScaled, ho_ThreeChannelImage;
  HObject  ho_SingleChannelImage;

  // Local control variables
  HTuple  hv_ImageWidth, hv_ImageHeight, hv_ImageNumChannels;
  HTuple  hv_ImageRangeMin, hv_ImageRangeMax, hv_DomainHandling;
  HTuple  hv_NormalizationType, hv_ModelType, hv_NumImages;
  HTuple  hv_Type, hv_NumMatches, hv_InputNumChannels, hv_OutputNumChannels;
  HTuple  hv_NumChannels1, hv_NumChannels3, hv_AreInputNumChannels1;
  HTuple  hv_AreInputNumChannels3, hv_AreInputNumChannels1Or3;
  HTuple  hv_ValidNumChannels, hv_ValidNumChannelsText, hv_ImageIndex;
  HTuple  hv_NumChannels, hv_ChannelIndex, hv_Min, hv_Max;
  HTuple  hv_Range, hv_Scale, hv_Shift, hv_MeanValues, hv_DeviationValues;
  HTuple  hv_UseDefaultNormalizationValues, hv_Exception;
  HTuple  hv_Indices, hv_RescaleRange, hv_CurrentNumChannels;
  HTuple  hv_DiffNumChannelsIndices, hv_Index, hv_DiffNumChannelsIndex;

  //
  //This procedure preprocesses the provided Images according to the parameters in
  //the dictionary DLPreprocessParam. Note that depending on the images, additional
  //preprocessing steps might be beneficial.
  //
  //Validate the preprocessing parameters.
  check_dl_preprocess_param(hv_DLPreprocessParam);
  //
  //Get the preprocessing parameters.
  GetDictTuple(hv_DLPreprocessParam, "image_width", &hv_ImageWidth);
  GetDictTuple(hv_DLPreprocessParam, "image_height", &hv_ImageHeight);
  GetDictTuple(hv_DLPreprocessParam, "image_num_channels", &hv_ImageNumChannels);
  GetDictTuple(hv_DLPreprocessParam, "image_range_min", &hv_ImageRangeMin);
  GetDictTuple(hv_DLPreprocessParam, "image_range_max", &hv_ImageRangeMax);
  GetDictTuple(hv_DLPreprocessParam, "domain_handling", &hv_DomainHandling);
  GetDictTuple(hv_DLPreprocessParam, "normalization_type", &hv_NormalizationType);
  GetDictTuple(hv_DLPreprocessParam, "model_type", &hv_ModelType);
  //
  //Validate the type of the input images.
  CountObj(ho_Images, &hv_NumImages);
  if (0 != (int(hv_NumImages==0)))
  {
    throw HException("Please provide some images to preprocess.");
  }
  GetImageType(ho_Images, &hv_Type);
  TupleRegexpTest(hv_Type, "byte|int|real", &hv_NumMatches);
  if (0 != (int(hv_NumMatches!=hv_NumImages)))
  {
    throw HException(HTuple("Please provide only images of type 'byte', 'int1', 'int2', 'uint2', 'int4', 'int8', or 'real'."));
  }
  //
  //Validate the number channels of the input images.
  CountChannels(ho_Images, &hv_InputNumChannels);
  hv_OutputNumChannels = HTuple(hv_NumImages,hv_ImageNumChannels);
  //Only for 'image_num_channels' 1 and 3 combinations of 1- and 3-channel images are allowed.
  if (0 != (HTuple(int(hv_ImageNumChannels==1)).TupleOr(int(hv_ImageNumChannels==3))))
  {
    hv_NumChannels1 = HTuple(hv_NumImages,1);
    hv_NumChannels3 = HTuple(hv_NumImages,3);
    hv_AreInputNumChannels1 = hv_InputNumChannels.TupleEqualElem(hv_NumChannels1);
    hv_AreInputNumChannels3 = hv_InputNumChannels.TupleEqualElem(hv_NumChannels3);
    hv_AreInputNumChannels1Or3 = hv_AreInputNumChannels1+hv_AreInputNumChannels3;
    hv_ValidNumChannels = int(hv_AreInputNumChannels1Or3==hv_NumChannels1);
    hv_ValidNumChannelsText = "Valid numbers of channels for the specified model are 1 or 3.";
  }
  else
  {
    hv_ValidNumChannels = int(hv_InputNumChannels==hv_OutputNumChannels);
    hv_ValidNumChannelsText = ("Valid number of channels for the specified model is "+hv_ImageNumChannels)+".";
  }
  if (0 != (hv_ValidNumChannels.TupleNot()))
  {
    throw HException("Please provide images with a valid number of channels. "+hv_ValidNumChannelsText);
  }
  //Preprocess the images.
  //
  //Apply the domain to the images.
  if (0 != (int(hv_DomainHandling==HTuple("full_domain"))))
  {
    FullDomain(ho_Images, &ho_Images);
  }
  else if (0 != (int(hv_DomainHandling==HTuple("crop_domain"))))
  {
    CropDomain(ho_Images, &ho_Images);
  }
  else if (0 != (HTuple(int(hv_DomainHandling==HTuple("keep_domain"))).TupleAnd(int(hv_ModelType==HTuple("anomaly_detection")))))
  {
    //Anomaly detection models accept the additional option 'keep_domain'.
  }
  else
  {
    throw HException("Unsupported parameter value for 'domain_handling'.");
  }
  //
  //Convert the images to real and zoom the images.
  //Zoom first to speed up if all image types are supported by zoom_image_size.
  if (0 != (int((hv_Type.TupleRegexpTest("int1|int4|int8"))==0)))
  {
    ZoomImageSize(ho_Images, &ho_Images, hv_ImageWidth, hv_ImageHeight, "constant");
    ConvertImageType(ho_Images, &ho_Images, "real");
  }
  else
  {
    ConvertImageType(ho_Images, &ho_Images, "real");
    ZoomImageSize(ho_Images, &ho_Images, hv_ImageWidth, hv_ImageHeight, "constant");
  }
  //
  if (0 != (int(hv_NormalizationType==HTuple("all_channels"))))
  {
    //Scale for each image the gray values of all channels to ImageRangeMin-ImageRangeMax.
    GenEmptyObj(&ho_ImagesScaled);
    {
    HTuple end_val74 = hv_NumImages;
    HTuple step_val74 = 1;
    for (hv_ImageIndex=1; hv_ImageIndex.Continue(end_val74, step_val74); hv_ImageIndex += step_val74)
    {
      SelectObj(ho_Images, &ho_ImageSelected, hv_ImageIndex);
      CountChannels(ho_ImageSelected, &hv_NumChannels);
      GenEmptyObj(&ho_ImageScaled);
      {
      HTuple end_val78 = hv_NumChannels;
      HTuple step_val78 = 1;
      for (hv_ChannelIndex=1; hv_ChannelIndex.Continue(end_val78, step_val78); hv_ChannelIndex += step_val78)
      {
        AccessChannel(ho_ImageSelected, &ho_Channel, hv_ChannelIndex);
        MinMaxGray(ho_Channel, ho_Channel, 0, &hv_Min, &hv_Max, &hv_Range);
        if (0 != (int((hv_Max-hv_Min)==0)))
        {
          hv_Scale = 1;
        }
        else
        {
          hv_Scale = (hv_ImageRangeMax-hv_ImageRangeMin)/(hv_Max-hv_Min);
        }
        hv_Shift = ((-hv_Scale)*hv_Min)+hv_ImageRangeMin;
        ScaleImage(ho_Channel, &ho_ChannelScaled, hv_Scale, hv_Shift);
        AppendChannel(ho_ImageScaled, ho_ChannelScaled, &ho_ImageScaled);
      }
      }
      ConcatObj(ho_ImagesScaled, ho_ImageScaled, &ho_ImagesScaled);
    }
    }
    ho_Images = ho_ImagesScaled;
  }
  else if (0 != (int(hv_NormalizationType==HTuple("first_channel"))))
  {
    //Scale for each image the gray values of first channel to ImageRangeMin-ImageRangeMax.
    GenEmptyObj(&ho_ImagesScaled);
    {
    HTuple end_val96 = hv_NumImages;
    HTuple step_val96 = 1;
    for (hv_ImageIndex=1; hv_ImageIndex.Continue(end_val96, step_val96); hv_ImageIndex += step_val96)
    {
      SelectObj(ho_Images, &ho_ImageSelected, hv_ImageIndex);
      MinMaxGray(ho_ImageSelected, ho_ImageSelected, 0, &hv_Min, &hv_Max, &hv_Range);
      if (0 != (int((hv_Max-hv_Min)==0)))
      {
        hv_Scale = 1;
      }
      else
      {
        hv_Scale = (hv_ImageRangeMax-hv_ImageRangeMin)/(hv_Max-hv_Min);
      }
      hv_Shift = ((-hv_Scale)*hv_Min)+hv_ImageRangeMin;
      ScaleImage(ho_ImageSelected, &ho_ImageSelected, hv_Scale, hv_Shift);
      ConcatObj(ho_ImagesScaled, ho_ImageSelected, &ho_ImagesScaled);
    }
    }
    ho_Images = ho_ImagesScaled;
  }
  else if (0 != (int(hv_NormalizationType==HTuple("constant_values"))))
  {
    //Scale for each image the gray values of all channels to the corresponding channel DeviationValues[].
    try
    {
      GetDictTuple(hv_DLPreprocessParam, "mean_values_normalization", &hv_MeanValues);
      GetDictTuple(hv_DLPreprocessParam, "deviation_values_normalization", &hv_DeviationValues);
      hv_UseDefaultNormalizationValues = 0;
    }
    // catch (Exception) 
    catch (HException &HDevExpDefaultException)
    {
      HDevExpDefaultException.ToHTuple(&hv_Exception);
      hv_MeanValues.Clear();
      hv_MeanValues[0] = 123.675;
      hv_MeanValues[1] = 116.28;
      hv_MeanValues[2] = 103.53;
      hv_DeviationValues.Clear();
      hv_DeviationValues[0] = 58.395;
      hv_DeviationValues[1] = 57.12;
      hv_DeviationValues[2] = 57.375;
      hv_UseDefaultNormalizationValues = 1;
    }
    GenEmptyObj(&ho_ImagesScaled);
    {
    HTuple end_val121 = hv_NumImages;
    HTuple step_val121 = 1;
    for (hv_ImageIndex=1; hv_ImageIndex.Continue(end_val121, step_val121); hv_ImageIndex += step_val121)
    {
      SelectObj(ho_Images, &ho_ImageSelected, hv_ImageIndex);
      CountChannels(ho_ImageSelected, &hv_NumChannels);
      //Ensure that the number of channels is equal |DeviationValues| and |MeanValues|
      if (0 != hv_UseDefaultNormalizationValues)
      {
        if (0 != (int(hv_NumChannels==1)))
        {
          Compose3(ho_ImageSelected, ho_ImageSelected, ho_ImageSelected, &ho_ImageSelected
              );
          CountChannels(ho_ImageSelected, &hv_NumChannels);
        }
        else if (0 != (int(hv_NumChannels!=3)))
        {
          throw HException("Using default values for normalization type 'constant_values' is allowed only for 1- and 3-channel images.");
        }
      }
      if (0 != (HTuple(int((hv_MeanValues.TupleLength())!=hv_NumChannels)).TupleOr(int((hv_DeviationValues.TupleLength())!=hv_NumChannels))))
      {
        throw HException("The length of mean and deviation values for normalization type 'constant_values' have to be the same size as the number of channels of the image.");
      }
      GenEmptyObj(&ho_ImageScaled);
      {
      HTuple end_val137 = hv_NumChannels;
      HTuple step_val137 = 1;
      for (hv_ChannelIndex=1; hv_ChannelIndex.Continue(end_val137, step_val137); hv_ChannelIndex += step_val137)
      {
        AccessChannel(ho_ImageSelected, &ho_Channel, hv_ChannelIndex);
        hv_Scale = 1.0/HTuple(hv_DeviationValues[hv_ChannelIndex-1]);
        hv_Shift = (-hv_Scale)*HTuple(hv_MeanValues[hv_ChannelIndex-1]);
        ScaleImage(ho_Channel, &ho_ChannelScaled, hv_Scale, hv_Shift);
        AppendChannel(ho_ImageScaled, ho_ChannelScaled, &ho_ImageScaled);
      }
      }
      ConcatObj(ho_ImagesScaled, ho_ImageScaled, &ho_ImagesScaled);
    }
    }
    ho_Images = ho_ImagesScaled;
  }
  else if (0 != (int(hv_NormalizationType==HTuple("none"))))
  {
    TupleFind(hv_Type, "byte", &hv_Indices);
    if (0 != (int(hv_Indices!=-1)))
    {
      //Shift the gray values from [0-255] to the expected range for byte images.
      hv_RescaleRange = (hv_ImageRangeMax-hv_ImageRangeMin)/255.0;
      SelectObj(ho_Images, &ho_ImageSelected, hv_Indices+1);
      ScaleImage(ho_ImageSelected, &ho_ImageSelected, hv_RescaleRange, hv_ImageRangeMin);
      ReplaceObj(ho_Images, ho_ImageSelected, &ho_Images, hv_Indices+1);
    }
  }
  else if (0 != (int(hv_NormalizationType!=HTuple("none"))))
  {
    throw HException("Unsupported parameter value for 'normalization_type'");
  }
  //
  //Ensure that the number of channels of the resulting images is consistent with the
  //number of channels of the given model. The only exceptions that are adapted below
  //are combinations of 1- and 3-channel images if ImageNumChannels is either 1 or 3.
  if (0 != (HTuple(int(hv_ImageNumChannels==1)).TupleOr(int(hv_ImageNumChannels==3))))
  {
    CountChannels(ho_Images, &hv_CurrentNumChannels);
    TupleFind(hv_CurrentNumChannels.TupleNotEqualElem(hv_OutputNumChannels), 1, &hv_DiffNumChannelsIndices);
    if (0 != (int(hv_DiffNumChannelsIndices!=-1)))
    {
      {
      HTuple end_val167 = (hv_DiffNumChannelsIndices.TupleLength())-1;
      HTuple step_val167 = 1;
      for (hv_Index=0; hv_Index.Continue(end_val167, step_val167); hv_Index += step_val167)
      {
        hv_DiffNumChannelsIndex = HTuple(hv_DiffNumChannelsIndices[hv_Index]);
        hv_ImageIndex = hv_DiffNumChannelsIndex+1;
        hv_NumChannels = HTuple(hv_CurrentNumChannels[hv_ImageIndex-1]);
        SelectObj(ho_Images, &ho_ImageSelected, hv_ImageIndex);
        if (0 != (HTuple(int(hv_NumChannels==1)).TupleAnd(int(hv_ImageNumChannels==3))))
        {
          //Conversion from 1- to 3-channel image required
          Compose3(ho_ImageSelected, ho_ImageSelected, ho_ImageSelected, &ho_ThreeChannelImage
              );
          ReplaceObj(ho_Images, ho_ThreeChannelImage, &ho_Images, hv_ImageIndex);
        }
        else if (0 != (HTuple(int(hv_NumChannels==3)).TupleAnd(int(hv_ImageNumChannels==1))))
        {
          //Conversion from 3- to 1-channel image required
          Rgb1ToGray(ho_ImageSelected, &ho_SingleChannelImage);
          ReplaceObj(ho_Images, ho_SingleChannelImage, &ho_Images, hv_ImageIndex);
        }
        else
        {
          throw HException(((("Unexpected error adapting the number of channels. The number of channels of the resulting image is "+hv_NumChannels)+HTuple(", but the number of channels of the model is "))+hv_ImageNumChannels)+".");
        }
      }
      }
    }
  }
  //
  //Write preprocessed images to output variable.
  (*ho_ImagesPreprocessed) = ho_Images;
  //
  return;
}

// Chapter: Deep Learning / Semantic Segmentation
// Short Description: Preprocess segmentation and weight images for deep-learning-based segmentation training and inference. 
void preprocess_dl_model_segmentations (HObject ho_ImagesRaw, HObject ho_Segmentations, 
    HObject *ho_SegmentationsPreprocessed, HTuple hv_DLPreprocessParam)
{

  // Local iconic variables
  HObject  ho_Domain, ho_SelectedSeg, ho_SelectedDomain;

  // Local control variables
  HTuple  hv_NumberImages, hv_NumberSegmentations;
  HTuple  hv_Width, hv_Height, hv_WidthSeg, hv_HeightSeg;
  HTuple  hv_DLModelType, hv_ImageWidth, hv_ImageHeight, hv_ImageNumChannels;
  HTuple  hv_ImageRangeMin, hv_ImageRangeMax, hv_DomainHandling;
  HTuple  hv_SetBackgroundID, hv_ClassesToBackground, hv_IgnoreClassIDs;
  HTuple  hv_IsInt, hv_IndexImage, hv_ImageWidthRaw, hv_ImageHeightRaw;
  HTuple  hv_EqualWidth, hv_EqualHeight, hv_Type, hv_EqualReal;

  //
  //This procedure preprocesses the segmentation or weight images
  //given by Segmentations so that they can be handled by
  //train_dl_model_batch and apply_dl_model.
  //
  //Check input data.
  //Examine number of images.
  CountObj(ho_ImagesRaw, &hv_NumberImages);
  CountObj(ho_Segmentations, &hv_NumberSegmentations);
  if (0 != (int(hv_NumberImages!=hv_NumberSegmentations)))
  {
    throw HException("Equal number of images given in ImagesRaw and Segmentations required");
  }
  //Size of images.
  GetImageSize(ho_ImagesRaw, &hv_Width, &hv_Height);
  GetImageSize(ho_Segmentations, &hv_WidthSeg, &hv_HeightSeg);
  if (0 != (HTuple(int(hv_Width!=hv_WidthSeg)).TupleOr(int(hv_Height!=hv_HeightSeg))))
  {
    throw HException("Equal size of the images given in ImagesRaw and Segmentations required.");
  }
  //Check the validity of the preprocessing parameters.
  check_dl_preprocess_param(hv_DLPreprocessParam);
  //
  //Get the relevant preprocessing parameters.
  GetDictTuple(hv_DLPreprocessParam, "model_type", &hv_DLModelType);
  GetDictTuple(hv_DLPreprocessParam, "image_width", &hv_ImageWidth);
  GetDictTuple(hv_DLPreprocessParam, "image_height", &hv_ImageHeight);
  GetDictTuple(hv_DLPreprocessParam, "image_num_channels", &hv_ImageNumChannels);
  GetDictTuple(hv_DLPreprocessParam, "image_range_min", &hv_ImageRangeMin);
  GetDictTuple(hv_DLPreprocessParam, "image_range_max", &hv_ImageRangeMax);
  GetDictTuple(hv_DLPreprocessParam, "domain_handling", &hv_DomainHandling);
  //Segmentation specific parameters.
  GetDictTuple(hv_DLPreprocessParam, "set_background_id", &hv_SetBackgroundID);
  GetDictTuple(hv_DLPreprocessParam, "class_ids_background", &hv_ClassesToBackground);
  GetDictTuple(hv_DLPreprocessParam, "ignore_class_ids", &hv_IgnoreClassIDs);
  //
  //Check the input parameter for setting the background ID.
  if (0 != (int(hv_SetBackgroundID!=HTuple())))
  {
    //Check that the model is a segmentation model.
    if (0 != (int(hv_DLModelType!=HTuple("segmentation"))))
    {
      throw HException("Setting class IDs to background is only implemented for segmentation.");
    }
    //Check the background ID.
    TupleIsIntElem(hv_SetBackgroundID, &hv_IsInt);
    if (0 != (int((hv_SetBackgroundID.TupleLength())!=1)))
    {
      throw HException("Only one class_id as 'set_background_id' allowed.");
    }
    else if (0 != (hv_IsInt.TupleNot()))
    {
      //Given class_id has to be of type int.
      throw HException("The class_id given as 'set_background_id' has to be of type int.");
    }
    //Check the values of ClassesToBackground.
    if (0 != (int((hv_ClassesToBackground.TupleLength())==0)))
    {
      //Check that the given classes are of length > 0.
      throw HException(HTuple("If 'set_background_id' is given, 'class_ids_background' must at least contain this class ID."));
    }
    else if (0 != (int((hv_ClassesToBackground.TupleIntersection(hv_IgnoreClassIDs))!=HTuple())))
    {
      //Check that class_ids_background is not included in the ignore_class_ids of the DLModel.
      throw HException("The given 'class_ids_background' must not be included in the 'ignore_class_ids' of the model.");
    }
  }
  //
  //Domain handling of the image to be preprocessed.
  //
  if (0 != (int(hv_DomainHandling==HTuple("full_domain"))))
  {
    FullDomain(ho_Segmentations, &ho_Segmentations);
  }
  else if (0 != (int(hv_DomainHandling==HTuple("crop_domain"))))
  {
    //If the domain should be cropped the domain has to be transferred
    //from the raw image to the segmentation image.
    GetDomain(ho_ImagesRaw, &ho_Domain);
    {
    HTuple end_val66 = hv_NumberImages;
    HTuple step_val66 = 1;
    for (hv_IndexImage=1; hv_IndexImage.Continue(end_val66, step_val66); hv_IndexImage += step_val66)
    {
      SelectObj(ho_Segmentations, &ho_SelectedSeg, hv_IndexImage);
      SelectObj(ho_Domain, &ho_SelectedDomain, hv_IndexImage);
      ChangeDomain(ho_SelectedSeg, ho_SelectedDomain, &ho_SelectedSeg);
      ReplaceObj(ho_Segmentations, ho_SelectedSeg, &ho_Segmentations, hv_IndexImage);
    }
    }
    CropDomain(ho_Segmentations, &ho_Segmentations);
  }
  else
  {
    throw HException("Unsupported parameter value for 'domain_handling'");
  }
  //
  //Preprocess the segmentation images.
  //
  //Set all background classes to the given background class ID.
  if (0 != (int(hv_SetBackgroundID!=HTuple())))
  {
    reassign_pixel_values(ho_Segmentations, &ho_Segmentations, hv_ClassesToBackground, 
        hv_SetBackgroundID);
  }
  //
  //Zoom images only if they have a different size than the specified size.
  GetImageSize(ho_Segmentations, &hv_ImageWidthRaw, &hv_ImageHeightRaw);
  hv_EqualWidth = hv_ImageWidth.TupleEqualElem(hv_ImageWidthRaw);
  hv_EqualHeight = hv_ImageHeight.TupleEqualElem(hv_ImageHeightRaw);
  if (0 != (HTuple(int((hv_EqualWidth.TupleMin())==0)).TupleOr(int((hv_EqualHeight.TupleMin())==0))))
  {
    ZoomImageSize(ho_Segmentations, &ho_Segmentations, hv_ImageWidth, hv_ImageHeight, 
        "nearest_neighbor");
  }
  //
  //Check the type of the input images
  //and convert if necessary.
  GetImageType(ho_Segmentations, &hv_Type);
  hv_EqualReal = hv_Type.TupleEqualElem("real");
  //
  if (0 != (int((hv_EqualReal.TupleMin())==0)))
  {
    //Convert the image type to 'real',
    //because the model expects 'real' images.
    ConvertImageType(ho_Segmentations, &ho_Segmentations, "real");
  }
  //
  //Write preprocessed Segmentations to output variable.
  (*ho_SegmentationsPreprocessed) = ho_Segmentations;
  return;
}

// Chapter: Deep Learning / Model
// Short Description: Preprocess given DLSamples according to the preprocessing parameters given in DLPreprocessParam. 
void preprocess_dl_samples (HTuple hv_DLSampleBatch, HTuple hv_DLPreprocessParam)
{

  // Local iconic variables
  HObject  ho_ImageRaw, ho_ImagePreprocessed, ho_AnomalyImageRaw;
  HObject  ho_AnomalyImagePreprocessed, ho_SegmentationRaw;
  HObject  ho_SegmentationPreprocessed;

  // Local control variables
  HTuple  hv_ModelType, hv_SampleIndex, hv_ImageExists;
  HTuple  hv_KeysExists, hv_AnomalyParamExist, hv_Rectangle1ParamExist;
  HTuple  hv_Rectangle2ParamExist, hv_SegmentationParamExist;

  //
  //This procedure preprocesses all images of the sample dictionaries in the tuple DLSampleBatch.
  //The images are preprocessed according to the parameters provided in DLPreprocessParam.
  //
  //Check the validity of the preprocessing parameters.
  //The procedure check_dl_preprocess_param might change DLPreprocessParam. To avoid race
  //conditions when preprocess_dl_samples is used from multiple threads with the same
  //DLPreprocessParam dictionary, work on a copy.
  CopyDict(hv_DLPreprocessParam, HTuple(), HTuple(), &hv_DLPreprocessParam);
  check_dl_preprocess_param(hv_DLPreprocessParam);
  GetDictTuple(hv_DLPreprocessParam, "model_type", &hv_ModelType);
  //
  //Preprocess the sample entries.
  //
  {
  HTuple end_val14 = (hv_DLSampleBatch.TupleLength())-1;
  HTuple step_val14 = 1;
  for (hv_SampleIndex=0; hv_SampleIndex.Continue(end_val14, step_val14); hv_SampleIndex += step_val14)
  {
    //
    //Check the existence of the sample keys.
    GetDictParam(HTuple(hv_DLSampleBatch[hv_SampleIndex]), "key_exists", "image", 
        &hv_ImageExists);
    //
    //Preprocess the images.
    if (0 != hv_ImageExists)
    {
      //
      //Get the image.
      GetDictObject(&ho_ImageRaw, HTuple(hv_DLSampleBatch[hv_SampleIndex]), "image");
      //
      //Preprocess the image.
      preprocess_dl_model_images(ho_ImageRaw, &ho_ImagePreprocessed, hv_DLPreprocessParam);
      //
      //Replace the image in the dictionary.
      SetDictObject(ho_ImagePreprocessed, HTuple(hv_DLSampleBatch[hv_SampleIndex]), 
          "image");
      //
      //Check existence of model specific sample keys:
      //- bbox_row1 for 'rectangle1'
      //- bbox_phi for 'rectangle2'
      //- segmentation_image for 'semantic segmentation'
      GetDictParam(HTuple(hv_DLSampleBatch[hv_SampleIndex]), "key_exists", (((HTuple("anomaly_ground_truth").Append("bbox_row1")).Append("bbox_phi")).Append("segmentation_image")), 
          &hv_KeysExists);
      hv_AnomalyParamExist = ((const HTuple&)hv_KeysExists)[0];
      hv_Rectangle1ParamExist = ((const HTuple&)hv_KeysExists)[1];
      hv_Rectangle2ParamExist = ((const HTuple&)hv_KeysExists)[2];
      hv_SegmentationParamExist = ((const HTuple&)hv_KeysExists)[3];
      //
      //Preprocess the anomaly ground truth if present.
      if (0 != hv_AnomalyParamExist)
      {
        //
        //Get the anomaly image.
        GetDictObject(&ho_AnomalyImageRaw, HTuple(hv_DLSampleBatch[hv_SampleIndex]), 
            "anomaly_ground_truth");
        //
        //Preprocess the anomaly image.
        preprocess_dl_model_anomaly(ho_AnomalyImageRaw, &ho_AnomalyImagePreprocessed, 
            hv_DLPreprocessParam);
        //
        //Set preprocessed anomaly image.
        SetDictObject(ho_AnomalyImagePreprocessed, HTuple(hv_DLSampleBatch[hv_SampleIndex]), 
            "anomaly_ground_truth");
      }
      //
      //Preprocess depending on the model type.
      //If bounding boxes are given, rescale them as well.
      if (0 != hv_Rectangle1ParamExist)
      {
        //
        //Preprocess the bounding boxes of type 'rectangle1'.
        preprocess_dl_model_bbox_rect1(ho_ImageRaw, HTuple(hv_DLSampleBatch[hv_SampleIndex]), 
            hv_DLPreprocessParam);
      }
      else if (0 != hv_Rectangle2ParamExist)
      {
        //
        //Preprocess the bounding boxes of type 'rectangle2'.
        preprocess_dl_model_bbox_rect2(ho_ImageRaw, HTuple(hv_DLSampleBatch[hv_SampleIndex]), 
            hv_DLPreprocessParam);
      }
      //
      //Preprocess the segmentation image if present.
      if (0 != hv_SegmentationParamExist)
      {
        //
        //Get the segmentation image.
        GetDictObject(&ho_SegmentationRaw, HTuple(hv_DLSampleBatch[hv_SampleIndex]), 
            "segmentation_image");
        //
        //Preprocess the segmentation image.
        preprocess_dl_model_segmentations(ho_ImageRaw, ho_SegmentationRaw, &ho_SegmentationPreprocessed, 
            hv_DLPreprocessParam);
        //
        //Set preprocessed segmentation image.
        SetDictObject(ho_SegmentationPreprocessed, HTuple(hv_DLSampleBatch[hv_SampleIndex]), 
            "segmentation_image");
      }
    }
    else
    {
      throw HException((HTuple("All samples processed need to include an image, but the sample with index ")+hv_SampleIndex)+" does not.");
    }
  }
  }
  //
  return;
}

// Chapter: Tuple / Conversion
// Short Description: Prints a tuple of values to a string. 
void pretty_print_tuple (HTuple hv_Tuple, HTuple *hv_TupleStr)
{

  // Local iconic variables

  // Local control variables
  HTuple  hv_J;

  //
  //This procedure prints a tuple of values to a string.
  //
  if (0 != (int((hv_Tuple.TupleLength())>1)))
  {
    (*hv_TupleStr) = "[";
    {
    HTuple end_val5 = (hv_Tuple.TupleLength())-1;
    HTuple step_val5 = 1;
    for (hv_J=0; hv_J.Continue(end_val5, step_val5); hv_J += step_val5)
    {
      if (0 != (int(hv_J>0)))
      {
        (*hv_TupleStr) += HTuple(HTuple(","));
      }
      (*hv_TupleStr) += HTuple(hv_Tuple[hv_J]);
    }
    }
    (*hv_TupleStr) += HTuple("]");
  }
  else
  {
    (*hv_TupleStr) = hv_Tuple;
  }
  return;
}

// Chapter: System / Multithreading
void read_dict_object (HObject *ho_ObjectData, HTuple hv_DictHandle, HTuple hv_Key)
{

  // Local control variables
  HTuple  hv_Exception;

  try
  {
    GetDictObject(&(*ho_ObjectData), hv_DictHandle, hv_Key);
  }
  // catch (Exception) 
  catch (HException &HDevExpDefaultException)
  {
    HDevExpDefaultException.ToHTuple(&hv_Exception);
    throw HException((("The key "+hv_Key)+" is missing from the dict ")+hv_DictHandle);
  }
  return;
}

// Chapter: System / Multithreading
void read_dict_tuple (HTuple hv_DictHandle, HTuple hv_Key, HTuple *hv_TupleData)
{

  // Local control variables
  HTuple  hv_Exception;

  try
  {
    GetDictTuple(hv_DictHandle, hv_Key, &(*hv_TupleData));
  }
  // catch (Exception) 
  catch (HException &HDevExpDefaultException)
  {
    HDevExpDefaultException.ToHTuple(&hv_Exception);
    throw HException((("The key "+hv_Key)+" is missing from the dict ")+hv_DictHandle);
  }
  return;
}

// Chapter: Deep Learning / Anomaly Detection
// Short Description: Generate a DLDataset dictionary for anomaly detection. 
void read_dl_dataset_anomaly (HTuple hv_ImageDir, HTuple hv_AnomalyDir, HTuple hv_ImageList, 
    HTuple hv_AnomalyList, HTuple hv_GenParam, HTuple *hv_DLDataset)
{

  // Local iconic variables

  // Local control variables
  HTuple  hv_GenParamCopy, hv_Keys, hv_KeyIndex;
  HTuple  hv_CurrentKey, hv_AnomalySubDirs, hv_FileNameAnomalyOnly;
  HTuple  hv_AnomalyDirGiven, hv_LabelList, hv_NoAnomalyIndices;
  HTuple  hv_ImageDirSep, hv_AnomalyDirSep, hv_AnomalyFileIndices;
  HTuple  hv_ClassNames, hv_ClassIDs, hv_NumSamples, hv_ThreadNum;
  HTuple  hv_ThreadSys, hv_NumSamplesPerThread, hv_ThreadIndex;
  HTuple  hv_Start, hv_End, hv_ThreadIds, hv_Samples, hv_Index;
  HTupleVector  hvec_VProcThreads(1), hvec_SamplesOutput(1);
  HTupleVector  hvec_Exceptions(1);

  // +++ Threading variables 
  HDevThread*         hcppthread_handle;
  HDevThreadContext   hcppthread_context; // <-signals begin of procedure

  //
  //This procedure creates the dictionary DLDataset which serves as an input for deep-learning-based anomaly detection models.
  //Information needed to create this dictionary is given through the input parameters.
  //
  //The output dictionary DLDataset has the following structure:
  //
  //DLDataset
  //{
  //    'image_dir'         : Common base path of all images
  //    'anomaly_dir'       : Common base path of all anomaly annotation files (ground truth annotations indicating anomalies in the images)
  //    'class_names'[]     : Tuple of class names ['ok','nok']
  //    'class_ids'[]       : Tuple of class ids [0,1]
  //    'samples'[]         : Tuple of dictionaries
  //    {
  //        'image_file_name'           : File path relative to 'image_dir' (including the file name)
  //        'anomaly_file_name'         : File path relative to 'anomaly_dir' (including the file name), contains a file with ground truth annotations
  //        'image_id'                  : Unique image ID
  //        'anomaly_label'             : Label on image level : 'ok'/'nok'
  //    }
  //}
  //
  //Images are not required to have a anomaly ground truth file. However, images that do have a
  //corresponding annotation file can be evaluated quantitatively later. Note: Unlike in
  //read_dl_dataset_classification, the folder name is not directly converted into the
  //'anomaly_label'. For images without anomalies (images in the good/ok directory), the label
  //is set to 'ok' and for all images containing anomalies (images in any directory except for
  //good/ok), the label is set to the general label 'nok'.
  //
  //Convert Anomaly Detection specific keys to general keys.
  hv_GenParamCopy = HTuple();
  if (0 != (int(hv_GenParam!=HTuple())))
  {
    CopyDict(hv_GenParam, HTuple(), HTuple(), &hv_GenParamCopy);
    GetDictParam(hv_GenParamCopy, "keys", HTuple(), &hv_Keys);
    {
    HTuple end_val33 = (hv_Keys.TupleLength())-1;
    HTuple step_val33 = 1;
    for (hv_KeyIndex=0; hv_KeyIndex.Continue(end_val33, step_val33); hv_KeyIndex += step_val33)
    {
      hv_CurrentKey = HTuple(hv_Keys[hv_KeyIndex]);
      if (0 != (int(hv_CurrentKey==HTuple("anomaly_sub_dirs"))))
      {
        GetDictTuple(hv_GenParamCopy, hv_CurrentKey, &hv_AnomalySubDirs);
        RemoveDictKey(hv_GenParamCopy, hv_CurrentKey);
        SetDictTuple(hv_GenParamCopy, "annotation_sub_dirs", hv_AnomalySubDirs);
      }
      else if (0 != (int(hv_CurrentKey==HTuple("file_name_anomaly_only"))))
      {
        GetDictTuple(hv_GenParamCopy, hv_CurrentKey, &hv_FileNameAnomalyOnly);
        RemoveDictKey(hv_GenParamCopy, hv_CurrentKey);
        SetDictTuple(hv_GenParamCopy, "file_name_annotation_only", hv_FileNameAnomalyOnly);
      }
    }
    }
  }
  //
  //Sanity checks of inputs.
  //
  //ImageDir has to be a single string.
  if (0 != (int((hv_ImageDir.TupleLength())!=1)))
  {
    throw HException("Exactly one base path is required as input for ImageDir");
  }
  else if (0 != ((hv_ImageDir.TupleIsString()).TupleNot()))
  {
    throw HException("ImageDir has to be a string");
  }
  //
  //AnomalyDir has to be a single string.
  hv_AnomalyDirGiven = 0;
  if (0 != (int((hv_AnomalyDir.TupleLength())>1)))
  {
    throw HException("Exactly one base path is required as input for AnomalyDir");
  }
  else if (0 != (int((hv_AnomalyDir.TupleLength())==1)))
  {
    if (0 != ((hv_AnomalyDir.TupleIsString()).TupleNot()))
    {
      throw HException("AnomalyDir has to be a string");
    }
    hv_AnomalyDirGiven = 1;
  }
  //
  //Check AnomalyList in case it is not empty.
  if (0 != (int(hv_AnomalyList!=HTuple())))
  {
    //Check if ImageList and AnomalyList have the same length.
    if (0 != (int((hv_ImageList.TupleLength())!=(hv_AnomalyList.TupleLength()))))
    {
      throw HException("AnomalyList must be empty or have the same length as ImageList");
    }
    else if (0 != (int(((hv_AnomalyList.TupleNotEqualElem("")).TupleMax())==0)))
    {
      throw HException("Please provide at least one valid anomaly file in AnomalyList or set the parameter to []");
    }
  }
  //
  //Prepare the image lists.
  //
  //Convert backslashes to forward slashes for unified processing.
  TupleRegexpReplace(hv_ImageDir, (HTuple("\\\\+").Append("replace_all")), "/", &hv_ImageDir);
  TupleRegexpReplace(hv_AnomalyDir, (HTuple("\\\\+").Append("replace_all")), "/", 
      &hv_AnomalyDir);
  TupleRegexpReplace(hv_ImageList, (HTuple("\\\\+").Append("replace_all")), "/", 
      &hv_ImageList);
  TupleRegexpReplace(hv_AnomalyList, (HTuple("\\\\+").Append("replace_all")), "/", 
      &hv_AnomalyList);
  //
  //Replace any forward slashes at the end.
  TupleRegexpReplace(hv_ImageDir, "/$", "", &hv_ImageDir);
  TupleRegexpReplace(hv_AnomalyDir, "/$", "", &hv_AnomalyDir);
  //Remove possible (not starting) double slashes.
  if (0 != (int((hv_ImageDir.TupleRegexpMatch("(.+)//"))!=HTuple(""))))
  {
    hv_ImageDir = hv_ImageDir.TupleRegexpReplace("(.+)//","$1/");
  }
  if (0 != (int((hv_AnomalyDir.TupleRegexpMatch("(.+)//"))!=HTuple(""))))
  {
    hv_AnomalyDir = hv_AnomalyDir.TupleRegexpReplace("(.+)//","$1/");
  }
  //
  //If no AnomalyList is given, create it out of the given ImageList or create both lists.
  if (0 != (int(hv_AnomalyList==HTuple())))
  {
    //List given images (or all images if none given) and their corresponding labels,
    //as well as their anomaly annotation files (if present).
    list_image_and_annotation_files("anomaly_detection", hv_ImageDir, hv_AnomalyDir, 
        hv_ImageList, hv_GenParamCopy, &hv_ImageList, &hv_LabelList, &hv_AnomalyList);
  }
  else
  {
    //Make sure that ImageList does not contain ImageDir.
    TupleRegexpReplace(hv_ImageList, ".*?"+hv_ImageDir, "", &hv_ImageList);
    TupleRegexpReplace(hv_ImageList, "^/", "", &hv_ImageList);
    //Get ground truth labels from the image directory names.
    TupleRegexpMatch("/"+hv_ImageList, ".*/([^/]+)/[^/]*$", &hv_LabelList);
    //Make sure that AnomalyList does not contain AnomalyDir.
    TupleRegexpReplace(hv_AnomalyList, ".*?"+hv_AnomalyDir, "", &hv_AnomalyList);
    TupleRegexpReplace(hv_AnomalyList, "^/", "", &hv_AnomalyList);
  }
  //
  //Check that for a given anomaly directory at least one annotation file is found.
  hv_NoAnomalyIndices = hv_AnomalyList.TupleFind("");
  if (0 != (HTuple(int((hv_NoAnomalyIndices.TupleLength())==(hv_ImageList.TupleLength()))).TupleAnd(hv_AnomalyDirGiven)))
  {
    throw HException(("Error: Could not find any matching anomaly regions in folder: \""+hv_AnomalyDir)+"\"");
  }
  //
  //If ImageDir or AnomalyDir is empty, we omit the path separator.
  if (0 != (int((hv_ImageDir.TupleStrlen())==0)))
  {
    hv_ImageDirSep = "";
  }
  else
  {
    hv_ImageDirSep = "/";
  }
  if (0 != (int((hv_AnomalyDir.TupleStrlen())==0)))
  {
    hv_AnomalyDirSep = "";
  }
  else
  {
    hv_AnomalyDirSep = "/";
  }
  //
  //Check if all images exist.
  images_exist((hv_ImageDir+hv_ImageDirSep)+hv_ImageList);
  //Check if all anomaly files exist.
  hv_AnomalyFileIndices = (hv_AnomalyList.TupleNotEqualElem("")).TupleFind(1);
  if (0 != (int(hv_AnomalyFileIndices!=-1)))
  {
    images_exist((hv_AnomalyDir+hv_AnomalyDirSep)+HTuple(hv_AnomalyList[hv_AnomalyFileIndices]));
  }
  //
  //Initialize the dictionary dataset.
  CreateDict(&(*hv_DLDataset));
  //
  //Set general information of the dataset.
  SetDictTuple((*hv_DLDataset), "image_dir", hv_ImageDir);
  if (0 != hv_AnomalyDirGiven)
  {
    SetDictTuple((*hv_DLDataset), "anomaly_dir", hv_AnomalyDir);
  }
  //We have the two anomaly labels 'ok' and 'nok'.
  hv_ClassNames.Clear();
  hv_ClassNames[0] = "ok";
  hv_ClassNames[1] = "nok";
  TupleGenSequence(0, (hv_ClassNames.TupleLength())-1, 1, &hv_ClassIDs);
  SetDictTuple((*hv_DLDataset), "class_names", hv_ClassNames);
  SetDictTuple((*hv_DLDataset), "class_ids", hv_ClassIDs);
  //
  //Get number of samples to set unique image ID for each sample according to its index in ImageList.
  hv_NumSamples = hv_ImageList.TupleLength();
  //
  //Create samples in parallel.
  //We use 8 threads, clearly less than the maximum number of HDevelop subthreads.
  hv_ThreadNum = 8;
  GetSystem("thread_num", &hv_ThreadSys);
  if (0 != (int(hv_ThreadNum>hv_ThreadSys)))
  {
    hv_ThreadNum = 1;
  }
  hv_NumSamplesPerThread = ((hv_NumSamples+hv_ThreadNum)-1)/hv_ThreadNum;
  {
  HTuple end_val163 = hv_ThreadNum-1;
  HTuple step_val163 = 1;
  for (hv_ThreadIndex=0; hv_ThreadIndex.Continue(end_val163, step_val163); hv_ThreadIndex += step_val163)
  {
    hv_Start = hv_ThreadIndex*hv_NumSamplesPerThread;
    hv_End = (hv_Start+hv_NumSamplesPerThread)-1;
    if (0 != (HTuple(int(hv_End<0)).TupleOr(int(hv_Start<0))))
    {
      throw HException("Negative indices when calculating samples per thread.");
    }
    //
    //Adapt indices for very last thread.
    if (0 != (int(hv_End>=hv_NumSamples)))
    {
      hv_End = hv_NumSamples-1;
      if (0 != (int(hv_End<hv_Start)))
      {
        break;
      }
      hv_NumSamplesPerThread = (hv_End-hv_Start)+1;
    }
    // Create a thread instance
    hcppthread_handle = new HDevThread(hcppthread_context,
                (void*)HDevExportCpp::_hcppthread_create_dl_dataset_anomaly_samples,6,2);
    // Set thread procedure call arguments 
    hcppthread_handle->SetInputCtrlParamTuple(0,hv_ImageList.TupleSelectRange(hv_Start,hv_End));
    hcppthread_handle->SetInputCtrlParamTuple(1,hv_LabelList.TupleSelectRange(hv_Start,hv_End));
    hcppthread_handle->SetInputCtrlParamTuple(2,hv_AnomalyList.TupleSelectRange(hv_Start,hv_End));
    hcppthread_handle->SetInputCtrlParamTuple(3,hv_AnomalyDirGiven);
    hcppthread_handle->SetInputCtrlParamTuple(4,hv_NumSamplesPerThread);
    hcppthread_handle->SetInputCtrlParamTuple(5,hv_Start);
    {HTuple at_idx;
    at_idx[0] = hv_ThreadIndex;
    hcppthread_handle->BindOutputCtrlParamVector(0,0,&hvec_SamplesOutput,at_idx);
    }
    {HTuple at_idx;
    at_idx[0] = hv_ThreadIndex;
    hcppthread_handle->BindOutputCtrlParamVector(1,0,&hvec_Exceptions,at_idx);
    }

    // Start proc line in thread
    hcppthread_handle->ParStart(&hvec_VProcThreads[hv_ThreadIndex].T());

    if (0 != (int(hv_End==(hv_NumSamples-1))))
    {
      break;
    }
  }
  }
  hv_ThreadIds = hvec_VProcThreads.ConvertVectorToTuple();
  HDevThread::ParJoin(hv_ThreadIds);
  hvec_VProcThreads.Clear();
  hv_ThreadIds = HTuple();
  hv_Samples = hvec_SamplesOutput.ConvertVectorToTuple();
  //
  //Check if create_dl_dataset_anomaly_samples returned
  //any exceptions, if yes then throw
  {
  HTuple end_val191 = HTuple(hvec_Exceptions.Length())-1;
  HTuple step_val191 = 1;
  for (hv_Index=0; hv_Index.Continue(end_val191, step_val191); hv_Index += step_val191)
  {
    if (0 != (int(hvec_Exceptions[hv_Index].T()!=HTuple())))
    {
      throw HException(hvec_Exceptions[hv_Index].T());
    }
  }
  }
  //
  //
  //Set the sample tuple.
  SetDictTuple((*hv_DLDataset), "samples", hv_Samples);
  //
  return;
}

// Chapter: Deep Learning / Model
// Short Description: Read the dictionaries DLSamples from files. 
void read_dl_samples (HTuple hv_DLDataset, HTuple hv_SampleIndices, HTuple *hv_DLSampleBatch)
{

  // Local iconic variables

  // Local control variables
  HTuple  hv_DatasetSamples, hv_MinIndex, hv_MaxIndex;
  HTuple  hv_KeyDirExists, hv_DictDir, hv_DLSamplesProc, hv_ImageIndex;
  HTuple  hv_KeyFileExists, hv_ImageID, hv_FileNameRelative;
  HTuple  hv_FileNameSample, hv_FileExists, hv_DictPath, hv_DLSample;
  HTuple  hv_Exception;

  //
  //This procedure reads a batch of DLSample dictionaries from disk.
  //The wanted samples are selected from a DLDataset by their indices.
  //The indices of the wanted samples are handed over in SampleIndices.
  //It returns the tuple of read-in dictionaries in DLSampleBatch.
  //
  //Sanity checks of inputs.
  //
  if (0 != (int((hv_SampleIndices.TupleLength())<=0)))
  {
    //Check the length of selected indices.
    throw HException(HTuple("Invalid length of SelectedIndices: ")+(hv_SampleIndices.TupleLength()));
  }
  else
  {
    //Get the samples from the DLDataset.
    GetDictTuple(hv_DLDataset, "samples", &hv_DatasetSamples);
    //Get min and max value of given indices.
    TupleMin(hv_SampleIndices, &hv_MinIndex);
    TupleMax(hv_SampleIndices, &hv_MaxIndex);
    if (0 != (HTuple(int(hv_MinIndex<0)).TupleOr(int(hv_MaxIndex>((hv_DatasetSamples.TupleLength())-1)))))
    {
      //Check the value range of the provided indices.
      throw HException("The given SampleIndices are not within the range of available samples in DLDataset.");
    }
  }
  //
  //Check if the key dlsample_dir is given.
  GetDictParam(hv_DLDataset, "key_exists", "dlsample_dir", &hv_KeyDirExists);
  //
  if (0 != hv_KeyDirExists)
  {
    //
    //Get the dlsample_dir.
    GetDictTuple(hv_DLDataset, "dlsample_dir", &hv_DictDir);
    //Get the samples to be processed.
    hv_DLSamplesProc = HTuple(hv_DatasetSamples[hv_SampleIndices]);
    //
    //Initialize DLSampleBatch tuple.
    (*hv_DLSampleBatch) = HTuple();
    //
    //Read in all DLSamples into the batch.
    {
    HTuple end_val37 = (hv_SampleIndices.TupleLength())-1;
    HTuple step_val37 = 1;
    for (hv_ImageIndex=0; hv_ImageIndex.Continue(end_val37, step_val37); hv_ImageIndex += step_val37)
    {
      //Check if dlsample key exist.
      GetDictParam(HTuple(hv_DLSamplesProc[hv_ImageIndex]), "key_exists", "dlsample_file_name", 
          &hv_KeyFileExists);
      //
      if (0 != (hv_KeyFileExists.TupleNot()))
      {
        //
        //If the key does not exist, check if a corresponding file exists.
        GetDictTuple(HTuple(hv_DLSamplesProc[hv_ImageIndex]), "image_id", &hv_ImageID);
        hv_FileNameRelative = hv_ImageID+"_dlsample.hdict";
        hv_FileNameSample = (hv_DictDir+"/")+hv_FileNameRelative;
        //
        FileExists(hv_FileNameSample, &hv_FileExists);
        if (0 != hv_FileExists)
        {
          //If it exists, create corresponding key.
          SetDictTuple(HTuple(hv_DLSamplesProc[hv_ImageIndex]), "dlsample_file_name", 
              hv_FileNameRelative);
        }
        else
        {
          //If not, throw an error.
          throw HException("No 'dlsample_file_name' and hdict file available for image ID "+hv_ImageID);
        }
        //
      }
      //
      //If dlsample dictionary is available for reading, read it.
      GetDictTuple(HTuple(hv_DLSamplesProc[hv_ImageIndex]), "dlsample_file_name", 
          &hv_DictPath);
      try
      {
        ReadDict((hv_DictDir+"/")+hv_DictPath, HTuple(), HTuple(), &hv_DLSample);
      }
      // catch (Exception) 
      catch (HException &HDevExpDefaultException)
      {
        HDevExpDefaultException.ToHTuple(&hv_Exception);
        throw HException((((("An error has occurred while reading "+hv_DictDir)+"/")+hv_DictPath)+HTuple(" , HALCON error # "))+HTuple(hv_Exception[0]));
      }
      //Add it to the DLSampleBatch.
      (*hv_DLSampleBatch) = (*hv_DLSampleBatch).TupleConcat(hv_DLSample);
      //
    }
    }
  }
  else
  {
    throw HException("The dataset needs to include the key 'dlsample_dir' for reading a DLSample from file.");
  }

  return;
}

// Chapter: Image / Manipulation
// Short Description: Changes a value of ValuesToChange in Image to NewValue. 
void reassign_pixel_values (HObject ho_Image, HObject *ho_ImageOut, HTuple hv_ValuesToChange, 
    HTuple hv_NewValue)
{

  // Local iconic variables
  HObject  ho_RegionToChange, ho_RegionClass;

  // Local control variables
  HTuple  hv_IndexReset;

  //
  //This procedure sets all pixels of Image
  //with the values given in ValuesToChange to the given value NewValue.
  //
  GenEmptyRegion(&ho_RegionToChange);
  {
  HTuple end_val5 = (hv_ValuesToChange.TupleLength())-1;
  HTuple step_val5 = 1;
  for (hv_IndexReset=0; hv_IndexReset.Continue(end_val5, step_val5); hv_IndexReset += step_val5)
  {
    Threshold(ho_Image, &ho_RegionClass, HTuple(hv_ValuesToChange[hv_IndexReset]), 
        HTuple(hv_ValuesToChange[hv_IndexReset]));
    Union2(ho_RegionToChange, ho_RegionClass, &ho_RegionToChange);
  }
  }
  OverpaintRegion(ho_Image, ho_RegionToChange, hv_NewValue, "fill");
  (*ho_ImageOut) = ho_Image;
  return;
}

// Chapter: Deep Learning / Evaluation
// Short Description: Reduce the evaluation result to a single value. 
void reduce_dl_evaluation_result (HTuple hv_EvaluationResult, HTuple hv_EvaluationComparisonKeys, 
    HTuple *hv_Value, HTuple *hv_ValidEvaluationKeys)
{

  // Local iconic variables

  // Local control variables
  HTuple  hv_KeysEvalResult, hv_NumMatches, hv_FirstMaxNumDetections;
  HTuple  hv_KeysFirstMaxNumDetections, hv_DetectionResult;
  HTuple  hv_Index, hv_ClassificationResult, hv_KeysExist;
  HTuple  hv_Indices, hv_Values, hv_K, hv_Key, hv_Tuple;

  //
  //In order to compare a model we need to reduce the evaluation parameter/result
  //to a single float Value which is comparable via >.
  //
  if (0 != (HTuple(int((hv_EvaluationComparisonKeys.TupleLength())>0)).TupleAnd(int((hv_EvaluationResult.TupleLength())>0))))
  {
    //We need to check for a special case: detection results.
    //They have a complex structure.
    GetDictParam(hv_EvaluationResult, "keys", HTuple(), &hv_KeysEvalResult);
    TupleRegexpTest(hv_KeysEvalResult, "max_num_detections_.*", &hv_NumMatches);
    if (0 != (int(hv_NumMatches>0)))
    {
      //We use only the first results of every level.
      GetDictTuple(hv_EvaluationResult, HTuple(hv_KeysEvalResult[0]), &hv_FirstMaxNumDetections);
      GetDictParam(hv_FirstMaxNumDetections, "keys", HTuple(), &hv_KeysFirstMaxNumDetections);
      GetDictTuple(hv_FirstMaxNumDetections, HTuple(hv_KeysFirstMaxNumDetections[0]), 
          &hv_DetectionResult);
      //We use this result here as the top level to retrieve values.
      hv_EvaluationResult = hv_DetectionResult;
    }
    //We need to check for a special case: classification results.
    //They have a complex structure.
    TupleFind(hv_KeysEvalResult.TupleEqualElem("global"), 1, &hv_Index);
    if (0 != (int(hv_Index!=-1)))
    {
      //We use the results for key 'global'.
      GetDictTuple(hv_EvaluationResult, HTuple(hv_KeysEvalResult[hv_Index]), &hv_ClassificationResult);
      hv_EvaluationResult = hv_ClassificationResult;
    }
    //Reduce comparison to keys that exist.
    GetDictParam(hv_EvaluationResult, "key_exists", hv_EvaluationComparisonKeys, 
        &hv_KeysExist);
    //
    TupleFind(hv_KeysExist, 1, &hv_Indices);
    if (0 != (int(hv_Indices==-1)))
    {
      hv_EvaluationComparisonKeys = HTuple();
    }
    else
    {
      hv_EvaluationComparisonKeys = HTuple(hv_EvaluationComparisonKeys[hv_Indices]);
    }
  }
  //
  (*hv_ValidEvaluationKeys) = hv_EvaluationComparisonKeys;
  //
  (*hv_Value) = 0.0;
  hv_Values = HTuple();
  if (0 != (int((hv_EvaluationResult.TupleLength())>0)))
  {
    {
    HTuple end_val41 = (hv_EvaluationComparisonKeys.TupleLength())-1;
    HTuple step_val41 = 1;
    for (hv_K=0; hv_K.Continue(end_val41, step_val41); hv_K += step_val41)
    {
      hv_Key = HTuple(hv_EvaluationComparisonKeys[hv_K]);
      //
      GetDictTuple(hv_EvaluationResult, hv_Key, &hv_Tuple);
      //Use the mean in order to reduce tuples with length > 1.
      hv_Values = hv_Values.TupleConcat(hv_Tuple.TupleMean());
    }
    }
  }
  else
  {
    (*hv_Value) = -1;
  }
  if (0 != (int((hv_Values.TupleLength())>0)))
  {
    (*hv_Value) = hv_Values.TupleMean();
  }
  //
  return;
}

// Chapter: File / Misc
// Short Description: This procedure removes a directory recursively. 
void remove_dir_recursively (HTuple hv_DirName)
{

  // Local control variables
  HTuple  hv_Dirs, hv_I, hv_Files;

  //Recursively delete all subdirectories.
  ListFiles(hv_DirName, "directories", &hv_Dirs);
  {
  HTuple end_val2 = (hv_Dirs.TupleLength())-1;
  HTuple step_val2 = 1;
  for (hv_I=0; hv_I.Continue(end_val2, step_val2); hv_I += step_val2)
  {
    remove_dir_recursively(HTuple(hv_Dirs[hv_I]));
  }
  }
  //Delete all files.
  ListFiles(hv_DirName, "files", &hv_Files);
  {
  HTuple end_val7 = (hv_Files.TupleLength())-1;
  HTuple step_val7 = 1;
  for (hv_I=0; hv_I.Continue(end_val7, step_val7); hv_I += step_val7)
  {
    DeleteFile(HTuple(hv_Files[hv_I]));
  }
  }
  //Remove empty directory.
  RemoveDir(hv_DirName);
  return;
}

// Chapter: Deep Learning / Model
// Short Description: This procedure replaces legacy preprocessing parameters. 
void replace_legacy_preprocessing_parameters (HTuple hv_DLPreprocessParam)
{

  // Local iconic variables

  // Local control variables
  HTuple  hv_Exception, hv_NormalizationTypeExists;
  HTuple  hv_NormalizationType, hv_LegacyNormalizationKeyExists;
  HTuple  hv_ContrastNormalization;

  //
  //This procedure adapts the dictionary DLPreprocessParam
  //if a legacy preprocessing parameter is set.
  //
  //Map legacy value set to new parameter.
  hv_Exception = 0;
  try
  {
    GetDictParam(hv_DLPreprocessParam, "key_exists", "normalization_type", &hv_NormalizationTypeExists);
    //
    if (0 != hv_NormalizationTypeExists)
    {
      GetDictTuple(hv_DLPreprocessParam, "normalization_type", &hv_NormalizationType);
      if (0 != (int(hv_NormalizationType==HTuple("true"))))
      {
        hv_NormalizationType = "first_channel";
      }
      else if (0 != (int(hv_NormalizationType==HTuple("false"))))
      {
        hv_NormalizationType = "none";
      }
      SetDictTuple(hv_DLPreprocessParam, "normalization_type", hv_NormalizationType);
    }
  }
  // catch (Exception) 
  catch (HException &HDevExpDefaultException)
  {
    HDevExpDefaultException.ToHTuple(&hv_Exception);
  }
  //
  //Map legacy parameter to new parameter and corresponding value.
  hv_Exception = 0;
  try
  {
    GetDictParam(hv_DLPreprocessParam, "key_exists", "contrast_normalization", &hv_LegacyNormalizationKeyExists);
    if (0 != hv_LegacyNormalizationKeyExists)
    {
      GetDictTuple(hv_DLPreprocessParam, "contrast_normalization", &hv_ContrastNormalization);
      //Replace 'contrast_normalization' by 'normalization_type'.
      if (0 != (int(hv_ContrastNormalization==HTuple("false"))))
      {
        SetDictTuple(hv_DLPreprocessParam, "normalization_type", "none");
      }
      else if (0 != (int(hv_ContrastNormalization==HTuple("true"))))
      {
        SetDictTuple(hv_DLPreprocessParam, "normalization_type", "first_channel");
      }
      RemoveDictKey(hv_DLPreprocessParam, "contrast_normalization");
    }
  }
  // catch (Exception) 
  catch (HException &HDevExpDefaultException)
  {
    HDevExpDefaultException.ToHTuple(&hv_Exception);
  }
  return;
}

// Chapter: Deep Learning / Model
// Short Description: Restore serialized DL train infos to resume the training. 
void restore_dl_train_info_for_resuming (HTuple hv_StartEpoch, HTuple hv_SerializationData, 
    HTuple hv_TrainParam, HTuple hv_DisplayData, HTuple *hv_EvaluationInfos, HTuple *hv_TrainInfos, 
    HTuple *hv_DisplayEvaluationEpochs, HTuple *hv_DisplayValidationEvaluationValues, 
    HTuple *hv_DisplayTrainEvaluationValues, HTuple *hv_DisplayLossEpochs, HTuple *hv_DisplayLoss, 
    HTuple *hv_DisplayLearningRates, HTuple *hv_TrainResultsRestored, HTuple *hv_StartEpochNumber)
{

  // Local iconic variables

  // Local control variables
  HTuple  hv_TrainResults, hv_SerializationStrategies;
  HTuple  hv_RawData, hv_FoundEpochs, hv_FoundDicts, hv_Index;
  HTuple  hv_Strategy, hv_Type, hv_Basename, hv_Substrings;
  HTuple  hv_BaseFolder, hv_Files, hv_DictFiles, hv_EpochStrings;
  HTuple  hv_EpochStringsNumbers, hv_DictFileNames, hv_IndexDict;
  HTuple  hv_DictFileName, hv_InfoDicts, hv_Epochs, hv_ReadSuccess;
  HTuple  hv_InfoDict, hv_Epoch, hv_Exception, hv_EvaluationComparisonKeys;
  HTuple  hv_DisplayEnabled, hv_IndexEval, hv_EvaluationResult;
  HTuple  hv_Value, hv_IndexTrain, hv_EpochsStatus, hv_MeanLoss;
  HTuple  hv_ModelParams, hv_DisplayLearningRate, hv_LossParam;
  HTuple  hv_CountSamples, hv_LossValues, hv_NumMeanLossSamples;
  HTuple  hv_SamplesPerEpoch, hv_MeanLossCur, hv_NumSamplesInterval;
  HTuple  hv_LossValueIdxsPrev, hv_MeanLossCurInterval, hv_LossValuesCurInterval;
  HTuple  hv_IndexSample, hv_TrainResult, hv_NumEpochs, hv_ValidEvaluationKeys;

  //
  //This procedure initializes training relevant parameters that are stored
  //during training. If StartEpoch is greater than zero or equal to 'resume',
  //the procedure restores this information. This allows to properly resume a
  //training that had been paused or should be continued for another reason.
  //
  //Initialize the variable to collect all training results during training.
  hv_TrainResults = HTuple();
  //
  //Initialize the variable to collect the evaluation information during training.
  (*hv_EvaluationInfos) = HTuple();
  (*hv_TrainResultsRestored) = HTuple();
  //
  //Initialize the variable to collect the train status information during training.
  (*hv_TrainInfos) = HTuple();
  //
  //Initialize visualization parameters.
  (*hv_DisplayLossEpochs) = HTuple();
  (*hv_DisplayLoss) = HTuple();
  (*hv_DisplayEvaluationEpochs) = HTuple();
  (*hv_DisplayValidationEvaluationValues) = HTuple();
  (*hv_DisplayTrainEvaluationValues) = HTuple();
  (*hv_DisplayLearningRates) = HTuple();
  //
  //Initialize the start epoch number.
  (*hv_StartEpochNumber) = 0;
  //
  //Training parameters are initialized for new training,
  //hence return if StartEpoch is zero.
  if (0 != (int(hv_StartEpoch==0.0)))
  {
    return;
  }
  //
  //This procedure reads the latest training and evaluation infos from disk to resume training.
  //
  //Initialize each serialization strategy.
  GetDictTuple(hv_SerializationData, "strategies", &hv_SerializationStrategies);
  GetDictTuple(hv_SerializationData, "raw_data", &hv_RawData);
  //
  //Loop over all serialization strategies. If more than one is available and StartEpoch='resume',
  //choose the most up-to-date training information that can be found.
  hv_FoundEpochs = HTuple();
  hv_FoundDicts = HTuple();
  {
  HTuple end_val43 = (hv_SerializationStrategies.TupleLength())-1;
  HTuple step_val43 = 1;
  for (hv_Index=0; hv_Index.Continue(end_val43, step_val43); hv_Index += step_val43)
  {
    //
    //Get current strategy and data.
    hv_Strategy = HTuple(hv_SerializationStrategies[hv_Index]);
    GetDictTuple(hv_Strategy, "type", &hv_Type);
    GetDictTuple(hv_Strategy, "basename", &hv_Basename);
    //
    if (0 != (HTuple(HTuple(int(hv_Type==HTuple("best"))).TupleOr(int(hv_Type==HTuple("current")))).TupleOr(int(hv_Type==HTuple("epochs")))))
    {
      //
      if (0 != (int(hv_Type==HTuple("epochs"))))
      {
        //Find the last written training information
        TupleRegexpReplace(hv_Basename, (HTuple("\\\\+").Append("replace_all")), 
            "/", &hv_Basename);
        TupleSplit(hv_Basename, "/", &hv_Substrings);
        hv_BaseFolder = ".";
        if (0 != (int((hv_Substrings.TupleLength())>1)))
        {
          hv_BaseFolder = (hv_Substrings.TupleSelectRange(0,(hv_Substrings.TupleLength())-2))+"/";
        }
        ListFiles(hv_BaseFolder, "files", &hv_Files);
        TupleRegexpSelect(hv_Files, "[0-9]\\.[0-9]*_info\\.hdict", &hv_DictFiles);
        TupleRegexpMatch(hv_DictFiles, "[0-9]\\.[0-9]*", &hv_EpochStrings);
        hv_EpochStringsNumbers = hv_EpochStrings.TupleNumber();
        TupleGenConst(hv_EpochStrings.TupleLength(), "", &hv_DictFileNames);
        {
        HTuple end_val65 = (hv_EpochStrings.TupleLength())-1;
        HTuple step_val65 = 1;
        for (hv_IndexDict=0; hv_IndexDict.Continue(end_val65, step_val65); hv_IndexDict += step_val65)
        {
          TupleRegexpSelect(hv_DictFiles, (HTuple(hv_EpochStringsNumbers[hv_IndexDict]).TupleString(".2f"))+"_info.hdict", 
              &hv_DictFileName);
          if (0 != (int((hv_DictFileName.TupleLength())!=0)))
          {
            hv_DictFileNames[hv_IndexDict] = hv_DictFileName;
          }
        }
        }
      }
      else
      {
        hv_DictFileNames = hv_Basename+"_info.hdict";
      }
      //
      //Try to read in the training information dicts.
      TupleGenConst(hv_DictFileNames.TupleLength(), -1, &hv_InfoDicts);
      TupleGenConst(hv_DictFileNames.TupleLength(), -1, &hv_Epochs);
      hv_ReadSuccess = 0;
      {
      HTuple end_val79 = (hv_DictFileNames.TupleLength())-1;
      HTuple step_val79 = 1;
      for (hv_IndexDict=0; hv_IndexDict.Continue(end_val79, step_val79); hv_IndexDict += step_val79)
      {
        try
        {
          ReadDict(HTuple(hv_DictFileNames[hv_IndexDict]), HTuple(), HTuple(), &hv_InfoDict);
          GetDictTuple(hv_InfoDict, "epoch", &hv_Epoch);
          hv_InfoDicts[hv_IndexDict] = hv_InfoDict;
          hv_Epochs[hv_IndexDict] = hv_Epoch;
          hv_ReadSuccess = 1;
        }
        // catch (Exception) 
        catch (HException &HDevExpDefaultException)
        {
          HDevExpDefaultException.ToHTuple(&hv_Exception);
        }
      }
      }
      if (0 != (hv_ReadSuccess.TupleNot()))
      {
        //Not even a single file has been found.
        continue;
      }
      //
      hv_FoundEpochs = hv_FoundEpochs.TupleConcat(hv_Epochs);
      hv_FoundDicts = hv_FoundDicts.TupleConcat(hv_InfoDicts);
      //
    }
    else if (0 != (int(hv_Type==HTuple("final"))))
    {
      //Nothing to restore.
      continue;
    }
    else
    {
      throw HException(("Unknown serialization strategy type: '"+hv_Type)+"'");
    }
  }
  }
  //
  //Check if training can or needs to be resumed.
  if (0 != (int(hv_StartEpoch==HTuple("resume"))))
  {
    //Resume at highest epoch available.
    hv_Epoch = hv_FoundEpochs.TupleMax();
    if (0 != (int(hv_Epoch<0.0)))
    {
      throw HException("No training information found. Training cannot be resumed.");
    }
    hv_Index = (hv_FoundEpochs.TupleEqualElem(hv_Epoch)).TupleFindFirst(1);
    hv_InfoDict = HTuple(hv_FoundDicts[hv_Index]);
    hv_StartEpoch = hv_Epoch;
  }
  else
  {
    //Check if requested StartEpoch was found.
    hv_Index = (hv_FoundEpochs.TupleEqualElem(hv_StartEpoch)).TupleFindFirst(1);
    if (0 != (HTuple(int(hv_Index!=HTuple())).TupleAnd(int(hv_Index!=-1))))
    {
      hv_InfoDict = HTuple(hv_FoundDicts[hv_Index]);
    }
    else
    {
      //Try to find the rounded value as it is serialized.
      hv_Index = ((hv_FoundEpochs.TupleString(".2f")).TupleEqualElem(hv_StartEpoch.TupleString(".2f"))).TupleFindFirst(1);
      if (0 != (HTuple(int(hv_Index!=HTuple())).TupleAnd(int(hv_Index!=-1))))
      {
        hv_InfoDict = HTuple(hv_FoundDicts[hv_Index]);
      }
      else
      {
        throw HException("No training information matches requested StartEpoch "+hv_StartEpoch);
      }
    }
  }
  //
  //Get evaluation and training infos.
  GetDictTuple(hv_InfoDict, "evaluation_infos", &(*hv_EvaluationInfos));
  GetDictTuple(hv_InfoDict, "train_infos", &(*hv_TrainInfos));
  GetDictTuple(hv_TrainParam, "evaluation_comparison_keys", &hv_EvaluationComparisonKeys);
  //
  //Restore history of display values.
  GetDictTuple(hv_DisplayData, "enabled", &hv_DisplayEnabled);
  if (0 != hv_DisplayEnabled)
  {
    {
    HTuple end_val139 = ((*hv_EvaluationInfos).TupleLength())-1;
    HTuple step_val139 = 1;
    for (hv_IndexEval=0; hv_IndexEval.Continue(end_val139, step_val139); hv_IndexEval += step_val139)
    {
      GetDictTuple(HTuple((*hv_EvaluationInfos)[hv_IndexEval]), "result", &hv_EvaluationResult);
      reduce_dl_evaluation_result(hv_EvaluationResult, hv_EvaluationComparisonKeys, 
          &hv_Value, &hv_ValidEvaluationKeys);
      (*hv_DisplayValidationEvaluationValues) = (*hv_DisplayValidationEvaluationValues).TupleConcat(hv_Value);
      hv_Value = -1;
      try
      {
        GetDictTuple(HTuple((*hv_EvaluationInfos)[hv_IndexEval]), "train_result", 
            &hv_EvaluationResult);
        reduce_dl_evaluation_result(hv_EvaluationResult, hv_EvaluationComparisonKeys, 
            &hv_Value, &hv_ValidEvaluationKeys);
      }
      // catch (Exception) 
      catch (HException &HDevExpDefaultException)
      {
        HDevExpDefaultException.ToHTuple(&hv_Exception);
      }
      (*hv_DisplayTrainEvaluationValues) = (*hv_DisplayTrainEvaluationValues).TupleConcat(hv_Value);
      GetDictTuple(HTuple((*hv_EvaluationInfos)[hv_IndexEval]), "epoch", &hv_Epoch);
      (*hv_DisplayEvaluationEpochs) = (*hv_DisplayEvaluationEpochs).TupleConcat(hv_Epoch);
    }
    }
    {
    HTuple end_val153 = ((*hv_TrainInfos).TupleLength())-1;
    HTuple step_val153 = 1;
    for (hv_IndexTrain=0; hv_IndexTrain.Continue(end_val153, step_val153); hv_IndexTrain += step_val153)
    {
      GetDictTuple(HTuple((*hv_TrainInfos)[hv_IndexTrain]), "epoch", &hv_EpochsStatus);
      (*hv_DisplayLossEpochs) = (*hv_DisplayLossEpochs).TupleConcat(hv_EpochsStatus);
      GetDictTuple(HTuple((*hv_TrainInfos)[hv_IndexTrain]), "mean_loss", &hv_MeanLoss);
      (*hv_DisplayLoss) = (*hv_DisplayLoss).TupleConcat(hv_MeanLoss);
      GetDictTuple(HTuple((*hv_TrainInfos)[hv_IndexTrain]), "model_params", &hv_ModelParams);
      GetDictTuple(hv_ModelParams, "learning_rate", &hv_DisplayLearningRate);
      (*hv_DisplayLearningRates) = (*hv_DisplayLearningRates).TupleConcat(hv_DisplayLearningRate);
    }
    }
  }
  //
  //Restore dictionaries that contain the approximate loss-values for each iteration.
  //We cannot reconstruct the exact loss values, therefore, we use the serialized mean values.
  hv_LossParam = "total_loss";
  hv_CountSamples = 0;
  hv_LossValues = HTuple();
  GetDictTuple(HTuple((*hv_TrainInfos)[0]), "epoch", &hv_Epoch);
  GetDictTuple(HTuple((*hv_TrainInfos)[0]), "mean_loss_samples", &hv_NumMeanLossSamples);
  hv_SamplesPerEpoch = (hv_NumMeanLossSamples.TupleReal())/hv_Epoch;
  //
  {
  HTuple end_val173 = ((*hv_TrainInfos).TupleLength())-1;
  HTuple step_val173 = 1;
  for (hv_IndexTrain=0; hv_IndexTrain.Continue(end_val173, step_val173); hv_IndexTrain += step_val173)
  {
    GetDictTuple(HTuple((*hv_TrainInfos)[hv_IndexTrain]), "mean_loss", &hv_MeanLossCur);
    GetDictTuple(HTuple((*hv_TrainInfos)[hv_IndexTrain]), "mean_loss_samples", &hv_NumMeanLossSamples);
    //The iterations within one interval are not fixed.
    //Calculate the current iteration and the number of iterations within the interval based on the epoch.
    GetDictTuple(HTuple((*hv_TrainInfos)[hv_IndexTrain]), "epoch", &hv_Epoch);
    hv_NumSamplesInterval = ((hv_Epoch*hv_SamplesPerEpoch).TupleRound())-hv_CountSamples;
    //For multiple resuming it can happen that more than one train-info for the same time-point exists.
    if (0 != (int(hv_NumSamplesInterval==0)))
    {
      continue;
    }
    //Calculate the mean loss within the interval between the previous and the current serialization time-point.
    hv_LossValueIdxsPrev = HTuple::TupleGenSequence((hv_LossValues.TupleLength())-(hv_NumMeanLossSamples-hv_NumSamplesInterval),(hv_LossValues.TupleLength())-1,1).TupleInt();
    if (0 != (hv_LossValueIdxsPrev.TupleLength()))
    {
      //The total mean loss (MeanLossCur) consists of the mean loss within this interval (MeanLossCurInterval) and
      //the fraction of previous samples.
      hv_MeanLossCurInterval = ((hv_MeanLossCur*hv_NumMeanLossSamples)-(HTuple(hv_LossValues[hv_LossValueIdxsPrev]).TupleSum()))/hv_NumSamplesInterval;
    }
    else
    {
      //In this case the total mean loss is just the loss of this interval.
      hv_MeanLossCurInterval = hv_MeanLossCur;
    }
    //
    hv_LossValuesCurInterval = HTuple(hv_NumSamplesInterval,hv_MeanLossCurInterval);
    hv_LossValues = hv_LossValues.TupleConcat(hv_LossValuesCurInterval);
    //
    //Pack the loss values into dictionaries.
    (*hv_TrainResultsRestored) = (*hv_TrainResultsRestored).TupleConcat(HTuple(hv_NumSamplesInterval,-1));
    {
    HTuple end_val200 = hv_NumSamplesInterval-1;
    HTuple step_val200 = 1;
    for (hv_IndexSample=0; hv_IndexSample.Continue(end_val200, step_val200); hv_IndexSample += step_val200)
    {
      CreateDict(&hv_TrainResult);
      SetDictTuple(hv_TrainResult, hv_LossParam, HTuple(hv_LossValuesCurInterval[hv_IndexSample]));
      (*hv_TrainResultsRestored)[hv_CountSamples] = hv_TrainResult;
      hv_CountSamples += 1;
    }
    }
  }
  }
  //
  //Plot the current training status.
  if (0 != hv_DisplayEnabled)
  {
    GetDictTuple(hv_TrainParam, "num_epochs", &hv_NumEpochs);
    SetDictTuple(HTuple((*hv_TrainInfos)[((*hv_TrainInfos).TupleLength())-1]), "num_epochs", 
        hv_NumEpochs);
    dev_display_update_train_dl_model(hv_TrainParam, hv_DisplayData, HTuple((*hv_TrainInfos)[((*hv_TrainInfos).TupleLength())-1]), 
        (*hv_DisplayLossEpochs), (*hv_DisplayLoss), (*hv_DisplayLearningRates), (*hv_DisplayEvaluationEpochs), 
        (*hv_DisplayValidationEvaluationValues), (*hv_DisplayTrainEvaluationValues));
  }
  //
  //Return StartEpoch as number.
  (*hv_StartEpochNumber) = hv_StartEpoch;
  //
  return;
}

// Chapter: Filters / Arithmetic
// Short Description: Scale the gray values of an image from the interval [Min,Max] to [0,255] 
void scale_image_range (HObject ho_Image, HObject *ho_ImageScaled, HTuple hv_Min, 
    HTuple hv_Max)
{

  // Local iconic variables
  HObject  ho_ImageSelected, ho_SelectedChannel;
  HObject  ho_LowerRegion, ho_UpperRegion, ho_ImageSelectedScaled;

  // Local control variables
  HTuple  hv_LowerLimit, hv_UpperLimit, hv_Mult;
  HTuple  hv_Add, hv_NumImages, hv_ImageIndex, hv_Channels;
  HTuple  hv_ChannelIndex, hv_MinGray, hv_MaxGray, hv_Range;

  //Convenience procedure to scale the gray values of the
  //input image Image from the interval [Min,Max]
  //to the interval [0,255] (default).
  //Gray values < 0 or > 255 (after scaling) are clipped.
  //
  //If the image shall be scaled to an interval different from [0,255],
  //this can be achieved by passing tuples with 2 values [From, To]
  //as Min and Max.
  //Example:
  //scale_image_range(Image:ImageScaled:[100,50],[200,250])
  //maps the gray values of Image from the interval [100,200] to [50,250].
  //All other gray values will be clipped.
  //
  //input parameters:
  //Image: the input image
  //Min: the minimum gray value which will be mapped to 0
  //     If a tuple with two values is given, the first value will
  //     be mapped to the second value.
  //Max: The maximum gray value which will be mapped to 255
  //     If a tuple with two values is given, the first value will
  //     be mapped to the second value.
  //
  //Output parameter:
  //ImageScale: the resulting scaled image.
  //
  if (0 != (int((hv_Min.TupleLength())==2)))
  {
    hv_LowerLimit = ((const HTuple&)hv_Min)[1];
    hv_Min = ((const HTuple&)hv_Min)[0];
  }
  else
  {
    hv_LowerLimit = 0.0;
  }
  if (0 != (int((hv_Max.TupleLength())==2)))
  {
    hv_UpperLimit = ((const HTuple&)hv_Max)[1];
    hv_Max = ((const HTuple&)hv_Max)[0];
  }
  else
  {
    hv_UpperLimit = 255.0;
  }
  //
  //Calculate scaling parameters.
  //Only scale if the scaling range is not zero.
  if (0 != (HTuple(int(((hv_Max-hv_Min).TupleAbs())<1.0E-6)).TupleNot()))
  {
    hv_Mult = ((hv_UpperLimit-hv_LowerLimit).TupleReal())/(hv_Max-hv_Min);
    hv_Add = ((-hv_Mult)*hv_Min)+hv_LowerLimit;
    //Scale image.
    ScaleImage(ho_Image, &ho_Image, hv_Mult, hv_Add);
  }
  //
  //Clip gray values if necessary.
  //This must be done for each image and channel separately.
  GenEmptyObj(&(*ho_ImageScaled));
  CountObj(ho_Image, &hv_NumImages);
  {
  HTuple end_val51 = hv_NumImages;
  HTuple step_val51 = 1;
  for (hv_ImageIndex=1; hv_ImageIndex.Continue(end_val51, step_val51); hv_ImageIndex += step_val51)
  {
    SelectObj(ho_Image, &ho_ImageSelected, hv_ImageIndex);
    CountChannels(ho_ImageSelected, &hv_Channels);
    {
    HTuple end_val54 = hv_Channels;
    HTuple step_val54 = 1;
    for (hv_ChannelIndex=1; hv_ChannelIndex.Continue(end_val54, step_val54); hv_ChannelIndex += step_val54)
    {
      AccessChannel(ho_ImageSelected, &ho_SelectedChannel, hv_ChannelIndex);
      MinMaxGray(ho_SelectedChannel, ho_SelectedChannel, 0, &hv_MinGray, &hv_MaxGray, 
          &hv_Range);
      Threshold(ho_SelectedChannel, &ho_LowerRegion, (hv_MinGray.TupleConcat(hv_LowerLimit)).TupleMin(), 
          hv_LowerLimit);
      Threshold(ho_SelectedChannel, &ho_UpperRegion, hv_UpperLimit, (hv_UpperLimit.TupleConcat(hv_MaxGray)).TupleMax());
      PaintRegion(ho_LowerRegion, ho_SelectedChannel, &ho_SelectedChannel, hv_LowerLimit, 
          "fill");
      PaintRegion(ho_UpperRegion, ho_SelectedChannel, &ho_SelectedChannel, hv_UpperLimit, 
          "fill");
      if (0 != (int(hv_ChannelIndex==1)))
      {
        CopyObj(ho_SelectedChannel, &ho_ImageSelectedScaled, 1, 1);
      }
      else
      {
        AppendChannel(ho_ImageSelectedScaled, ho_SelectedChannel, &ho_ImageSelectedScaled
            );
      }
    }
    }
    ConcatObj((*ho_ImageScaled), ho_ImageSelectedScaled, &(*ho_ImageScaled));
  }
  }
  return;
}

// Chapter: Deep Learning / Model
// Short Description: Serialize a DLModelHandle with current meta information. 
void serialize_train_dl_model_intermediate (HTuple hv_DLModelHandle, HTuple hv_Epoch, 
    HTuple hv_Strategy, HTuple hv_TrainInfos, HTuple hv_EvaluationInfos, HTuple *hv_FilenameModel, 
    HTuple *hv_FilenameMetaData)
{

  // Local iconic variables

  // Local control variables
  HTuple  hv_Type, hv_Basename, hv_Exception, hv_Epochs;
  HTuple  hv_Index, hv_MetaData;

  //
  //Serialize the model DLModelHandle with current meta information.
  //
  //We need the type of strategy used.
  GetDictTuple(hv_Strategy, "type", &hv_Type);

  //Get basename/default.
  try
  {
    GetDictTuple(hv_Strategy, "basename", &hv_Basename);
  }
  // catch (Exception) 
  catch (HException &HDevExpDefaultException)
  {
    HDevExpDefaultException.ToHTuple(&hv_Exception);
    if (0 != (int(hv_Type==HTuple("epochs"))))
    {
      hv_Basename = "model_at_epoch";
    }
    else
    {
      hv_Basename = hv_Type;
    }
  }
  //
  //If we serialize epochs and only one basename is given,
  //we need to add the current epoch to it.
  //If a basename has been specified for each epoch,
  //appending the current epoch is not necessary.
  if (0 != (int(hv_Type==HTuple("epochs"))))
  {
    GetDictTuple(hv_Strategy, "epochs", &hv_Epochs);
    if (0 != (int((hv_Basename.TupleLength())==(hv_Epochs.TupleLength()))))
    {
      TupleFindLast(hv_Epoch.TupleLessElem(hv_Epochs), 0, &hv_Index);
      hv_Basename = HTuple(hv_Basename[hv_Index]);
    }
    else
    {
      hv_Basename = (hv_Basename+"_")+(hv_Epoch.TupleString(".2f"));
    }
  }
  //
  //Filenames.
  (*hv_FilenameModel) = hv_Basename+".hdl";
  (*hv_FilenameMetaData) = hv_Basename+"_info.hdict";
  //
  //Metadata.
  CreateDict(&hv_MetaData);
  SetDictTuple(hv_MetaData, "train_infos", hv_TrainInfos);
  SetDictTuple(hv_MetaData, "evaluation_infos", hv_EvaluationInfos);
  SetDictTuple(hv_MetaData, "epoch", hv_Epoch);
  //
  //Write files.
  WriteDlModel(hv_DLModelHandle, (*hv_FilenameModel));
  WriteDict(hv_MetaData, (*hv_FilenameMetaData), HTuple(), HTuple());
  return;
}

// Chapter: Graphics / Text
// Short Description: Set font independent of OS 
void set_display_font (HTuple hv_WindowHandle, HTuple hv_Size, HTuple hv_Font, HTuple hv_Bold, 
    HTuple hv_Slant)
{

  // Local iconic variables

  // Local control variables
  HTuple  hv_OS, hv_Fonts, hv_Style, hv_Exception;
  HTuple  hv_AvailableFonts, hv_Fdx, hv_Indices;

  //This procedure sets the text font of the current window with
  //the specified attributes.
  //
  //Input parameters:
  //WindowHandle: The graphics window for which the font will be set
  //Size: The font size. If Size=-1, the default of 16 is used.
  //Bold: If set to 'true', a bold font is used
  //Slant: If set to 'true', a slanted font is used
  //
  GetSystem("operating_system", &hv_OS);
  if (0 != (HTuple(int(hv_Size==HTuple())).TupleOr(int(hv_Size==-1))))
  {
    hv_Size = 16;
  }
  if (0 != (int((hv_OS.TupleSubstr(0,2))==HTuple("Win"))))
  {
    //Restore previous behaviour
    hv_Size = (1.13677*hv_Size).TupleInt();
  }
  else
  {
    hv_Size = hv_Size.TupleInt();
  }
  if (0 != (int(hv_Font==HTuple("Courier"))))
  {
    hv_Fonts.Clear();
    hv_Fonts[0] = "Courier";
    hv_Fonts[1] = "Courier 10 Pitch";
    hv_Fonts[2] = "Courier New";
    hv_Fonts[3] = "CourierNew";
    hv_Fonts[4] = "Liberation Mono";
  }
  else if (0 != (int(hv_Font==HTuple("mono"))))
  {
    hv_Fonts.Clear();
    hv_Fonts[0] = "Consolas";
    hv_Fonts[1] = "Menlo";
    hv_Fonts[2] = "Courier";
    hv_Fonts[3] = "Courier 10 Pitch";
    hv_Fonts[4] = "FreeMono";
    hv_Fonts[5] = "Liberation Mono";
  }
  else if (0 != (int(hv_Font==HTuple("sans"))))
  {
    hv_Fonts.Clear();
    hv_Fonts[0] = "Luxi Sans";
    hv_Fonts[1] = "DejaVu Sans";
    hv_Fonts[2] = "FreeSans";
    hv_Fonts[3] = "Arial";
    hv_Fonts[4] = "Liberation Sans";
  }
  else if (0 != (int(hv_Font==HTuple("serif"))))
  {
    hv_Fonts.Clear();
    hv_Fonts[0] = "Times New Roman";
    hv_Fonts[1] = "Luxi Serif";
    hv_Fonts[2] = "DejaVu Serif";
    hv_Fonts[3] = "FreeSerif";
    hv_Fonts[4] = "Utopia";
    hv_Fonts[5] = "Liberation Serif";
  }
  else
  {
    hv_Fonts = hv_Font;
  }
  hv_Style = "";
  if (0 != (int(hv_Bold==HTuple("true"))))
  {
    hv_Style += HTuple("Bold");
  }
  else if (0 != (int(hv_Bold!=HTuple("false"))))
  {
    hv_Exception = "Wrong value of control parameter Bold";
    throw HException(hv_Exception);
  }
  if (0 != (int(hv_Slant==HTuple("true"))))
  {
    hv_Style += HTuple("Italic");
  }
  else if (0 != (int(hv_Slant!=HTuple("false"))))
  {
    hv_Exception = "Wrong value of control parameter Slant";
    throw HException(hv_Exception);
  }
  if (0 != (int(hv_Style==HTuple(""))))
  {
    hv_Style = "Normal";
  }
  QueryFont(hv_WindowHandle, &hv_AvailableFonts);
  hv_Font = "";
  {
  HTuple end_val48 = (hv_Fonts.TupleLength())-1;
  HTuple step_val48 = 1;
  for (hv_Fdx=0; hv_Fdx.Continue(end_val48, step_val48); hv_Fdx += step_val48)
  {
    hv_Indices = hv_AvailableFonts.TupleFind(HTuple(hv_Fonts[hv_Fdx]));
    if (0 != (int((hv_Indices.TupleLength())>0)))
    {
      if (0 != (int(HTuple(hv_Indices[0])>=0)))
      {
        hv_Font = HTuple(hv_Fonts[hv_Fdx]);
        break;
      }
    }
  }
  }
  if (0 != (int(hv_Font==HTuple(""))))
  {
    throw HException("Wrong value of control parameter Font");
  }
  hv_Font = (((hv_Font+"-")+hv_Style)+"-")+hv_Size;
  SetFont(hv_WindowHandle, hv_Font);
  return;
}

// Chapter: Deep Learning / Model
// Short Description: Split the samples into training, validation, and test subsets. 
void split_dl_dataset (HTuple hv_DLDataset, HTuple hv_TrainingPercent, HTuple hv_ValidationPercent, 
    HTuple hv_GenParam)
{

  // Local iconic variables
  HObject  ho_SegmImage;

  // Local control variables
  HTuple  hv_OverwriteSplit, hv_ModelType, hv_SplitNames;
  HTuple  hv_GenParamName, hv_GenParamIndex, hv_Type, hv_DLSamples;
  HTuple  hv_DLSample, hv_AnomalyDetectionLabelExists, hv_BBoxLabelIdExists;
  HTuple  hv_ImageLabelIdExists, hv_SegmFileExists, hv_ClassIDs;
  HTuple  hv_ClassNames, hv_ClassIDToClassIndex, hv_TrainingRatio;
  HTuple  hv_ValidationRatio, hv_SplitRatios, hv_SplitRatiosInvSortIndices;
  HTuple  hv_NotYetSplit, hv_IndexSample, hv_SplitExists;
  HTuple  hv_ImageIndicesAllClass, hv_NumImagesPerClass, hv_ImageIndices;
  HTuple  hv_AnomalyLabel, hv_Labels, hv_ImageLabelID, hv_BboxLabels;
  HTuple  hv_SegmDir, hv_SegmFileName, hv_AbsoluteHisto, hv_LabelIndices;
  HTuple  hv_Index, hv_ClassIndex, hv_ImageIndicesPerClass;
  HTuple  hv_SplitImageIndices, hv_AssignedImageIndices, hv_ClassSortIndices;
  HTuple  hv_ImageIndicesClass, hv_ImageIndicesClassToBeAssigned;
  HTuple  hv_SplitIndex, hv_NumToBeAssignedToThisSplit, hv_AssignedImageIndicesToThisSplit;
  HTuple  hv_NumAlreadyAssignedToThisSplit, hv_NumStillToBeAssigned;
  HTuple  hv_ImageIndex, hv_Rand, hv_RatioIndex, hv_CurrentSplitIndex;
  HTuple  hv_CurrentSplitRatio, hv_ImageIndicesWithoutLabel;
  HTuple  hv_NumImageIndicesWithoutLabel, hv_NumToBeAssigned;
  HTuple  hv_MaxRatioIndex, hv_SplitNameIndex, hv_SplitName;
  HTuple  hv_SplitIndices, hv_SampleSplitIndex, hv_ErrorDict;

  //
  //This procedure divides the samples in DLDataset
  //into three disjoint subsets: train, validation, and test.
  //The number of samples in each subset is defined
  //by the given percentages TrainingPercent and ValidationPercent.
  //As a result, every sample has a new key named 'split'
  //with an associated value 'train', 'validation', or 'test'.
  //Thereby the classes of every image are taken
  //into consideration, in order to avoid accidental predominance
  //of certain classes in one of the subsets. In the case of a
  //dataset used for anomaly detection, all images of type 'nok'
  //are sorted into the 'test' split.
  //
  //Check input data.
  if (0 != (int(hv_TrainingPercent<0)))
  {
    throw HException("TrainingPercent must not be smaller than zero.");
  }
  if (0 != (int(hv_ValidationPercent<0)))
  {
    throw HException("ValidationPercent must not be smaller than zero.");
  }
  if (0 != (int((hv_TrainingPercent+hv_ValidationPercent)>100)))
  {
    throw HException("The sum of TrainingPercent and ValidationPercent must not be greater than 100.");
  }
  //
  //** Set the default values ***
  //
  //Overwrite an existing split?
  hv_OverwriteSplit = 0;
  //Initialize model_type of the DLDataset.
  hv_ModelType = "";
  //Names for split subsets.
  hv_SplitNames.Clear();
  hv_SplitNames[0] = "train";
  hv_SplitNames[1] = "validation";
  hv_SplitNames[2] = "test";
  //
  //Get input for generic parameters.
  if (0 != (int(hv_GenParam!=HTuple())))
  {
    GetDictParam(hv_GenParam, "keys", HTuple(), &hv_GenParamName);
    {
    HTuple end_val36 = (hv_GenParamName.TupleLength())-1;
    HTuple step_val36 = 1;
    for (hv_GenParamIndex=0; hv_GenParamIndex.Continue(end_val36, step_val36); hv_GenParamIndex += step_val36)
    {
      if (0 != (int(HTuple(hv_GenParamName[hv_GenParamIndex])==HTuple("overwrite_split"))))
      {
        GetDictTuple(hv_GenParam, "overwrite_split", &hv_OverwriteSplit);
        hv_Type = hv_OverwriteSplit.TupleType();
        if (0 != (HTuple(int(hv_Type==4)).TupleAnd(int(hv_OverwriteSplit==HTuple("true")))))
        {
          hv_OverwriteSplit = 1;
        }
        if (0 != (HTuple(int(hv_Type==4)).TupleAnd(int(hv_OverwriteSplit==HTuple("false")))))
        {
          hv_OverwriteSplit = 0;
        }
      }
      else if (0 != (int(HTuple(hv_GenParamName[hv_GenParamIndex])==HTuple("model_type"))))
      {
        GetDictTuple(hv_GenParam, "model_type", &hv_ModelType);
      }
      else
      {
        throw HException("Unknown GenParam entry: "+HTuple(hv_GenParamName[hv_GenParamIndex]));
      }
    }
    }
  }
  //
  //Read in the samples.
  GetDictTuple(hv_DLDataset, "samples", &hv_DLSamples);
  //
  //Try to guess the ModelType if not set by GenParam.
  if (0 != (int(hv_ModelType==HTuple(""))))
  {
    hv_DLSample = ((const HTuple&)hv_DLSamples)[0];
    //Check for relevant keys.
    GetDictParam(hv_DLSample, "key_exists", "anomaly_label", &hv_AnomalyDetectionLabelExists);
    GetDictParam(hv_DLSample, "key_exists", "bbox_label_id", &hv_BBoxLabelIdExists);
    GetDictParam(hv_DLSample, "key_exists", "image_label_id", &hv_ImageLabelIdExists);
    GetDictParam(hv_DLSample, "key_exists", "segmentation_file_name", &hv_SegmFileExists);
    //
    if (0 != hv_AnomalyDetectionLabelExists)
    {
      hv_ModelType = "anomaly_detection";
    }
    else if (0 != hv_ImageLabelIdExists)
    {
      hv_ModelType = "classification";
    }
    else if (0 != hv_BBoxLabelIdExists)
    {
      hv_ModelType = "detection";
    }
    else if (0 != hv_SegmFileExists)
    {
      hv_ModelType = "segmentation";
    }
    else
    {
      throw HException("Parameter 'model_type' cannot be determined.");
    }
  }
  //
  //Get data from DLDataset.
  if (0 != (int(hv_ModelType!=HTuple("anomaly_detection"))))
  {
    GetDictTuple(hv_DLDataset, "class_ids", &hv_ClassIDs);
    GetDictTuple(hv_DLDataset, "class_names", &hv_ClassNames);
  }
  else
  {
    hv_ClassIDs.Clear();
    hv_ClassIDs[0] = 0;
    hv_ClassIDs[1] = 1;
    hv_ClassNames.Clear();
    hv_ClassNames[0] = "ok";
    hv_ClassNames[1] = "nok";
  }
  hv_ClassIDToClassIndex = HTuple((hv_ClassIDs.TupleMax())+1,-1);
  hv_ClassIDToClassIndex[hv_ClassIDs] = HTuple::TupleGenSequence(0,(hv_ClassIDs.TupleLength())-1,1);
  //
  //Calculate ratios of training and validation datasets.
  hv_TrainingRatio = hv_TrainingPercent*0.01;
  hv_ValidationRatio = hv_ValidationPercent*0.01;
  hv_SplitRatios.Clear();
  hv_SplitRatios.Append(hv_TrainingRatio);
  hv_SplitRatios.Append(hv_ValidationRatio);
  hv_SplitRatios.Append((1.0-hv_TrainingRatio)-hv_ValidationRatio);
  hv_SplitRatiosInvSortIndices = HTuple(hv_SplitRatios.TupleSortIndex()).TupleInverse();
  //
  //Test whether the dataset is already split.
  hv_NotYetSplit = 1;
  {
  HTuple end_val98 = (hv_DLSamples.TupleLength())-1;
  HTuple step_val98 = 1;
  for (hv_IndexSample=0; hv_IndexSample.Continue(end_val98, step_val98); hv_IndexSample += step_val98)
  {
    GetDictParam(HTuple(hv_DLSamples[hv_IndexSample]), "key_exists", "split", &hv_SplitExists);
    if (0 != hv_SplitExists)
    {
      hv_NotYetSplit = 0;
      break;
    }
  }
  }
  //
  //Split the dataset if no split is present
  //or split should be overwritten.
  if (0 != (hv_NotYetSplit.TupleOr(hv_OverwriteSplit)))
  {
    //
    //Initialize a tuple to collect the indices
    //of images that contain a class.
    hv_ImageIndicesAllClass = HTuple((hv_ClassNames.TupleLength())*(hv_DLSamples.TupleLength()),0);
    //
    //Get labels of every sample image
    //and count how many images per class there are.
    //
    hv_NumImagesPerClass = HTuple(hv_ClassIDs.TupleLength(),0);
    hv_ImageIndices = HTuple::TupleGenSequence(0,(hv_DLSamples.TupleLength())-1,1);
    {
    HTuple end_val119 = (hv_DLSamples.TupleLength())-1;
    HTuple step_val119 = 1;
    for (hv_IndexSample=0; hv_IndexSample.Continue(end_val119, step_val119); hv_IndexSample += step_val119)
    {
      hv_DLSample = HTuple(hv_DLSamples[hv_IndexSample]);
      if (0 != (int(hv_ModelType==HTuple("anomaly_detection"))))
      {
        //Get labels - anomaly detection.
        GetDictTuple(hv_DLSample, "anomaly_label", &hv_AnomalyLabel);
        hv_Labels = int(hv_AnomalyLabel!=HTuple("ok"));
      }
      else if (0 != (int(hv_ModelType==HTuple("classification"))))
      {
        //Get labels - classification.
        GetDictTuple(hv_DLSample, "image_label_id", &hv_ImageLabelID);
        hv_Labels = hv_ImageLabelID;
      }
      else if (0 != (int(hv_ModelType==HTuple("detection"))))
      {
        //Get labels - object detection.
        GetDictTuple(hv_DLSample, "bbox_label_id", &hv_BboxLabels);
        hv_Labels = (hv_BboxLabels.TupleSort()).TupleUniq();
      }
      else if (0 != (int(hv_ModelType==HTuple("segmentation"))))
      {
        //Get labels - semantic segmentation.
        GetDictTuple(hv_DLDataset, "segmentation_dir", &hv_SegmDir);
        GetDictTuple(hv_DLSample, "segmentation_file_name", &hv_SegmFileName);
        ReadImage(&ho_SegmImage, (hv_SegmDir+"/")+hv_SegmFileName);
        //
        GrayHistoAbs(ho_SegmImage, ho_SegmImage, 1, &hv_AbsoluteHisto);
        hv_Labels = (hv_AbsoluteHisto.TupleGreaterElem(0)).TupleFind(1);
      }
      //
      //Add up images per class.
      hv_LabelIndices = HTuple(hv_ClassIDToClassIndex[hv_Labels]);
      hv_NumImagesPerClass[hv_LabelIndices] = HTuple(hv_NumImagesPerClass[hv_LabelIndices])+1;
      {
      HTuple end_val146 = (hv_Labels.TupleLength())-1;
      HTuple step_val146 = 1;
      for (hv_Index=0; hv_Index.Continue(end_val146, step_val146); hv_Index += step_val146)
      {
        //Add image index to ImageIndicesAllClass.
        hv_ClassIndex = HTuple(hv_LabelIndices[hv_Index]);
        hv_ImageIndicesAllClass[((hv_ClassIndex*(hv_DLSamples.TupleLength()))+HTuple(hv_NumImagesPerClass[hv_ClassIndex]))-1] = hv_IndexSample;
      }
      }
    }
    }
    //
    //Write the image indices per class to a dictionary.
    CreateDict(&hv_ImageIndicesPerClass);
    {
    HTuple end_val155 = (hv_ClassNames.TupleLength())-1;
    HTuple step_val155 = 1;
    for (hv_ClassIndex=0; hv_ClassIndex.Continue(end_val155, step_val155); hv_ClassIndex += step_val155)
    {
      SetDictTuple(hv_ImageIndicesPerClass, HTuple(hv_ClassNames[hv_ClassIndex]), 
          hv_ImageIndicesAllClass.TupleSelectRange(hv_ClassIndex*(hv_DLSamples.TupleLength()),((hv_ClassIndex*(hv_DLSamples.TupleLength()))+HTuple(hv_NumImagesPerClass[hv_ClassIndex]))-1));
    }
    }
    //
    //** Start splitting. ***
    //
    //Create a dictionary where the image indices
    //for the three subsets are saved.
    CreateDict(&hv_SplitImageIndices);
    {
    HTuple end_val164 = (hv_SplitNames.TupleLength())-1;
    HTuple step_val164 = 1;
    for (hv_Index=0; hv_Index.Continue(end_val164, step_val164); hv_Index += step_val164)
    {
      SetDictTuple(hv_SplitImageIndices, HTuple(hv_SplitNames[hv_Index]), HTuple());
    }
    }
    //We split based on classes,
    //starting with the smallest class.
    hv_AssignedImageIndices = HTuple();
    hv_ClassSortIndices = hv_NumImagesPerClass.TupleSortIndex();
    {
    HTuple end_val171 = (hv_NumImagesPerClass.TupleLength())-1;
    HTuple step_val171 = 1;
    for (hv_ClassIndex=0; hv_ClassIndex.Continue(end_val171, step_val171); hv_ClassIndex += step_val171)
    {
      //Get all image indices where this class is present.
      GetDictTuple(hv_ImageIndicesPerClass, HTuple(hv_ClassNames[HTuple(hv_ClassSortIndices[hv_ClassIndex])]), 
          &hv_ImageIndicesClass);
      hv_ImageIndicesClass = hv_ImageIndicesClass.TupleUniq();
      //Remove image indices that have already been assigned.
      hv_ImageIndicesClassToBeAssigned = hv_ImageIndicesClass.TupleDifference(hv_AssignedImageIndices);
      tuple_shuffle(hv_ImageIndicesClassToBeAssigned, &hv_ImageIndicesClassToBeAssigned);
      //
      {
      HTuple end_val179 = (hv_SplitNames.TupleLength())-1;
      HTuple step_val179 = 1;
      for (hv_SplitIndex=0; hv_SplitIndex.Continue(end_val179, step_val179); hv_SplitIndex += step_val179)
      {
        //Check how many of the indices have already been assigned
        //and how many should be assigned.
        if (0 != (HTuple(int(hv_ModelType==HTuple("anomaly_detection"))).TupleAnd(int(HTuple(hv_ClassNames[HTuple(hv_ClassSortIndices[hv_ClassIndex])])==HTuple("nok")))))
        {
          //All 'nok' images for anomaly detection are sorted into the test set.
          if (0 != (int(HTuple(hv_SplitNames[hv_SplitIndex])==HTuple("test"))))
          {
            hv_NumToBeAssignedToThisSplit = hv_ImageIndicesClass.TupleLength();
          }
          else
          {
            hv_NumToBeAssignedToThisSplit = 0;
          }
        }
        else
        {
          hv_NumToBeAssignedToThisSplit = ((HTuple(hv_SplitRatios[hv_SplitIndex])*(hv_ImageIndicesClass.TupleLength())).TupleFloor()).TupleInt();
        }
        GetDictTuple(hv_SplitImageIndices, HTuple(hv_SplitNames[hv_SplitIndex]), 
            &hv_AssignedImageIndicesToThisSplit);
        hv_NumAlreadyAssignedToThisSplit = (hv_ImageIndicesClass.TupleIntersection(hv_AssignedImageIndicesToThisSplit)).TupleLength();
        hv_NumStillToBeAssigned = hv_NumToBeAssignedToThisSplit-hv_NumAlreadyAssignedToThisSplit;
        //
        if (0 != (int(hv_NumStillToBeAssigned>0)))
        {
          if (0 != (int(hv_NumStillToBeAssigned>(hv_ImageIndicesClassToBeAssigned.TupleLength()))))
          {
            hv_NumStillToBeAssigned = hv_ImageIndicesClassToBeAssigned.TupleLength();
          }
          hv_AssignedImageIndicesToThisSplit = hv_AssignedImageIndicesToThisSplit.TupleConcat(hv_ImageIndicesClassToBeAssigned.TupleSelectRange(0,hv_NumStillToBeAssigned-1));
          SetDictTuple(hv_SplitImageIndices, HTuple(hv_SplitNames[hv_SplitIndex]), 
              hv_AssignedImageIndicesToThisSplit);
          //Update the remaining image indices of this class.
          hv_ImageIndicesClassToBeAssigned = hv_ImageIndicesClassToBeAssigned.TupleSelectRange(hv_NumStillToBeAssigned,(hv_ImageIndicesClassToBeAssigned.TupleLength())-1);
        }
      }
      }
      //The remaining image indices are assigned to random subsets
      //according to the defined ratios.
      if (0 != (int((hv_ImageIndicesClassToBeAssigned.TupleLength())>0)))
      {
        {
        HTuple end_val209 = (hv_ImageIndicesClassToBeAssigned.TupleLength())-1;
        HTuple step_val209 = 1;
        for (hv_ImageIndex=0; hv_ImageIndex.Continue(end_val209, step_val209); hv_ImageIndex += step_val209)
        {
          hv_Rand = HTuple::TupleRand(1);
          {
          HTuple end_val211 = (hv_SplitRatios.TupleLength())-1;
          HTuple step_val211 = 1;
          for (hv_RatioIndex=0; hv_RatioIndex.Continue(end_val211, step_val211); hv_RatioIndex += step_val211)
          {
            hv_CurrentSplitIndex = HTuple(hv_SplitRatiosInvSortIndices[hv_RatioIndex]);
            hv_CurrentSplitRatio = HTuple(hv_SplitRatios[hv_CurrentSplitIndex]);
            if (0 != (int(hv_Rand<=hv_CurrentSplitRatio)))
            {
              GetDictTuple(hv_SplitImageIndices, HTuple(hv_SplitNames[hv_CurrentSplitIndex]), 
                  &hv_AssignedImageIndicesToThisSplit);
              hv_AssignedImageIndicesToThisSplit = hv_AssignedImageIndicesToThisSplit.TupleConcat(HTuple(hv_ImageIndicesClassToBeAssigned[hv_ImageIndex]));
              SetDictTuple(hv_SplitImageIndices, HTuple(hv_SplitNames[hv_CurrentSplitIndex]), 
                  hv_AssignedImageIndicesToThisSplit);
              break;
            }
            else
            {
              hv_Rand = hv_Rand-hv_CurrentSplitRatio;
            }
          }
          }
        }
        }
      }
      hv_AssignedImageIndices = ((hv_AssignedImageIndices.TupleConcat(hv_ImageIndicesClass)).TupleUniq()).TupleSort();
    }
    }
    //
    //There might be images not having any labels:
    //Assign them based on the ratio.
    hv_ImageIndicesWithoutLabel = hv_ImageIndices.TupleDifference(hv_AssignedImageIndices);
    hv_NumImageIndicesWithoutLabel = hv_ImageIndicesWithoutLabel.TupleLength();
    if (0 != (int(hv_NumImageIndicesWithoutLabel>0)))
    {
      tuple_shuffle(hv_ImageIndicesWithoutLabel, &hv_ImageIndicesWithoutLabel);
      {
      HTuple end_val234 = (hv_SplitRatios.TupleLength())-1;
      HTuple step_val234 = 1;
      for (hv_Index=0; hv_Index.Continue(end_val234, step_val234); hv_Index += step_val234)
      {
        hv_NumToBeAssigned = (HTuple(hv_SplitRatios[hv_Index])*hv_NumImageIndicesWithoutLabel).TupleInt();
        GetDictTuple(hv_SplitImageIndices, HTuple(hv_SplitNames[hv_Index]), &hv_AssignedImageIndicesToThisSplit);
        hv_AssignedImageIndicesToThisSplit = hv_AssignedImageIndicesToThisSplit.TupleConcat(hv_ImageIndicesWithoutLabel.TupleSelectRange(0,hv_NumToBeAssigned-1));
        SetDictTuple(hv_SplitImageIndices, HTuple(hv_SplitNames[hv_Index]), hv_AssignedImageIndicesToThisSplit.TupleSort());
        hv_ImageIndicesWithoutLabel = hv_ImageIndicesWithoutLabel.TupleSelectRange(hv_NumToBeAssigned,(hv_ImageIndicesWithoutLabel.TupleLength())-1);
      }
      }
      //If there are still image indices, assign them to split with highest ratio.
      hv_MaxRatioIndex = hv_SplitRatiosInvSortIndices.TupleFind(0);
      if (0 != (int((hv_ImageIndicesWithoutLabel.TupleLength())>0)))
      {
        GetDictTuple(hv_SplitImageIndices, HTuple(hv_SplitNames[hv_MaxRatioIndex]), 
            &hv_AssignedImageIndicesToThisSplit);
        hv_AssignedImageIndicesToThisSplit = hv_AssignedImageIndicesToThisSplit.TupleConcat(hv_ImageIndicesWithoutLabel);
        SetDictTuple(hv_SplitImageIndices, HTuple(hv_SplitNames[hv_MaxRatioIndex]), 
            hv_AssignedImageIndicesToThisSplit.TupleSort());
      }
    }
    //
    //Assign 'split' entries to samples.
    {
    HTuple end_val251 = (hv_SplitNames.TupleLength())-1;
    HTuple step_val251 = 1;
    for (hv_SplitNameIndex=0; hv_SplitNameIndex.Continue(end_val251, step_val251); hv_SplitNameIndex += step_val251)
    {
      hv_SplitName = HTuple(hv_SplitNames[hv_SplitNameIndex]);
      GetDictTuple(hv_SplitImageIndices, hv_SplitName, &hv_SplitIndices);
      {
      HTuple end_val254 = (hv_SplitIndices.TupleLength())-1;
      HTuple step_val254 = 1;
      for (hv_SplitIndex=0; hv_SplitIndex.Continue(end_val254, step_val254); hv_SplitIndex += step_val254)
      {
        hv_SampleSplitIndex = HTuple(hv_SplitIndices[hv_SplitIndex]);
        SetDictTuple(HTuple(hv_DLSamples[hv_SampleSplitIndex]), "split", hv_SplitName);
      }
      }
    }
    }
  }
  else
  {
    hv_ErrorDict = "The dataset is already split. You can overwrite the existing split using the generic parameter 'overwrite_split'.";
    throw HException(hv_ErrorDict);
  }
  return;
}

// Chapter: Deep Learning / Anomaly Detection
// Short Description: Apply the given thresholds on anomaly detection results for image classification and region segmentation. 
void threshold_dl_anomaly_results (HTuple hv_AnomalySegmentationThreshold, HTuple hv_AnomalyClassificationThreshold, 
    HTuple hv_DLResults)
{

  // Local iconic variables
  HObject  ho_AnomalyImage, ho_AnomalyRegion;

  // Local control variables
  HTuple  hv_DLResultIndex, hv_DLResult, hv_AnomalyImageExists;
  HTuple  hv_AnomalyScoreExists, hv_AnomalyScore;

  //This procedure applies given thresholds on anomaly detection results.
  //The thresholds are used for:
  //
  //1. Region segmentation: AnomalySegmentationThreshold is used as threshold
  //whether a pixel within the anomaly image belongs to a region of an anomaly.
  //The region is returned in DLResult under the key 'anomaly_region'.
  //2. Image classification: AnomalyClassificationThreshold is used as threshold
  //whether the image is classified as containing an anomaly ('nok' / class_id: 1) or not ('ok' / class_id: 0).
  //The class is returned in DLResult under the keys 'anomaly_class' and 'anomaly_class_id'.
  //
  //The applied thresholds are also stored in DLResult.
  //
  //Check for invalid AnomalySegmentationThreshold.
  if (0 != (int((hv_AnomalySegmentationThreshold.TupleLength())!=1)))
  {
    throw HException("AnomalySegmentationThreshold must be specified by exactly one value.");
  }
  //
  if (0 != (HTuple(int(hv_AnomalySegmentationThreshold<0.0)).TupleOr(int(hv_AnomalySegmentationThreshold>1.0))))
  {
    throw HException(HTuple("AnomalySegmentationThreshold must be in the interval [0.0, 1.0]."));
  }
  //
  //Check for invalid AnomalyClassificationThreshold.
  if (0 != (int((hv_AnomalyClassificationThreshold.TupleLength())!=1)))
  {
    throw HException("AnomalyClassificationThreshold must be specified by exactly one value.");
  }
  //
  if (0 != (int(hv_AnomalyClassificationThreshold<0.0)))
  {
    throw HException("AnomalyClassificationThreshold cannot be negative.");
  }
  //
  //Evaluate each DLResult.
  {
  HTuple end_val31 = (hv_DLResults.TupleLength())-1;
  HTuple step_val31 = 1;
  for (hv_DLResultIndex=0; hv_DLResultIndex.Continue(end_val31, step_val31); hv_DLResultIndex += step_val31)
  {
    //
    //Read anomaly image and anomaly score from DLResult.
    hv_DLResult = HTuple(hv_DLResults[hv_DLResultIndex]);
    GetDictParam(hv_DLResult, "key_exists", "anomaly_image", &hv_AnomalyImageExists);
    GetDictParam(hv_DLResult, "key_exists", "anomaly_score", &hv_AnomalyScoreExists);
    if (0 != (HTuple(hv_AnomalyImageExists.TupleNot()).TupleOr(hv_AnomalyScoreExists.TupleNot())))
    {
      throw HException("DLResult must contain keys 'anomaly_image' and 'anomaly_score'.");
    }
    //
    read_dict_object(&ho_AnomalyImage, hv_DLResult, "anomaly_image");
    read_dict_tuple(hv_DLResult, "anomaly_score", &hv_AnomalyScore);
    //
    //Apply AnomalyThreshold to the anomaly image.
    Threshold(ho_AnomalyImage, &ho_AnomalyRegion, hv_AnomalySegmentationThreshold, 
        1.0);
    //
    //Write AnomalyRegion to DLResult.
    SetDictObject(ho_AnomalyRegion, hv_DLResult, "anomaly_region");
    //
    //Classify sample as 'ok' or 'nok'.
    if (0 != (int(hv_AnomalyScore<hv_AnomalyClassificationThreshold)))
    {
      SetDictTuple(hv_DLResult, "anomaly_class", "ok");
      SetDictTuple(hv_DLResult, "anomaly_class_id", 0);
    }
    else
    {
      SetDictTuple(hv_DLResult, "anomaly_class", "nok");
      SetDictTuple(hv_DLResult, "anomaly_class_id", 1);
    }
    //
    //Write anomaly thresholds to DLResult.
    SetDictTuple(hv_DLResult, "anomaly_classification_threshold", hv_AnomalyClassificationThreshold);
    SetDictTuple(hv_DLResult, "anomaly_segmentation_threshold", hv_AnomalySegmentationThreshold);
  }
  }
  //
  return;
}

// Chapter: Time
// Short Description: Create a formatted string of a time span. 
void timespan_string (HTuple hv_TotalSeconds, HTuple hv_Format, HTuple *hv_TimeString)
{

  // Local iconic variables

  // Local control variables
  HTuple  hv_Seconds, hv_TotalMinutes, hv_Minutes;
  HTuple  hv_TotalHours, hv_Hours, hv_Days;

  //
  //This procedure creates a readable representation of a time span
  //given the elapsed time in seconds.
  //
  //Ensure that the input is an integer.
  hv_TotalSeconds = hv_TotalSeconds.TupleInt();
  //
  hv_Seconds = hv_TotalSeconds%60;
  //
  hv_TotalMinutes = hv_TotalSeconds/60;
  hv_Minutes = hv_TotalMinutes%60;
  //
  hv_TotalHours = hv_TotalSeconds/3600;
  hv_Hours = hv_TotalHours%24;
  //
  hv_Days = hv_TotalSeconds/86400;
  //
  if (0 != (int(hv_Format==HTuple("auto"))))
  {
    //Print the highest non-zero unit and all remaining sub-units.
    if (0 != (int(hv_Days>0)))
    {
      (*hv_TimeString) = (((((((hv_Days.TupleString("d"))+"d ")+(hv_Hours.TupleString("d")))+"h ")+(hv_Minutes.TupleString("d")))+"m ")+(hv_Seconds.TupleString("d")))+"s";
    }
    else if (0 != (int(hv_Hours>0)))
    {
      (*hv_TimeString) = (((((hv_Hours.TupleString("d"))+"h ")+(hv_Minutes.TupleString("d")))+"m ")+(hv_Seconds.TupleString("d")))+"s";
    }
    else if (0 != (int(hv_Minutes>0)))
    {
      (*hv_TimeString) = (((hv_Minutes.TupleString("d"))+"m ")+(hv_Seconds.TupleString("d")))+"s";
    }
    else
    {
      (*hv_TimeString) = (hv_Seconds.TupleString("d"))+"s";
    }
  }
  else if (0 != (int(hv_Format==HTuple("top1"))))
  {
    //Print the highest non-zero unit.
    if (0 != (int(hv_Days>0)))
    {
      (*hv_TimeString) = (hv_Days.TupleString("d"))+"d";
    }
    else if (0 != (int(hv_Hours>0)))
    {
      (*hv_TimeString) = (hv_Hours.TupleString("d"))+"h";
    }
    else if (0 != (int(hv_Minutes>0)))
    {
      (*hv_TimeString) = (hv_Minutes.TupleString("d"))+"m";
    }
    else
    {
      (*hv_TimeString) = (hv_Seconds.TupleString("d"))+"s";
    }
  }
  else if (0 != (int(hv_Format==HTuple("top2"))))
  {
    //Print the highest non-zero unit and the following sub-unit.
    if (0 != (int(hv_Days>0)))
    {
      (*hv_TimeString) = (((hv_Days.TupleString("d"))+"d ")+(hv_Hours.TupleString("d")))+"h";
    }
    else if (0 != (int(hv_Hours>0)))
    {
      (*hv_TimeString) = (((hv_Hours.TupleString("d"))+"h ")+(hv_Minutes.TupleString("d")))+"m";
    }
    else if (0 != (int(hv_Minutes>0)))
    {
      (*hv_TimeString) = (((hv_Minutes.TupleString("d"))+"m ")+(hv_Seconds.TupleString("d")))+"s";
    }
    else
    {
      (*hv_TimeString) = (hv_Seconds.TupleString("d"))+"s";
    }
  }
  else if (0 != (int(hv_Format==HTuple("dhms"))))
  {
    //Print a Days-Hours-Minutes-Seconds string.
    (*hv_TimeString) = (((((((hv_Days.TupleString("d"))+"d ")+(hv_Hours.TupleString("d")))+"h ")+(hv_Minutes.TupleString("d")))+"m ")+(hv_Seconds.TupleString("d")))+"s";
  }
  else if (0 != (int(hv_Format==HTuple("hms"))))
  {
    //Print a Hours-Minutes-Seconds string, where hours can be >= 24.
    (*hv_TimeString) = (((((hv_TotalHours.TupleString("d"))+"h ")+(hv_Minutes.TupleString("d")))+"m ")+(hv_Seconds.TupleString("d")))+"s";
  }
  else
  {
    throw HException("Unknown format string.");
  }
  //
  return;
}

// Chapter: Deep Learning / Model
// Short Description: Train a deep-learning-based model on a dataset. 
void train_dl_model (HTuple hv_DLDataset, HTuple hv_DLModelHandle, HTuple hv_TrainParam, 
    HTuple hv_StartEpoch, HTuple *hv_TrainResults, HTuple *hv_TrainInfos, HTuple *hv_EvaluationInfos)
{

  // Local iconic variables

  // Local control variables
  HTuple  hv_ModelType, hv_DLSamples, hv_TrainSampleIndices;
  HTuple  hv_NumTrainSamples, hv_NumEpochs, hv_SeedRand, hv_SampleIndicesTrainRaw;
  HTuple  hv_Index, hv_Shuffled, hv_SampleSeedsTrainRaw, hv_BatchSize;
  HTuple  hv_ChangeStrategyData, hv_SerializationData, hv_DisplayData;
  HTuple  hv_DisplayEnabled, hv_DisplayPreviewInitialized;
  HTuple  hv_DisplayEvaluationEpochs, hv_DisplayValidationEvaluationValues;
  HTuple  hv_DisplayTrainEvaluationValues, hv_DisplayLossEpochs;
  HTuple  hv_DisplayLoss, hv_DisplayLearningRates, hv_TrainResultsRestored;
  HTuple  hv_StartTime, hv_FirstIteration, hv_Epoch, hv_Iteration;
  HTuple  hv_NumIterationsPerEpoch, hv_BatchSizeDevice, hv_BatchSizeMultiplier;
  HTuple  hv_BatchSizeModel, hv_NumIterations, hv_SampleIndicesTrain;
  HTuple  hv_BatchStart, hv_BatchEnd, hv_BatchIndices, hv_DLSampleBatch;
  HTuple  hv_AugmentationParam, hv_TrainResult, hv_EvaluationIntervalEpochs;
  HTuple  hv_EvaluationInterval, hv_ValidationEvaluationResult;
  HTuple  hv_TrainEvaluationResult, hv_DisplayParam, hv_SelectPercentageTrainSamples;
  HTuple  hv_EvaluationParam, hv__, hv_TrainEvaluationSampleIndices;
  HTuple  hv_TrainEvaluationRatio, hv_NumTrainEvaluationSampleIndices;
  HTuple  hv_Exception, hv_EvaluationInfo, hv_EvaluationComparisonKeys;
  HTuple  hv_Valuevalidation, hv_ValueTrain, hv_TrainInfoUpdateIntervalSeconds;
  HTuple  hv_LastUpdate, hv_Seconds, hv_NumSamplesMeanLoss;
  HTuple  hv_TrainInfo, hv_UpdateTime, hv_EpochsStatus, hv_MeanLoss;
  HTuple  hv_DisplayLearningRate, hv_UpdateImagesIntervalEpochs;
  HTuple  hv_UpdateImagesInterval, hv_WindowImages, hv_FirstCall;
  HTuple  hv_NumImages, hv_GenParamTiled, hv_TrainParamAnomaly;
  HTuple  hv_EnableDisplay, hv_WindowHandleInfo;

  //
  //This procedure contains all steps for training a model given through DLModelHandle
  //on a dataset DLDataset.
  //The required training parameters are provided through the dictionary TrainParam,
  //which can be created by create_dl_train_param.
  //The training is started at StartEpoch, which allows resuming the training of a model.
  //In case of models of type 'anomaly_detection', training cannot be resumed and hence,
  //StartEpoch is always 0.
  //
  //The procedure returns three dictionaries:
  //- TrainResults: Collected results returned by train_dl_model_batch of every iteration.
  //                For models of type 'anomaly_detection': The final error and the final epoch.
  //- TrainInfo: Collected information of the training progress. This dictionary is empty
  //             for models of type 'anomaly_detection'.
  //- EvaluationInfos: Evaluation results collected during training. This dictionary is empty
  //                   for models of type 'anomaly_detection'.
  //
  //Get the model type.
  GetDlModelParam(hv_DLModelHandle, "type", &hv_ModelType);
  if (0 != (HTuple(HTuple(HTuple(int(hv_ModelType!=HTuple("classification"))).TupleAnd(int(hv_ModelType!=HTuple("detection")))).TupleAnd(int(hv_ModelType!=HTuple("segmentation")))).TupleAnd(int(hv_ModelType!=HTuple("anomaly_detection")))))
  {
    throw HException("Invalid model type");
  }
  //
  //Get the samples for training.
  GetDictTuple(hv_DLDataset, "samples", &hv_DLSamples);
  find_dl_samples(hv_DLSamples, "split", "train", "match", &hv_TrainSampleIndices);
  hv_NumTrainSamples = hv_TrainSampleIndices.TupleLength();
  //
  //Check inconsistent training parameters.
  check_train_dl_model_params(hv_DLDataset, hv_DLModelHandle, hv_NumTrainSamples, 
      hv_StartEpoch, hv_TrainParam);
  //
  if (0 != (HTuple(HTuple(int(hv_ModelType==HTuple("classification"))).TupleOr(int(hv_ModelType==HTuple("detection")))).TupleOr(int(hv_ModelType==HTuple("segmentation")))))
  {
    //
    //Check if training is required.
    GetDictTuple(hv_TrainParam, "num_epochs", &hv_NumEpochs);
    if (0 != (hv_StartEpoch.TupleIsNumber()))
    {
      if (0 != (int(hv_StartEpoch>=hv_NumEpochs)))
      {
        //Nothing to do.
        return;
      }
    }
    //
    //Set random seed according to parameter value.
    GetDictTuple(hv_TrainParam, "seed_rand", &hv_SeedRand);
    if (0 != (int((hv_SeedRand.TupleLength())>0)))
    {
      //Note, that setting this random seed will not enforce every training to
      //result in the exact same model because the cuDNN library uses approximate
      //algorithms on some architectures.
      //If you want to enforce bit-wise reproducibility, you should also set:
      //   'set_system('cudnn_deterministic', 'true')'
      //However, this can slow down computations on some architectures.
      SetSystem("seed_rand", hv_SeedRand);
    }
    //
    //Generate a random sample index for the whole training independent of batch size.
    hv_SampleIndicesTrainRaw = HTuple();
    {
    HTuple end_val56 = (hv_NumEpochs.TupleCeil())-1;
    HTuple step_val56 = 1;
    for (hv_Index=0; hv_Index.Continue(end_val56, step_val56); hv_Index += step_val56)
    {
      tuple_shuffle(hv_TrainSampleIndices, &hv_Shuffled);
      hv_SampleIndicesTrainRaw = hv_SampleIndicesTrainRaw.TupleConcat(hv_Shuffled);
    }
    }
    //
    //Generate a random seed pool for the whole training independent of batch size.
    hv_SampleSeedsTrainRaw = HTuple(((HTuple(2).TuplePow(31))-1)*HTuple::TupleRand(hv_SampleIndicesTrainRaw.TupleLength())).TupleInt();
    //
    //Initialize the variables for the training.
    //
    //Initialize the batch size with an invalid value so that
    //the while loop will initialize all values directly.
    hv_BatchSize = -1;
    //
    //Initialize change strategies.
    init_train_dl_model_change_strategies(hv_TrainParam, &hv_ChangeStrategyData);
    //
    //Initialize serialization strategies.
    init_train_dl_model_serialization_strategies(hv_TrainParam, &hv_SerializationData);
    //
    //Initialize visualizations if enabled.
    dev_display_init_train_dl_model(hv_DLModelHandle, hv_TrainParam, &hv_DisplayData);
    GetDictTuple(hv_DisplayData, "enabled", &hv_DisplayEnabled);
    hv_DisplayPreviewInitialized = 0;
    //
    //Initialize parameters to start new or resume previous training.
    restore_dl_train_info_for_resuming(hv_StartEpoch, hv_SerializationData, hv_TrainParam, 
        hv_DisplayData, &(*hv_EvaluationInfos), &(*hv_TrainInfos), &hv_DisplayEvaluationEpochs, 
        &hv_DisplayValidationEvaluationValues, &hv_DisplayTrainEvaluationValues, 
        &hv_DisplayLossEpochs, &hv_DisplayLoss, &hv_DisplayLearningRates, &hv_TrainResultsRestored, 
        &hv_StartEpoch);
    //
    //Start time for measurement of elapsed training time.
    CountSeconds(&hv_StartTime);
    //
    //The while loop needs to know if it is the very first iteration.
    hv_FirstIteration = 1;
    while (0 != 1)
    {
      //Do some initializations only for the very first iteration.
      if (0 != hv_FirstIteration)
      {
        //Jump to StartEpoch (Default: 0 but it could be used to resume training at given StartIteration).
        hv_Epoch = hv_StartEpoch;
      }
      else
      {
        hv_Epoch = (hv_Iteration+1)/(hv_NumIterationsPerEpoch.TupleReal());
      }
      //
      //Update any parameters based on strategies.
      update_train_dl_model_change_strategies(hv_DLModelHandle, hv_ChangeStrategyData, 
          hv_Epoch);
      //
      //Check if the current batch size and total model batch size differ.
      GetDlModelParam(hv_DLModelHandle, "batch_size", &hv_BatchSizeDevice);
      GetDlModelParam(hv_DLModelHandle, "batch_size_multiplier", &hv_BatchSizeMultiplier);
      hv_BatchSizeModel = hv_BatchSizeDevice*hv_BatchSizeMultiplier;
      //
      if (0 != (HTuple(int(hv_BatchSize!=hv_BatchSizeModel)).TupleOr(hv_FirstIteration)))
      {
        //Set the current value.
        hv_BatchSize = hv_BatchSizeModel;
        //Now, we compute all values which are related to the batch size of the model.
        //That way, the batch_size can be changed during the training without issues.
        //All inputs/outputs/visualizations are based on epochs.
        //
        //Calculate total number of iterations.
        hv_NumIterationsPerEpoch = ((hv_NumTrainSamples/(hv_BatchSize.TupleReal())).TupleFloor()).TupleInt();
        hv_NumIterations = (hv_NumIterationsPerEpoch*hv_NumEpochs).TupleInt();
        //Select those indices that fit into the batch size.
        hv_SampleIndicesTrain = hv_SampleIndicesTrainRaw.TupleSelectRange(0,(hv_NumIterations*hv_BatchSize)-1);
        //The TrainResults tuple will be updated every iteration.
        //Hence, we initialize it as a constant tuple for speedup.
        //It is based on the iterations and hence cannot be reused if the batch size changes.
        TupleGenConst(hv_NumIterations, -1, &(*hv_TrainResults));
        if (0 != (hv_FirstIteration.TupleNot()))
        {
          hv_Iteration = (((hv_Epoch.TupleReal())*hv_NumIterationsPerEpoch).TupleFloor()).TupleInt();
          hv_Epoch = (hv_Iteration+1)/(hv_NumIterationsPerEpoch.TupleReal());
        }
      }
      //
      //In the first iteration do some initializations.
      if (0 != hv_FirstIteration)
      {
        //Jump to StartEpoch (Default: 0 but it could be used to resume training at given StartIteration).
        hv_Iteration = (((hv_StartEpoch.TupleReal())*hv_NumIterationsPerEpoch).TupleFloor()).TupleInt();
        hv_FirstIteration = 0;
        if (0 != (int(((hv_Iteration*hv_BatchSize)+hv_BatchSize)>(hv_SampleIndicesTrain.TupleLength()))))
        {
          hv_Iteration = hv_NumIterations-1;
          break;
        }
        if (0 != (HTuple(int(hv_StartEpoch>0.0)).TupleAnd(int((hv_TrainResultsRestored.TupleLength())>0))))
        {
          //Overwrite the first train results.
          if (0 != (int((hv_TrainResultsRestored.TupleLength())>hv_Iteration)))
          {
            hv_TrainResultsRestored = hv_TrainResultsRestored.TupleSelectRange((hv_TrainResultsRestored.TupleLength())-hv_Iteration,(hv_TrainResultsRestored.TupleLength())-1);
          }
          (*hv_TrainResults)[HTuple::TupleGenSequence(hv_Iteration-(hv_TrainResultsRestored.TupleLength()),hv_Iteration-1,1)] = hv_TrainResultsRestored;
        }
      }
      //
      //Generate the sample batch indices.
      hv_BatchStart = hv_Iteration*hv_BatchSize;
      hv_BatchEnd = (hv_BatchStart+hv_BatchSize)-1;
      hv_BatchIndices = hv_SampleIndicesTrain.TupleSelectRange(hv_BatchStart,hv_BatchEnd);
      //
      //Set a random seed for the sample batch.
      SetSystem("seed_rand", HTuple(hv_SampleSeedsTrainRaw[hv_BatchEnd]));
      //
      //Read preprocessed samples.
      read_dl_samples(hv_DLDataset, hv_BatchIndices, &hv_DLSampleBatch);
      //
      //Augment samples based on train parameter.
      GetDictTuple(hv_TrainParam, "augmentation_param", &hv_AugmentationParam);
      augment_dl_samples(hv_DLSampleBatch, hv_AugmentationParam);
      //
      //Train the model on current batch.
      TrainDlModelBatch(hv_DLModelHandle, hv_DLSampleBatch, &hv_TrainResult);
      //
      //We store each train result.
      (*hv_TrainResults)[hv_Iteration] = hv_TrainResult;
      //
      //Evaluation handling.
      GetDictTuple(hv_TrainParam, "evaluation_interval_epochs", &hv_EvaluationIntervalEpochs);
      hv_EvaluationInterval = ((hv_EvaluationIntervalEpochs*hv_NumIterationsPerEpoch).TupleFloor()).TupleInt();
      hv_ValidationEvaluationResult = HTuple();
      hv_TrainEvaluationResult = HTuple();
      GetDictTuple(hv_DisplayData, "display_param", &hv_DisplayParam);
      //Get percentage of evaluated training samples from display parameters.
      GetDictTuple(hv_DisplayParam, "selected_percentage_train_samples", &hv_SelectPercentageTrainSamples);
      if (0 != (int(hv_EvaluationInterval>0)))
      {
        //Evaluate the model at given intervals.
        if (0 != (HTuple(HTuple(int(((hv_Iteration+1)%hv_EvaluationInterval)==0)).TupleAnd(int(hv_Iteration!=0))).TupleOr(int(hv_Iteration==(hv_NumIterations-1)))))
        {
          GetDictTuple(hv_TrainParam, "evaluation_param", &hv_EvaluationParam);
          //Evaluate on validation split.
          evaluate_dl_model(hv_DLDataset, hv_DLModelHandle, "split", "validation", 
              hv_EvaluationParam, &hv_ValidationEvaluationResult, &hv__);
          //Evaluate a subset of the train split.
          tuple_shuffle(hv_TrainSampleIndices, &hv_TrainEvaluationSampleIndices);
          hv_TrainEvaluationRatio = hv_SelectPercentageTrainSamples/100.0;
          hv_NumTrainEvaluationSampleIndices = (hv_TrainEvaluationRatio*(hv_TrainEvaluationSampleIndices.TupleLength())).TupleInt();
          if (0 != (int(hv_NumTrainEvaluationSampleIndices>0)))
          {
            //It might happen that the subset is too small for evaluation.
            try
            {
              evaluate_dl_model(hv_DLDataset, hv_DLModelHandle, "sample_indices", 
                  hv_TrainEvaluationSampleIndices.TupleSelectRange(0,hv_NumTrainEvaluationSampleIndices-1), 
                  hv_EvaluationParam, &hv_TrainEvaluationResult, &hv__);
            }
            // catch (Exception) 
            catch (HException &HDevExpDefaultException)
            {
              HDevExpDefaultException.ToHTuple(&hv_Exception);
            }
          }
          CreateDict(&hv_EvaluationInfo);
          SetDictTuple(hv_EvaluationInfo, "epoch", hv_Epoch);
          SetDictTuple(hv_EvaluationInfo, "iteration", hv_Iteration);
          SetDictTuple(hv_EvaluationInfo, "result", hv_ValidationEvaluationResult);
          SetDictTuple(hv_EvaluationInfo, "result_train", hv_TrainEvaluationResult);
          (*hv_EvaluationInfos) = (*hv_EvaluationInfos).TupleConcat(hv_EvaluationInfo);
          if (0 != hv_DisplayEnabled)
          {
            GetDictTuple(hv_TrainParam, "evaluation_comparison_keys", &hv_EvaluationComparisonKeys);
            reduce_dl_evaluation_result(hv_ValidationEvaluationResult, hv_EvaluationComparisonKeys, 
                &hv_Valuevalidation, &hv__);
            reduce_dl_evaluation_result(hv_TrainEvaluationResult, hv_EvaluationComparisonKeys, 
                &hv_ValueTrain, &hv__);
            hv_DisplayValidationEvaluationValues = hv_DisplayValidationEvaluationValues.TupleConcat(hv_Valuevalidation);
            hv_DisplayTrainEvaluationValues = hv_DisplayTrainEvaluationValues.TupleConcat(hv_ValueTrain);
            hv_DisplayEvaluationEpochs = hv_DisplayEvaluationEpochs.TupleConcat(hv_Epoch);
          }
        }
      }
      //
      //Check if an update is needed.
      GetDictTuple(hv_TrainParam, "update_interval_seconds", &hv_TrainInfoUpdateIntervalSeconds);
      GetDictTuple(hv_DisplayData, "last_update", &hv_LastUpdate);
      CountSeconds(&hv_Seconds);
      //Check for next update (enough time has elapsed or last iteration).
      if (0 != (HTuple(HTuple(int(((hv_LastUpdate-hv_Seconds).TupleAbs())>hv_TrainInfoUpdateIntervalSeconds)).TupleOr(int(hv_Iteration==(hv_NumIterations-1)))).TupleOr(int((hv_ValidationEvaluationResult.TupleLength())>0))))
      {
        SetDictTuple(hv_DisplayData, "last_update", hv_Seconds);
        GetDictTuple(hv_TrainParam, "evaluation_comparison_keys", &hv_EvaluationComparisonKeys);
        GetDictTuple(hv_TrainParam, "num_samples_mean_loss", &hv_NumSamplesMeanLoss);
        collect_train_dl_model_info(hv_DLModelHandle, (*hv_TrainResults), (*hv_EvaluationInfos), 
            hv_EvaluationComparisonKeys, hv_Iteration, hv_NumIterations, hv_NumIterationsPerEpoch, 
            hv_NumSamplesMeanLoss, &hv_TrainInfo);
        SetDictTuple(hv_TrainInfo, "start_epoch", hv_StartEpoch);
        SetDictTuple(hv_TrainInfo, "start_time", hv_StartTime);
        CountSeconds(&hv_UpdateTime);
        SetDictTuple(hv_TrainInfo, "time_elapsed", hv_UpdateTime-hv_StartTime);
        (*hv_TrainInfos) = (*hv_TrainInfos).TupleConcat(hv_TrainInfo);
        //
        //Display handling.
        if (0 != hv_DisplayEnabled)
        {
          GetDictTuple(hv_TrainInfo, "epoch", &hv_EpochsStatus);
          hv_DisplayLossEpochs = hv_DisplayLossEpochs.TupleConcat(hv_EpochsStatus);
          GetDictTuple(hv_TrainInfo, "mean_loss", &hv_MeanLoss);
          hv_DisplayLoss = hv_DisplayLoss.TupleConcat(hv_MeanLoss);
          GetDlModelParam(hv_DLModelHandle, "learning_rate", &hv_DisplayLearningRate);
          hv_DisplayLearningRates = hv_DisplayLearningRates.TupleConcat(hv_DisplayLearningRate);
          dev_display_update_train_dl_model(hv_TrainParam, hv_DisplayData, hv_TrainInfo, 
              hv_DisplayLossEpochs, hv_DisplayLoss, hv_DisplayLearningRates, hv_DisplayEvaluationEpochs, 
              hv_DisplayValidationEvaluationValues, hv_DisplayTrainEvaluationValues);
        }
      }
      //Image result preview handling.
      if (0 != hv_DisplayEnabled)
      {
        //Check if the image preview has to be updated.
        GetDictTuple(hv_DisplayParam, "update_images_interval_epochs", &hv_UpdateImagesIntervalEpochs);
        hv_UpdateImagesInterval = (((hv_UpdateImagesIntervalEpochs.TupleReal())*hv_NumIterationsPerEpoch).TupleFloor()).TupleInt();
        if (0 != (int(hv_UpdateImagesInterval==0)))
        {
          hv_UpdateImagesInterval = 1;
        }
        if (0 != (HTuple(int((hv_Iteration%hv_UpdateImagesInterval)==0)).TupleOr(hv_DisplayPreviewInitialized.TupleNot())))
        {
          GetDictTuple(hv_DisplayData, "window_images", &hv_WindowImages);
          hv_FirstCall = int((hv_WindowImages.TupleLength())==0);
          GetDictTuple(hv_DisplayParam, "num_images", &hv_NumImages);
          GetDictTuple(hv_DisplayParam, "tiled_param", &hv_GenParamTiled);
          //
          dev_display_dl_data_tiled(hv_DLDataset, hv_DLModelHandle, hv_NumImages, 
              "validation", hv_GenParamTiled, hv_WindowImages, &hv_WindowImages);
          //
          if (0 != hv_FirstCall)
          {
            SetDictTuple(hv_DisplayData, "window_images", hv_WindowImages);
            set_display_font(hv_WindowImages, 12, "mono", "true", "false");
          }
          dev_display_tiled_legend(hv_WindowImages, hv_GenParamTiled);
          hv_DisplayPreviewInitialized = 1;
        }
      }
      //
      //Serialization handling.
      update_train_dl_model_serialization(hv_TrainParam, hv_SerializationData, hv_Iteration, 
          hv_NumIterations, hv_Epoch, hv_ValidationEvaluationResult, hv_DLModelHandle, 
          (*hv_TrainInfos), (*hv_EvaluationInfos));
      //
      //Check for end of training.
      if (0 != (int(hv_Iteration>=(hv_NumIterations-1))))
      {
        break;
      }
      //
      //Continue with next iteration.
      hv_Iteration += 1;
    }
    //
  }
  else if (0 != (int(hv_ModelType==HTuple("anomaly_detection"))))
  {
    //Read the training samples.
    read_dl_samples(hv_DLDataset, hv_TrainSampleIndices, &hv_DLSamples);
    //
    //Get training parameters for anomaly detection.
    GetDictTuple(hv_TrainParam, "anomaly_param", &hv_TrainParamAnomaly);
    //
    //Display information about training.
    GetDictTuple(hv_TrainParam, "display_param", &hv_DisplayParam);
    GetDictTuple(hv_DisplayParam, "enabled", &hv_EnableDisplay);
    if (0 != hv_EnableDisplay)
    {
      dev_display_train_info_anomaly_detection(hv_TrainParam, &hv_WindowHandleInfo);
    }
    //
    //Train the model.
    TrainDlModelAnomalyDataset(hv_DLModelHandle, hv_DLSamples, hv_TrainParamAnomaly, 
        &(*hv_TrainResults));
    //
    //Initialize TrainInfos and EvaluationInfos
    (*hv_TrainInfos) = HTuple();
    (*hv_EvaluationInfos) = HTuple();
    //
    //Close window with information about the training.
    if (0 != hv_EnableDisplay)
    {
      HDevWindowStack::SetActive(hv_WindowHandleInfo);
    }
  }
  //
  return;
}

// Chapter: Tuple / Element Order
// Short Description: Sort the elements of a tuple randomly. 
void tuple_shuffle (HTuple hv_Tuple, HTuple *hv_Shuffled)
{

  // Local iconic variables

  // Local control variables
  HTuple  hv_ShuffleIndices;

  //This procedure sorts the input tuple randomly.
  //
  if (0 != (int((hv_Tuple.TupleLength())>0)))
  {
    //Create a tuple of random numbers,
    //sort this tuple, and return the indices
    //of this sorted tuple.
    hv_ShuffleIndices = HTuple::TupleRand(hv_Tuple.TupleLength()).TupleSortIndex();
    //Assign the elements of Tuple
    //to these random positions.
    (*hv_Shuffled) = HTuple(hv_Tuple[hv_ShuffleIndices]);
  }
  else
  {
    //If the input tuple is empty,
    //an empty tuple should be returned.
    (*hv_Shuffled) = HTuple();
  }
  return;
}

// Chapter: Deep Learning / Evaluation
// Short Description: Update RunningMeasures by evaluating Samples and corresponding Results. 
void update_running_evaluation_measures (HTuple hv_Samples, HTuple hv_Results, HTuple hv_EvalParams, 
    HTuple hv_RunningMeasures)
{

  // Local control variables
  HTuple  hv_EvaluationType;

  //
  //This procedure updates the running measures depending on the evaluation type.
  GetDictTuple(hv_EvalParams, "evaluation_type", &hv_EvaluationType);
  if (0 != (int(hv_EvaluationType==HTuple("anomaly_detection"))))
  {
    update_running_image_anomaly_measures(hv_Samples, hv_Results, hv_EvalParams, 
        hv_RunningMeasures);
  }
  else if (0 != (int(hv_EvaluationType==HTuple("classification"))))
  {
    update_running_image_classification_measures(hv_Samples, hv_Results, hv_EvalParams, 
        hv_RunningMeasures);
  }
  else if (0 != (int(hv_EvaluationType==HTuple("detection"))))
  {
    update_running_instance_measures(hv_Samples, hv_Results, hv_EvalParams, hv_RunningMeasures);
  }
  else if (0 != (int(hv_EvaluationType==HTuple("segmentation"))))
  {
    update_running_pixel_measures(hv_Samples, hv_Results, hv_EvalParams, hv_RunningMeasures);
  }
  //
  return;
}

// Chapter: Deep Learning / Anomaly Detection
// Short Description: Update running measures for an image anomaly detection evaluation. 
void update_running_image_anomaly_measures (HTuple hv_Samples, HTuple hv_Results, 
    HTuple hv_EvalParams, HTuple hv_RunningMeasures)
{

  // Local iconic variables

  // Local control variables
  HTuple  hv_ImageIDs, hv_AnomalyLabelIDs, hv_AnomalyScores;
  HTuple  hv_SampleIndex, hv_Sample, hv_ImageID, hv_AnomalyLabelID;
  HTuple  hv_Result, hv_AnomalyScore;

  //
  //This procedure updates the RunningMeasures for an evaluation for anomaly detection.
  //
  //These measures are stored in the dictionary RunningMeasures and
  //updated by incorporating the Results the model obtained for the Samples.
  //
  //
  //Get image ids.
  GetDictTuple(hv_RunningMeasures, "image_ids", &hv_ImageIDs);
  //Get anomaly label ids.
  GetDictTuple(hv_RunningMeasures, "anomaly_label_ids", &hv_AnomalyLabelIDs);
  //Get anomaly scores.
  GetDictTuple(hv_RunningMeasures, "anomaly_scores", &hv_AnomalyScores);
  //Loop over all samples and update running measures accordingly.
  {
  HTuple end_val14 = (hv_Samples.TupleLength())-1;
  HTuple step_val14 = 1;
  for (hv_SampleIndex=0; hv_SampleIndex.Continue(end_val14, step_val14); hv_SampleIndex += step_val14)
  {
    hv_Sample = HTuple(hv_Samples[hv_SampleIndex]);
    GetDictTuple(hv_Sample, "image_id", &hv_ImageID);
    GetDictTuple(hv_Sample, "anomaly_label_id", &hv_AnomalyLabelID);
    hv_Result = HTuple(hv_Results[hv_SampleIndex]);
    GetDictTuple(hv_Result, "anomaly_score", &hv_AnomalyScore);
    //
    hv_ImageIDs = hv_ImageIDs.TupleConcat(hv_ImageID);
    hv_AnomalyLabelIDs = hv_AnomalyLabelIDs.TupleConcat(hv_AnomalyLabelID);
    hv_AnomalyScores = hv_AnomalyScores.TupleConcat(hv_AnomalyScore);
  }
  }
  //
  //Set image ids in running measures.
  SetDictTuple(hv_RunningMeasures, "image_ids", hv_ImageIDs);
  //Set anomaly label ids in running measures.
  SetDictTuple(hv_RunningMeasures, "anomaly_label_ids", hv_AnomalyLabelIDs);
  //Set anomaly scores in running measures.
  SetDictTuple(hv_RunningMeasures, "anomaly_scores", hv_AnomalyScores);
  //
  return;
}

// Chapter: Deep Learning / Evaluation
// Short Description: Update running measures for an image classification evaluation. 
void update_running_image_classification_measures (HTuple hv_Samples, HTuple hv_Results, 
    HTuple hv_EvalParams, HTuple hv_RunningMeasures)
{

  // Local iconic variables

  // Local control variables
  HTuple  hv_RegExpTopKError, hv_Measures, hv_K;
  HTuple  hv_M, hv_ComputeTopKError, hv_ImageIDs, hv_ImageLabelIDs;
  HTuple  hv_Top1Prediction, hv_TopKPredictionDicts, hv_Index;
  HTuple  hv_Sample, hv_ImageID, hv_ImageLabelID, hv_Result;
  HTuple  hv_PredictedClassIDs, hv_TopKPrediction, hv_TopKPredictionDict;

  //
  //This procedure updates the RunningMeasures for an evaluation for classification.
  //
  //To avoid memory, only save first K predictions per sample.
  hv_RegExpTopKError = "top([0-9]+)_error";
  GetDictTuple(hv_EvalParams, "measures", &hv_Measures);
  hv_K = 1;
  {
  HTuple end_val7 = (hv_Measures.TupleLength())-1;
  HTuple step_val7 = 1;
  for (hv_M=0; hv_M.Continue(end_val7, step_val7); hv_M += step_val7)
  {
    hv_ComputeTopKError = HTuple(hv_Measures[hv_M]).TupleRegexpTest(hv_RegExpTopKError);
    if (0 != hv_ComputeTopKError)
    {
      hv_K = hv_K.TupleMax2((HTuple(hv_Measures[hv_M]).TupleRegexpMatch(hv_RegExpTopKError)).TupleNumber());
    }
  }
  }
  //
  //Extend tuples in RunningMeasures with new results.
  GetDictTuple(hv_RunningMeasures, "image_ids", &hv_ImageIDs);
  GetDictTuple(hv_RunningMeasures, "image_label_ids", &hv_ImageLabelIDs);
  GetDictTuple(hv_RunningMeasures, "top1_predictions", &hv_Top1Prediction);
  GetDictTuple(hv_RunningMeasures, "topk_predictions", &hv_TopKPredictionDicts);
  {
  HTuple end_val19 = (hv_Samples.TupleLength())-1;
  HTuple step_val19 = 1;
  for (hv_Index=0; hv_Index.Continue(end_val19, step_val19); hv_Index += step_val19)
  {
    hv_Sample = HTuple(hv_Samples[hv_Index]);
    GetDictTuple(hv_Sample, "image_id", &hv_ImageID);
    GetDictTuple(hv_Sample, "image_label_id", &hv_ImageLabelID);
    hv_Result = HTuple(hv_Results[hv_Index]);
    GetDictTuple(hv_Result, "classification_class_ids", &hv_PredictedClassIDs);
    hv_TopKPrediction = hv_PredictedClassIDs.TupleSelectRange(0,hv_K-1);
    CreateDict(&hv_TopKPredictionDict);
    SetDictTuple(hv_TopKPredictionDict, "predictions", hv_TopKPrediction);
    //
    hv_ImageIDs = hv_ImageIDs.TupleConcat(hv_ImageID);
    hv_ImageLabelIDs = hv_ImageLabelIDs.TupleConcat(hv_ImageLabelID);
    hv_Top1Prediction = hv_Top1Prediction.TupleConcat(HTuple(hv_TopKPrediction[0]));
    hv_TopKPredictionDicts = hv_TopKPredictionDicts.TupleConcat(hv_TopKPredictionDict);
  }
  }
  //
  SetDictTuple(hv_RunningMeasures, "image_ids", hv_ImageIDs);
  SetDictTuple(hv_RunningMeasures, "image_label_ids", hv_ImageLabelIDs);
  SetDictTuple(hv_RunningMeasures, "top1_predictions", hv_Top1Prediction);
  SetDictTuple(hv_RunningMeasures, "topk_predictions", hv_TopKPredictionDicts);
  //
  return;
}

// Chapter: Deep Learning / Evaluation
// Short Description: Update running measures for an instance-based evaluation. 
void update_running_instance_measures (HTuple hv_Samples, HTuple hv_Results, HTuple hv_EvalParams, 
    HTuple hv_RunningMeasures)
{

  // Local iconic variables

  // Local control variables
  HTuple  hv_MaxNumDetections, hv_AreaRanges, hv_IoUThresholds;
  HTuple  hv_InstanceType, hv_ClassIDs, hv_NumClasses, hv_Measures;
  HTuple  hv_AreaNames, hv_MinAreas, hv_MaxAreas, hv_NumAreaRanges;
  HTuple  hv_AllocationBlockLength, hv_DetailedEvaluation;
  HTuple  hv_KeyExists, hv_ClassIDToClassIdx, hv_EvaluateOrientation;
  HTuple  hv_SIdx, hv_CurrentSample, hv_CurrentResult, hv_GtClassIDs;
  HTuple  hv_ResClassIDs, hv_NumGT, hv_NumRes, hv_Confidences;
  HTuple  hv_ResSortIndices, hv_GtAreas, hv_ResAreas, hv_IoUs;
  HTuple  hv_GtPhis, hv_ResPhis, hv_MDIdx, hv_MaxNum, hv_MaxNumStr;
  HTuple  hv_CurrentRunningMeasures, hv_AreaIdx, hv_MinArea;
  HTuple  hv_MaxArea, hv_AreaName, hv_AreaRunningMeasures;
  HTuple  hv_GtIgnore, hv_GtIgnoreInds, hv_PerClassNumGt;
  HTuple  hv_PerClassNumPred, hv_PerClassConfidences, hv_PerClassNumGtIgnore;
  HTuple  hv_SampleHasFP, hv_SampleHasFN, hv_ClsIdx, hv_CurrentClassID;
  HTuple  hv_CurrentGtIdxs, hv_CurrentNumGt, hv_CurrentGtIgnore;
  HTuple  hv_CurrentNumGtIgnore, hv_CurrentNumGtNoIgnore;
  HTuple  hv_CurrentResIdxs, hv_CurrentNumRes, hv_CurrentResAreas;
  HTuple  hv_OldNumPred, hv_CurrentClassConfidences, hv_GtSortIdx;
  HTuple  hv_CurrentResPhis, hv_CurrentGtPhis, hv_ITIdx, hv_GtMatched;
  HTuple  hv_ResMatched, hv_ResAbsOrientationDiff, hv_ResIgnore;
  HTuple  hv_ResIdx, hv_CurrentIoU, hv_MatchIdx, hv_GtIdx;
  HTuple  hv_AreaIgnore, hv_PerIoUMeasure, hv_PerClassMeasures;
  HTuple  hv_CurrentIsTP, hv_CurrentIgnore, hv_CurrentAbsOrientationDiff;
  HTuple  hv_GtMatchedNoIgnore, hv_ResIsFPClass, hv_ResIsFPBackground;
  HTuple  hv_ResIsFPLocalization, hv_ResIsFPDuplicate, hv_ResIsFPMultiple;
  HTuple  hv_ResAbsOrientationDiffClass, hv_ResAbsOrientationDiffLocalization;
  HTuple  hv_ResAbsOrientationDiffDuplicate, hv_ResAbsOrientationDiffMultiple;
  HTuple  hv_FPResIdxsThisClass, hv_FPResIdxsAllResults, hv_GTIdxsNotToIgnore;
  HTuple  hv_MaxIoU, hv_IoUsWithGT, hv_MaxIdx, hv_GTClassIDMaxIoU;
  HTuple  hv_AbsOrientationDiff, hv_IsFPClass, hv_IsFPBackground;
  HTuple  hv_IsFPLocalization, hv_IsFPDuplicate, hv_IsFPMultiple;
  HTuple  hv_AbsOrientationDiffMultiple, hv_AbsOrientationDiffDuplicate;
  HTuple  hv_AbsOrientationDiffLocalization, hv_AbsOrientationDiffClass;
  HTuple  hv_CurrentImageID, hv_ImageIDsWithFN, hv_NumImageIDsWithFN;
  HTuple  hv_ImageIDsWithFP, hv_NumImageIDsWithFP;

  //
  //This procedure updates the RunningMeasures
  //for an instance-based evaluation for detection.
  //These measures are stored in the dictionary RunningMeasures and
  //updated by incorporating the Results the model obtained for the Samples.
  //
  dev_update_off();
  //Get the necessary evaluation parameters.
  GetDictTuple(hv_EvalParams, "max_num_detections", &hv_MaxNumDetections);
  GetDictTuple(hv_EvalParams, "area_ranges", &hv_AreaRanges);
  GetDictTuple(hv_EvalParams, "iou_threshold", &hv_IoUThresholds);
  GetDictTuple(hv_EvalParams, "instance_type", &hv_InstanceType);
  GetDictTuple(hv_EvalParams, "class_ids", &hv_ClassIDs);
  GetDictTuple(hv_EvalParams, "num_classes", &hv_NumClasses);
  GetDictTuple(hv_EvalParams, "measures", &hv_Measures);
  //
  //Get the area parameters: name, min, and max.
  GetDictTuple(hv_AreaRanges, "name", &hv_AreaNames);
  GetDictTuple(hv_AreaRanges, "min", &hv_MinAreas);
  GetDictTuple(hv_AreaRanges, "max", &hv_MaxAreas);
  hv_NumAreaRanges = (hv_AreaNames.TupleLength())-1;
  //
  //Get the allocation length for extending tuples.
  GetDictTuple(hv_EvalParams, "allocation_block_length", &hv_AllocationBlockLength);
  //
  //Check if a detailed evaluation should be done.
  hv_DetailedEvaluation = 0;
  GetDictParam(hv_EvalParams, "key_exists", "detailed_evaluation", &hv_KeyExists);
  if (0 != (HTuple(hv_KeyExists[0])))
  {
    GetDictTuple(hv_EvalParams, "detailed_evaluation", &hv_DetailedEvaluation);
  }
  if (0 != hv_DetailedEvaluation)
  {
    //We need a mapping from class IDs to class indices
    hv_ClassIDToClassIdx = HTuple((hv_ClassIDs.TupleMax())+1,-1);
    hv_ClassIDToClassIdx[hv_ClassIDs] = HTuple::TupleGenSequence(0,hv_NumClasses-1,1);
  }
  //
  //Check if the orientation is to be evaluated.
  hv_EvaluateOrientation = 0;
  if (0 != (HTuple(int(hv_InstanceType==HTuple("rectangle2"))).TupleAnd(HTuple(int((hv_Measures.TupleFind("soap"))!=-1)).TupleOr(int((hv_Measures.TupleFind("all"))!=-1)))))
  {
    hv_EvaluateOrientation = 1;
  }
  //
  //Go through samples.
  {
  HTuple end_val44 = (hv_Samples.TupleLength())-1;
  HTuple step_val44 = 1;
  for (hv_SIdx=0; hv_SIdx.Continue(end_val44, step_val44); hv_SIdx += step_val44)
  {
    //
    hv_CurrentSample = HTuple(hv_Samples[hv_SIdx]);
    hv_CurrentResult = HTuple(hv_Results[hv_SIdx]);
    //
    //Get classes.
    GetDictTuple(hv_CurrentSample, "bbox_label_id", &hv_GtClassIDs);
    GetDictTuple(hv_CurrentResult, "bbox_class_id", &hv_ResClassIDs);
    hv_NumGT = hv_GtClassIDs.TupleLength();
    hv_NumRes = hv_ResClassIDs.TupleLength();
    //
    //Get result confidences and sort them in descending order.
    GetDictTuple(hv_CurrentResult, "bbox_confidence", &hv_Confidences);
    hv_ResSortIndices = (-hv_Confidences).TupleSortIndex();
    hv_Confidences = HTuple(hv_Confidences[hv_ResSortIndices]);
    //Sort the result class IDs.
    hv_ResClassIDs = HTuple(hv_ResClassIDs[hv_ResSortIndices]);
    //
    //Compute the IoUs of the instances.
    area_iou(hv_CurrentSample, hv_CurrentResult, hv_InstanceType, hv_ResSortIndices, 
        &hv_GtAreas, &hv_ResAreas, &hv_IoUs);
    //
    if (0 != hv_EvaluateOrientation)
    {
      GetDictTuple(hv_CurrentSample, "bbox_phi", &hv_GtPhis);
      GetDictTuple(hv_CurrentResult, "bbox_phi", &hv_ResPhis);
      hv_ResPhis = HTuple(hv_ResPhis[hv_ResSortIndices]);
    }
    //Loop over the maximal number of detections.
    {
    HTuple end_val71 = (hv_MaxNumDetections.TupleLength())-1;
    HTuple step_val71 = 1;
    for (hv_MDIdx=0; hv_MDIdx.Continue(end_val71, step_val71); hv_MDIdx += step_val71)
    {
      hv_MaxNum = HTuple(hv_MaxNumDetections[hv_MDIdx]);
      hv_MaxNumStr = ""+hv_MaxNum;
      if (0 != (int(hv_MaxNum==-1)))
      {
        hv_MaxNumStr = "all";
      }
      GetDictTuple(hv_RunningMeasures, "max_num_detections_"+hv_MaxNumStr, &hv_CurrentRunningMeasures);
      //
      //Loop over the area ranges.
      {
      HTuple end_val80 = (hv_AreaNames.TupleLength())-1;
      HTuple step_val80 = 1;
      for (hv_AreaIdx=0; hv_AreaIdx.Continue(end_val80, step_val80); hv_AreaIdx += step_val80)
      {
        //
        //Get information about the current area range.
        hv_MinArea = HTuple(hv_MinAreas[hv_AreaIdx]);
        hv_MaxArea = HTuple(hv_MaxAreas[hv_AreaIdx]);
        hv_AreaName = HTuple(hv_AreaNames[hv_AreaIdx]);
        //
        GetDictTuple(hv_CurrentRunningMeasures, "area_"+hv_AreaName, &hv_AreaRunningMeasures);
        //
        //Set ignore-flag for ground truth instances.
        hv_GtIgnore = HTuple(hv_NumGT,0);
        //
        //Ignore ground truth instances with area outside the area range.
        if (0 != (int(hv_NumGT>0)))
        {
          TupleFind((hv_GtAreas.TupleLessElem(hv_MinArea)).TupleOr(hv_GtAreas.TupleGreaterElem(hv_MaxArea)), 
              1, &hv_GtIgnoreInds);
          if (0 != (int(hv_GtIgnoreInds>-1)))
          {
            hv_GtIgnore[hv_GtIgnoreInds] = 1;
          }
        }
        //
        GetDictTuple(hv_AreaRunningMeasures, "num_gt", &hv_PerClassNumGt);
        GetDictTuple(hv_AreaRunningMeasures, "num_pred", &hv_PerClassNumPred);
        GetDictTuple(hv_AreaRunningMeasures, "confidence", &hv_PerClassConfidences);
        GetDictTuple(hv_AreaRunningMeasures, "num_gt_ignore", &hv_PerClassNumGtIgnore);
        //
        if (0 != hv_DetailedEvaluation)
        {
          //Store if a sample has at least one false positive or false negative (for each IoU threshold).
          hv_SampleHasFP = HTuple(hv_IoUThresholds.TupleLength(),0);
          hv_SampleHasFN = HTuple(hv_IoUThresholds.TupleLength(),0);
        }
        //
        //Loop over the classes.
        {
        HTuple end_val112 = hv_NumClasses-1;
        HTuple step_val112 = 1;
        for (hv_ClsIdx=0; hv_ClsIdx.Continue(end_val112, step_val112); hv_ClsIdx += step_val112)
        {
          hv_CurrentClassID = HTuple(hv_ClassIDs[hv_ClsIdx]);
          //
          //Get the ground truth for this class.
          hv_CurrentGtIdxs = hv_GtClassIDs.TupleFind(hv_CurrentClassID);
          if (0 != (int(hv_CurrentGtIdxs==-1)))
          {
            hv_CurrentGtIdxs = HTuple();
          }
          hv_CurrentNumGt = hv_CurrentGtIdxs.TupleLength();
          //
          //Get ground truth ignore for this class.
          hv_CurrentGtIgnore = HTuple(hv_GtIgnore[hv_CurrentGtIdxs]);
          if (0 != (int((hv_CurrentGtIgnore.TupleLength())==0)))
          {
            hv_CurrentNumGtIgnore = 0;
          }
          else
          {
            hv_CurrentNumGtIgnore = hv_CurrentGtIgnore.TupleSum();
          }
          //
          //Number of gt for this class and without ignore.
          hv_CurrentNumGtNoIgnore = hv_CurrentNumGt-hv_CurrentNumGtIgnore;
          //
          //Get results for this class.
          hv_CurrentResIdxs = hv_ResClassIDs.TupleFind(hv_CurrentClassID);
          if (0 != (int(hv_CurrentResIdxs==-1)))
          {
            hv_CurrentResIdxs = HTuple();
          }
          hv_CurrentNumRes = hv_MaxNum.TupleMin2(hv_CurrentResIdxs.TupleLength());
          //MaxNum -1 corresponds to taking all results.
          if (0 != (int(hv_MaxNum==-1)))
          {
            hv_CurrentNumRes = hv_CurrentResIdxs.TupleLength();
          }
          hv_CurrentResIdxs = hv_CurrentResIdxs.TupleSelectRange(0,hv_CurrentNumRes-1);
          //
          //Get areas of the current results.
          hv_CurrentResAreas = HTuple(hv_ResAreas[hv_CurrentResIdxs]);
          //
          //Update the confidences, num_gt and num_pred for this class.
          hv_OldNumPred = HTuple(hv_PerClassNumPred[hv_ClsIdx]);
          hv_PerClassNumGt[hv_ClsIdx] = HTuple(hv_PerClassNumGt[hv_ClsIdx])+hv_CurrentNumGt;
          hv_PerClassNumGtIgnore[hv_ClsIdx] = HTuple(hv_PerClassNumGtIgnore[hv_ClsIdx])+hv_CurrentNumGtIgnore;
          hv_PerClassNumPred[hv_ClsIdx] = HTuple(hv_PerClassNumPred[hv_ClsIdx])+hv_CurrentNumRes;
          GetDictTuple(hv_PerClassConfidences, "class_"+HTuple(hv_ClassIDs[hv_ClsIdx]), 
              &hv_CurrentClassConfidences);
          //Confidences are allocated in blocks of AllocationBlockLength. Therefore, we have to check
          //if the allocated block is long enough, otherwise allocate a new block.
          if (0 != (int(HTuple(hv_PerClassNumPred[hv_ClsIdx])>(hv_CurrentClassConfidences.TupleLength()))))
          {
            hv_CurrentClassConfidences = hv_CurrentClassConfidences.TupleConcat(HTuple(hv_AllocationBlockLength,-1));
          }
          hv_CurrentClassConfidences[HTuple::TupleGenSequence(hv_OldNumPred,HTuple(hv_PerClassNumPred[hv_ClsIdx])-1,1)] = HTuple(hv_Confidences[hv_CurrentResIdxs]);
          SetDictTuple(hv_PerClassConfidences, "class_"+HTuple(hv_ClassIDs[hv_ClsIdx]), 
              hv_CurrentClassConfidences);
          //
          //Sort the ground truth: Non-ignored instances first.
          hv_GtSortIdx = hv_CurrentGtIgnore.TupleSortIndex();
          hv_CurrentGtIgnore = HTuple(hv_CurrentGtIgnore[hv_GtSortIdx]);
          hv_CurrentGtIdxs = HTuple(hv_CurrentGtIdxs[hv_GtSortIdx]);
          //
          //Get orientations of result and ground truth instances.
          if (0 != hv_EvaluateOrientation)
          {
            hv_CurrentResPhis = HTuple(hv_ResPhis[hv_CurrentResIdxs]);
            hv_CurrentGtPhis = HTuple(hv_GtPhis[hv_CurrentGtIdxs]);
          }
          //
          if (0 != (int(hv_CurrentNumRes>0)))
          {
            //Loop over IoU thresholds.
            {
            HTuple end_val175 = (hv_IoUThresholds.TupleLength())-1;
            HTuple step_val175 = 1;
            for (hv_ITIdx=0; hv_ITIdx.Continue(end_val175, step_val175); hv_ITIdx += step_val175)
            {
              //We check which ground truth and
              //result instance can be matched.
              hv_GtMatched = HTuple(hv_CurrentNumGt,0);
              hv_ResMatched = HTuple(hv_CurrentNumRes,0);
              //
              if (0 != hv_EvaluateOrientation)
              {
                //Initialize the absolute orientation difference to -1.
                hv_ResAbsOrientationDiff = HTuple(hv_CurrentNumRes,-1);
              }
              //Store which detections should be ignored.
              hv_ResIgnore = HTuple(hv_CurrentNumRes,0);
              {
              HTuple end_val187 = hv_CurrentNumRes-1;
              HTuple step_val187 = 1;
              for (hv_ResIdx=0; hv_ResIdx.Continue(end_val187, step_val187); hv_ResIdx += step_val187)
              {
                //Set the currently best achieved IoU to the IoU threshold and
                //initialize the matching index.
                hv_CurrentIoU = HTuple(hv_IoUThresholds[hv_ITIdx]).TupleMin2(1-1.0e-10);
                hv_MatchIdx = -1;
                //Loop over ground truth.
                {
                HTuple end_val193 = hv_CurrentNumGt-1;
                HTuple step_val193 = 1;
                for (hv_GtIdx=0; hv_GtIdx.Continue(end_val193, step_val193); hv_GtIdx += step_val193)
                {
                  //Continue if this ground truth has already been matched.
                  if (0 != (HTuple(hv_GtMatched[hv_GtIdx])))
                  {
                    continue;
                  }
                  //Stop if matched with non-ignored ground truth and current ground truth is on ignore.
                  if (0 != (int(hv_MatchIdx>-1)))
                  {
                    if (0 != (HTuple(int(HTuple(hv_CurrentGtIgnore[hv_MatchIdx])==0)).TupleAnd(int(HTuple(hv_CurrentGtIgnore[hv_GtIdx])==1))))
                    {
                      break;
                    }
                  }
                  //Continue if IoU is not better than a previous match.
                  if (0 != (int(HTuple(hv_IoUs[(HTuple(hv_CurrentGtIdxs[hv_GtIdx])*hv_NumRes)+HTuple(hv_CurrentResIdxs[hv_ResIdx])])<hv_CurrentIoU)))
                  {
                    continue;
                  }
                  //We got a new best match, store it.
                  hv_CurrentIoU = HTuple(hv_IoUs[(HTuple(hv_CurrentGtIdxs[hv_GtIdx])*hv_NumRes)+HTuple(hv_CurrentResIdxs[hv_ResIdx])]);
                  hv_MatchIdx = hv_GtIdx;
                }
                }
                //If a match has been made we store it for both ground truth and result.
                if (0 != (int(hv_MatchIdx!=-1)))
                {
                  //In COCO they use the IDs of GT and Res, we just use 1
                  //to indicate the matching, but don't store which one has been matched.
                  hv_ResMatched[hv_ResIdx] = 1;
                  hv_GtMatched[hv_MatchIdx] = 1;
                  hv_ResIgnore[hv_ResIdx] = HTuple(hv_CurrentGtIgnore[hv_MatchIdx]);
                  //
                  if (0 != hv_EvaluateOrientation)
                  {
                    //Set the absolute orientation difference.
                    hv_ResAbsOrientationDiff[hv_ResIdx] = (HTuple(hv_CurrentResPhis[hv_ResIdx])-HTuple(hv_CurrentGtPhis[hv_MatchIdx])).TupleAbs();
                    if (0 != (int(HTuple(hv_ResAbsOrientationDiff[hv_ResIdx])>(HTuple(180).TupleRad()))))
                    {
                      hv_ResAbsOrientationDiff[hv_ResIdx] = (HTuple(360).TupleRad())-HTuple(hv_ResAbsOrientationDiff[hv_ResIdx]);
                    }
                  }
                }
              }
              }
              //Ignore the unmatched results that are outside of the current area range.
              hv_AreaIgnore = (hv_CurrentResAreas.TupleLessElem(hv_MinArea)).TupleOr(hv_CurrentResAreas.TupleGreaterElem(hv_MaxArea));
              hv_ResIgnore = HTuple((hv_ResMatched.TupleEqualElem(-1)).TupleAnd(hv_AreaIgnore.TupleEqualElem(1))).TupleOr(hv_ResIgnore);
              //True positives are the matched results.
              GetDictTuple(hv_AreaRunningMeasures, "iou_"+((""+HTuple(hv_IoUThresholds[hv_ITIdx])).TupleRegexpReplace("\\.","")), 
                  &hv_PerIoUMeasure);
              GetDictTuple(hv_PerIoUMeasure, "class_"+HTuple(hv_ClassIDs[hv_ClsIdx]), 
                  &hv_PerClassMeasures);
              GetDictTuple(hv_PerClassMeasures, "is_tp", &hv_CurrentIsTP);
              //As for confidences, check if we have to allocate a new block.
              if (0 != (int(HTuple(hv_PerClassNumPred[hv_ClsIdx])>(hv_CurrentIsTP.TupleLength()))))
              {
                hv_CurrentIsTP = hv_CurrentIsTP.TupleConcat(HTuple(hv_AllocationBlockLength,-1));
              }
              hv_CurrentIsTP[HTuple::TupleGenSequence(hv_OldNumPred,HTuple(hv_PerClassNumPred[hv_ClsIdx])-1,1)] = hv_ResMatched;
              SetDictTuple(hv_PerClassMeasures, "is_tp", hv_CurrentIsTP);
              //Set the ignored results.
              GetDictTuple(hv_PerClassMeasures, "ignore", &hv_CurrentIgnore);
              if (0 != (int(HTuple(hv_PerClassNumPred[hv_ClsIdx])>(hv_CurrentIgnore.TupleLength()))))
              {
                hv_CurrentIgnore = hv_CurrentIgnore.TupleConcat(HTuple(hv_AllocationBlockLength,-1));
              }
              hv_CurrentIgnore[HTuple::TupleGenSequence(hv_OldNumPred,HTuple(hv_PerClassNumPred[hv_ClsIdx])-1,1)] = hv_ResIgnore;
              SetDictTuple(hv_PerClassMeasures, "ignore", hv_CurrentIgnore);
              //Set the absolute orientation difference.
              if (0 != hv_EvaluateOrientation)
              {
                GetDictTuple(hv_PerClassMeasures, "abs_orientation_diff", &hv_CurrentAbsOrientationDiff);
                if (0 != (int(HTuple(hv_PerClassNumPred[hv_ClsIdx])>(hv_CurrentAbsOrientationDiff.TupleLength()))))
                {
                  hv_CurrentAbsOrientationDiff = hv_CurrentAbsOrientationDiff.TupleConcat(HTuple(hv_AllocationBlockLength,-1));
                }
                hv_CurrentAbsOrientationDiff[HTuple::TupleGenSequence(hv_OldNumPred,HTuple(hv_PerClassNumPred[hv_ClsIdx])-1,1)] = hv_ResAbsOrientationDiff;
                SetDictTuple(hv_PerClassMeasures, "abs_orientation_diff", hv_CurrentAbsOrientationDiff);
              }
              //
              //Beginning of detailed evaluation processing (optional).
              //
              if (0 != hv_DetailedEvaluation)
              {
                //Check if there have been false negatives.
                if (0 != (int(hv_CurrentNumGtNoIgnore>0)))
                {
                  hv_GtMatchedNoIgnore = hv_GtMatched.TupleAnd(hv_CurrentGtIgnore.TupleNot());
                  if (0 != (int((hv_GtMatchedNoIgnore.TupleSum())<hv_CurrentNumGtNoIgnore)))
                  {
                    hv_SampleHasFN[hv_ITIdx] = 1;
                  }
                }
                //
                //Initialize the detailed running measures.
                hv_ResIsFPClass = HTuple(hv_CurrentNumRes,-1);
                hv_ResIsFPBackground = HTuple(hv_CurrentNumRes,0);
                hv_ResIsFPLocalization = HTuple(hv_CurrentNumRes,0);
                hv_ResIsFPDuplicate = HTuple(hv_CurrentNumRes,0);
                hv_ResIsFPMultiple = HTuple(hv_CurrentNumRes,0);
                //
                //Initialize detailed running measures for orientation difference.
                if (0 != hv_EvaluateOrientation)
                {
                  hv_ResAbsOrientationDiffClass = HTuple(hv_CurrentNumRes,-1);
                  hv_ResAbsOrientationDiffLocalization = HTuple(hv_CurrentNumRes,-1);
                  hv_ResAbsOrientationDiffDuplicate = HTuple(hv_CurrentNumRes,-1);
                  hv_ResAbsOrientationDiffMultiple = HTuple(hv_CurrentNumRes,-1);
                }
                //Check if there have been false positives.
                if (0 != (int((hv_ResMatched.TupleSum())<hv_CurrentNumRes)))
                {
                  hv_SampleHasFP[hv_ITIdx] = 1;
                  //
                  //For each false positive, find out what was the reason for being false positive:
                  hv_FPResIdxsThisClass = hv_ResMatched.TupleFind(0);
                  hv_FPResIdxsAllResults = HTuple(hv_CurrentResIdxs[hv_FPResIdxsThisClass]);
                  hv_GTIdxsNotToIgnore = hv_GtIgnore.TupleFind(0);
                  {
                  HTuple end_val292 = (hv_FPResIdxsThisClass.TupleLength())-1;
                  HTuple step_val292 = 1;
                  for (hv_ResIdx=0; hv_ResIdx.Continue(end_val292, step_val292); hv_ResIdx += step_val292)
                  {
                    if (0 != (HTuple(hv_ResIgnore[HTuple(hv_FPResIdxsThisClass[hv_ResIdx])]).TupleNot()))
                    {
                      if (0 != (HTuple(int((hv_GTIdxsNotToIgnore.TupleLength())==0)).TupleOr(int(hv_GTIdxsNotToIgnore==-1))))
                      {
                        //No GT instances or all GT instances are ignored.
                        //Thus, any detection is a background detection
                        hv_MaxIoU = 0.0;
                      }
                      else
                      {
                        //We have GT instances to consider.
                        hv_IoUsWithGT = HTuple(hv_IoUs[(hv_GTIdxsNotToIgnore*hv_NumRes)+HTuple(hv_FPResIdxsAllResults[hv_ResIdx])]);
                        hv_MaxIoU = hv_IoUsWithGT.TupleMax();
                        //It is enough to look for the first occurrence because the IoUs to ground truth should be different.
                        hv_MaxIdx = hv_IoUsWithGT.TupleFindFirst(hv_MaxIoU);
                        hv_GTClassIDMaxIoU = HTuple(hv_GtClassIDs[HTuple(hv_GTIdxsNotToIgnore[hv_MaxIdx])]);
                      }
                      if (0 != (hv_EvaluateOrientation.TupleAnd(int(hv_MaxIoU>0.0))))
                      {
                        //Calculate the absolute orientation difference to the GT instance with maximal IoU.
                        hv_AbsOrientationDiff = (HTuple(hv_ResPhis[HTuple(hv_FPResIdxsAllResults[hv_ResIdx])])-HTuple(hv_GtPhis[HTuple(hv_GTIdxsNotToIgnore[hv_MaxIdx])])).TupleAbs();
                        if (0 != (int(hv_AbsOrientationDiff>(HTuple(180).TupleRad()))))
                        {
                          hv_AbsOrientationDiff = (HTuple(360).TupleRad())-hv_AbsOrientationDiff;
                        }
                      }
                      //Determine false positive type.
                      if (0 != (int(hv_MaxIoU==0.0)))
                      {
                        //Background detection. This detection does not overlap to any ground truth (that is not ignored).
                        hv_ResIsFPBackground[HTuple(hv_FPResIdxsThisClass[hv_ResIdx])] = 1;
                      }
                      else if (0 != (HTuple(int(hv_MaxIoU>=HTuple(hv_IoUThresholds[hv_ITIdx]))).TupleAnd(int(hv_CurrentClassID!=hv_GTClassIDMaxIoU))))
                      {
                        //False class.
                        //Note that this does not necessarily mean that this detection
                        //would be a true positive if the class was changed. It could still be a duplicate.
                        hv_ResIsFPClass[HTuple(hv_FPResIdxsThisClass[hv_ResIdx])] = HTuple(hv_ClassIDToClassIdx[hv_GTClassIDMaxIoU]);
                        //Store the absolute orientation difference.
                        if (0 != hv_EvaluateOrientation)
                        {
                          hv_ResAbsOrientationDiffClass[HTuple(hv_FPResIdxsThisClass[hv_ResIdx])] = hv_AbsOrientationDiff;
                        }
                      }
                      else if (0 != (HTuple(int(hv_MaxIoU>=HTuple(hv_IoUThresholds[hv_ITIdx]))).TupleAnd(int(hv_CurrentClassID==hv_GTClassIDMaxIoU))))
                      {
                        //Duplicate detection. There must exist another detection with a higher confidence with the same ground truth.
                        hv_ResIsFPDuplicate[HTuple(hv_FPResIdxsThisClass[hv_ResIdx])] = 1;
                        //Store the absolute orientation difference.
                        if (0 != hv_EvaluateOrientation)
                        {
                          hv_ResAbsOrientationDiffDuplicate[HTuple(hv_FPResIdxsThisClass[hv_ResIdx])] = hv_AbsOrientationDiff;
                        }
                      }
                      else if (0 != (HTuple(int(hv_MaxIoU<HTuple(hv_IoUThresholds[hv_ITIdx]))).TupleAnd(int(hv_CurrentClassID==hv_GTClassIDMaxIoU))))
                      {
                        //Bad localization. Class is correct, but the IoU is too low.
                        hv_ResIsFPLocalization[HTuple(hv_FPResIdxsThisClass[hv_ResIdx])] = 1;
                        //Store the absolute orientation difference.
                        if (0 != hv_EvaluateOrientation)
                        {
                          hv_ResAbsOrientationDiffLocalization[HTuple(hv_FPResIdxsThisClass[hv_ResIdx])] = hv_AbsOrientationDiff;
                        }
                      }
                      else if (0 != (HTuple(int(hv_MaxIoU<HTuple(hv_IoUThresholds[hv_ITIdx]))).TupleAnd(int(hv_CurrentClassID!=hv_GTClassIDMaxIoU))))
                      {
                        //Wrong class and bad localization.
                        hv_ResIsFPMultiple[HTuple(hv_FPResIdxsThisClass[hv_ResIdx])] = 1;
                        //Store the absolute orientation difference.
                        if (0 != hv_EvaluateOrientation)
                        {
                          hv_ResAbsOrientationDiffMultiple[HTuple(hv_FPResIdxsThisClass[hv_ResIdx])] = hv_AbsOrientationDiff;
                        }
                      }
                      else
                      {
                        //This case should never occur.
                        throw HException("Fatal error during detailed evaluation.");
                      }
                    }
                  }
                  }
                }
                //
                //Overwrite the detailed running measures.
                GetDictTuple(hv_PerClassMeasures, "is_fp_class", &hv_IsFPClass);
                GetDictTuple(hv_PerClassMeasures, "is_fp_background", &hv_IsFPBackground);
                GetDictTuple(hv_PerClassMeasures, "is_fp_localization", &hv_IsFPLocalization);
                GetDictTuple(hv_PerClassMeasures, "is_fp_duplicate", &hv_IsFPDuplicate);
                GetDictTuple(hv_PerClassMeasures, "is_fp_multiple", &hv_IsFPMultiple);
                if (0 != hv_EvaluateOrientation)
                {
                  GetDictTuple(hv_PerClassMeasures, "abs_orientation_diff_multiple", 
                      &hv_AbsOrientationDiffMultiple);
                  GetDictTuple(hv_PerClassMeasures, "abs_orientation_diff_duplicate", 
                      &hv_AbsOrientationDiffDuplicate);
                  GetDictTuple(hv_PerClassMeasures, "abs_orientation_diff_localization", 
                      &hv_AbsOrientationDiffLocalization);
                  GetDictTuple(hv_PerClassMeasures, "abs_orientation_diff_class", 
                      &hv_AbsOrientationDiffClass);
                }
                //Allocate new blocks if necessary (all have the same length).
                if (0 != (int(HTuple(hv_PerClassNumPred[hv_ClsIdx])>(hv_IsFPClass.TupleLength()))))
                {
                  hv_IsFPClass = hv_IsFPClass.TupleConcat(HTuple(hv_AllocationBlockLength,-1));
                  hv_IsFPBackground = hv_IsFPBackground.TupleConcat(HTuple(hv_AllocationBlockLength,-1));
                  hv_IsFPLocalization = hv_IsFPLocalization.TupleConcat(HTuple(hv_AllocationBlockLength,-1));
                  hv_IsFPDuplicate = hv_IsFPDuplicate.TupleConcat(HTuple(hv_AllocationBlockLength,-1));
                  hv_IsFPMultiple = hv_IsFPMultiple.TupleConcat(HTuple(hv_AllocationBlockLength,-1));
                  if (0 != hv_EvaluateOrientation)
                  {
                    hv_AbsOrientationDiffMultiple = hv_AbsOrientationDiffMultiple.TupleConcat(HTuple(hv_AllocationBlockLength,-1));
                    hv_AbsOrientationDiffDuplicate = hv_AbsOrientationDiffDuplicate.TupleConcat(HTuple(hv_AllocationBlockLength,-1));
                    hv_AbsOrientationDiffLocalization = hv_AbsOrientationDiffLocalization.TupleConcat(HTuple(hv_AllocationBlockLength,-1));
                    hv_AbsOrientationDiffClass = hv_AbsOrientationDiffClass.TupleConcat(HTuple(hv_AllocationBlockLength,-1));
                  }
                }
                hv_IsFPClass[HTuple::TupleGenSequence(hv_OldNumPred,HTuple(hv_PerClassNumPred[hv_ClsIdx])-1,1)] = hv_ResIsFPClass;
                hv_IsFPBackground[HTuple::TupleGenSequence(hv_OldNumPred,HTuple(hv_PerClassNumPred[hv_ClsIdx])-1,1)] = hv_ResIsFPBackground;
                hv_IsFPLocalization[HTuple::TupleGenSequence(hv_OldNumPred,HTuple(hv_PerClassNumPred[hv_ClsIdx])-1,1)] = hv_ResIsFPLocalization;
                hv_IsFPDuplicate[HTuple::TupleGenSequence(hv_OldNumPred,HTuple(hv_PerClassNumPred[hv_ClsIdx])-1,1)] = hv_ResIsFPDuplicate;
                hv_IsFPMultiple[HTuple::TupleGenSequence(hv_OldNumPred,HTuple(hv_PerClassNumPred[hv_ClsIdx])-1,1)] = hv_ResIsFPMultiple;
                if (0 != hv_EvaluateOrientation)
                {
                  hv_AbsOrientationDiffMultiple[HTuple::TupleGenSequence(hv_OldNumPred,HTuple(hv_PerClassNumPred[hv_ClsIdx])-1,1)] = hv_ResAbsOrientationDiffMultiple;
                  hv_AbsOrientationDiffDuplicate[HTuple::TupleGenSequence(hv_OldNumPred,HTuple(hv_PerClassNumPred[hv_ClsIdx])-1,1)] = hv_ResAbsOrientationDiffDuplicate;
                  hv_AbsOrientationDiffLocalization[HTuple::TupleGenSequence(hv_OldNumPred,HTuple(hv_PerClassNumPred[hv_ClsIdx])-1,1)] = hv_ResAbsOrientationDiffLocalization;
                  hv_AbsOrientationDiffClass[HTuple::TupleGenSequence(hv_OldNumPred,HTuple(hv_PerClassNumPred[hv_ClsIdx])-1,1)] = hv_ResAbsOrientationDiffClass;
                }
                SetDictTuple(hv_PerClassMeasures, "is_fp_class", hv_IsFPClass);
                SetDictTuple(hv_PerClassMeasures, "is_fp_background", hv_IsFPBackground);
                SetDictTuple(hv_PerClassMeasures, "is_fp_localization", hv_IsFPLocalization);
                SetDictTuple(hv_PerClassMeasures, "is_fp_duplicate", hv_IsFPDuplicate);
                SetDictTuple(hv_PerClassMeasures, "is_fp_multiple", hv_IsFPMultiple);
                if (0 != hv_EvaluateOrientation)
                {
                  SetDictTuple(hv_PerClassMeasures, "abs_orientation_diff_multiple", 
                      hv_AbsOrientationDiffMultiple);
                  SetDictTuple(hv_PerClassMeasures, "abs_orientation_diff_duplicate", 
                      hv_AbsOrientationDiffDuplicate);
                  SetDictTuple(hv_PerClassMeasures, "abs_orientation_diff_localization", 
                      hv_AbsOrientationDiffLocalization);
                  SetDictTuple(hv_PerClassMeasures, "abs_orientation_diff_class", 
                      hv_AbsOrientationDiffClass);
                }
              }
              //
              //End of detailed evaluation processing.
              //
            }
            }
          }
          else
          {
            if (0 != (hv_DetailedEvaluation.TupleAnd(int(hv_CurrentNumGtNoIgnore>0))))
            {
              //There are false negatives for this class.
              //Loop over IoU thresholds.
              {
              HTuple end_val412 = (hv_IoUThresholds.TupleLength())-1;
              HTuple step_val412 = 1;
              for (hv_ITIdx=0; hv_ITIdx.Continue(end_val412, step_val412); hv_ITIdx += step_val412)
              {
                hv_SampleHasFN[hv_ITIdx] = 1;
              }
              }
            }
          }
        }
        }
        //Update the confidences, num_gt and num_pred.
        SetDictTuple(hv_AreaRunningMeasures, "num_gt", hv_PerClassNumGt);
        SetDictTuple(hv_AreaRunningMeasures, "num_pred", hv_PerClassNumPred);
        SetDictTuple(hv_AreaRunningMeasures, "confidence", hv_PerClassConfidences);
        SetDictTuple(hv_AreaRunningMeasures, "num_gt_ignore", hv_PerClassNumGtIgnore);
        //
        if (0 != hv_DetailedEvaluation)
        {
          //Set values that are calculated over all classes (for each IoU threshold).
          {
          HTuple end_val426 = (hv_IoUThresholds.TupleLength())-1;
          HTuple step_val426 = 1;
          for (hv_ITIdx=0; hv_ITIdx.Continue(end_val426, step_val426); hv_ITIdx += step_val426)
          {
            GetDictTuple(hv_AreaRunningMeasures, "iou_"+((""+HTuple(hv_IoUThresholds[hv_ITIdx])).TupleRegexpReplace("\\.","")), 
                &hv_PerIoUMeasure);
            //Set image IDs with false negatives
            if (0 != (HTuple(hv_SampleHasFN[hv_ITIdx])))
            {
              GetDictTuple(hv_CurrentSample, "image_id", &hv_CurrentImageID);
              GetDictTuple(hv_PerIoUMeasure, "image_ids_with_false_negatives", &hv_ImageIDsWithFN);
              GetDictTuple(hv_PerIoUMeasure, "num_image_ids_with_false_negatives", 
                  &hv_NumImageIDsWithFN);
              //Allocate a new block if necessary.
              if (0 != (int((hv_NumImageIDsWithFN+1)>(hv_ImageIDsWithFN.TupleLength()))))
              {
                hv_ImageIDsWithFN = hv_ImageIDsWithFN.TupleConcat(HTuple(hv_AllocationBlockLength,-1));
              }
              hv_ImageIDsWithFN[hv_NumImageIDsWithFN] = hv_CurrentImageID;
              SetDictTuple(hv_PerIoUMeasure, "image_ids_with_false_negatives", hv_ImageIDsWithFN);
              SetDictTuple(hv_PerIoUMeasure, "num_image_ids_with_false_negatives", 
                  hv_NumImageIDsWithFN+1);
            }
            if (0 != (HTuple(hv_SampleHasFP[hv_ITIdx])))
            {
              GetDictTuple(hv_CurrentSample, "image_id", &hv_CurrentImageID);
              GetDictTuple(hv_PerIoUMeasure, "image_ids_with_false_positives", &hv_ImageIDsWithFP);
              GetDictTuple(hv_PerIoUMeasure, "num_image_ids_with_false_positives", 
                  &hv_NumImageIDsWithFP);
              //Allocate a new block if necessary.
              if (0 != (int((hv_NumImageIDsWithFP+1)>(hv_ImageIDsWithFP.TupleLength()))))
              {
                hv_ImageIDsWithFP = hv_ImageIDsWithFP.TupleConcat(HTuple(hv_AllocationBlockLength,-1));
              }
              hv_ImageIDsWithFP[hv_NumImageIDsWithFP] = hv_CurrentImageID;
              SetDictTuple(hv_PerIoUMeasure, "image_ids_with_false_positives", hv_ImageIDsWithFP);
              SetDictTuple(hv_PerIoUMeasure, "num_image_ids_with_false_positives", 
                  hv_NumImageIDsWithFP+1);
            }
          }
          }
        }
      }
      }
      SetDictTuple(hv_CurrentRunningMeasures, "area_"+hv_AreaName, hv_AreaRunningMeasures);
    }
    }
  }
  }
  //
  return;
}

// Chapter: Deep Learning / Evaluation
// Short Description: Update running measures for a pixel-based evaluation. 
void update_running_pixel_measures (HTuple hv_Samples, HTuple hv_Results, HTuple hv_EvalParams, 
    HTuple hv_RunningMeasures)
{

  // Local iconic variables
  HObject  ho_Annot, ho_Result, ho_ClsIgnore, ho_ClsIgnoreTmp;
  HObject  ho_ClsAnnot, ho_ClsResult, ho_TPReg, ho_FPReg, ho_FNReg;

  // Local control variables
  HTuple  hv_ClassIDs, hv_IgnoreClassIDs, hv_Measures;
  HTuple  hv_CalcConfMatrix, hv_NumClasses, hv_MaxId, hv_ConfMatrix;
  HTuple  hv_MapClassIDs, hv_ClsIdToClsIdx, hv_TP, hv_FP;
  HTuple  hv_FN, hv_SampleIndex, hv_Rows, hv_Columns, hv_AnnotVals;
  HTuple  hv_ResultVals, hv_ConfTuple, hv_ConfHist, hv_BinSize;
  HTuple  hv_ConfMatrixTmp, hv_IgnoreIndex, hv_ClsIndex, hv_ClsId;
  HTuple  hv_ClsTP, hv_ClsFP, hv_ClsFN;

  //
  //This procedure updates the RunningMeasures
  //for a pixel-based evaluation for segmentation.
  //These measures are stored in the dictionary RunningMeasures and
  //updated by incorporating the Results the model obtained for the Samples.
  //
  //Get the class IDs.
  GetDictTuple(hv_EvalParams, "class_ids", &hv_ClassIDs);
  //Get the ignore class IDs.
  GetDictTuple(hv_EvalParams, "ignore_class_ids", &hv_IgnoreClassIDs);
  //Check if we need to compute/update the confusion matrix.
  GetDictTuple(hv_EvalParams, "measures", &hv_Measures);
  hv_CalcConfMatrix = HTuple(int((hv_Measures.TupleFind("pixel_confusion_matrix"))>-1)).TupleOr(int((hv_Measures.TupleFind("all"))>-1));
  //Get the number of classes.
  GetDictTuple(hv_EvalParams, "num_classes", &hv_NumClasses);
  //Get the maximum class ID plus a possible ignore-class.
  hv_MaxId = (hv_ClassIDs.TupleMax())+(int((hv_IgnoreClassIDs.TupleLength())>0));
  //
  if (0 != hv_CalcConfMatrix)
  {
    //Get the current confusion matrix.
    GetDictTuple(hv_RunningMeasures, "pixel_confusion_matrix", &hv_ConfMatrix);
    //Check if we need to map the class IDs.
    GetDictParam(hv_EvalParams, "key_exists", "class_id_mapping", &hv_MapClassIDs);
    if (0 != hv_MapClassIDs)
    {
      GetDictTuple(hv_EvalParams, "class_id_mapping", &hv_ClsIdToClsIdx);
      hv_NumClasses = (hv_ClsIdToClsIdx.TupleMax())+1;
    }
  }
  else
  {
    //Get the tuples for TP/FP/FN
    GetDictTuple(hv_RunningMeasures, "tp", &hv_TP);
    GetDictTuple(hv_RunningMeasures, "fp", &hv_FP);
    GetDictTuple(hv_RunningMeasures, "fn", &hv_FN);
  }
  //
  //Loop over images, i.e. sample dicts.
  {
  HTuple end_val35 = (hv_Samples.TupleLength())-1;
  HTuple step_val35 = 1;
  for (hv_SampleIndex=0; hv_SampleIndex.Continue(end_val35, step_val35); hv_SampleIndex += step_val35)
  {
    //Get annotation.
    GetDictObject(&ho_Annot, HTuple(hv_Samples[hv_SampleIndex]), "segmentation_image");
    //Get result.
    GetDictObject(&ho_Result, HTuple(hv_Results[hv_SampleIndex]), "segmentation_image");
    //
    //Update the measures.
    if (0 != hv_CalcConfMatrix)
    {
      //Get the ground truth and predicted class IDs of all pixels.
      GetRegionPoints(ho_Annot, &hv_Rows, &hv_Columns);
      GetGrayval(ho_Annot, hv_Rows, hv_Columns, &hv_AnnotVals);
      GetGrayval(ho_Result, hv_Rows, hv_Columns, &hv_ResultVals);
      //Map the class IDs to class indices.
      if (0 != hv_MapClassIDs)
      {
        hv_AnnotVals = HTuple(hv_ClsIdToClsIdx[hv_AnnotVals]);
        hv_ResultVals = HTuple(hv_ClsIdToClsIdx[hv_ResultVals]);
      }
      //The ground truth and predicted IDs are accumulated
      //such that each confusion pair (class_i <-> class_j) gets a unique value.
      hv_ConfTuple = (hv_NumClasses*hv_AnnotVals)+hv_ResultVals;
      //Compute the histogram of this confusion tuple.
      TupleHistoRange(hv_ConfTuple, 0, (hv_NumClasses*hv_NumClasses)-1, hv_NumClasses*hv_NumClasses, 
          &hv_ConfHist, &hv_BinSize);
      CreateMatrix(hv_NumClasses, hv_NumClasses, hv_ConfHist, &hv_ConfMatrixTmp);
      TransposeMatrix(hv_ConfMatrixTmp, &hv_ConfMatrixTmp);
      AddMatrix(hv_ConfMatrix, hv_ConfMatrixTmp, &hv_ConfMatrix);
    }
    else
    {
      //Get the ignore region.
      GenEmptyRegion(&ho_ClsIgnore);
      {
      HTuple end_val63 = (hv_IgnoreClassIDs.TupleLength())-1;
      HTuple step_val63 = 1;
      for (hv_IgnoreIndex=0; hv_IgnoreIndex.Continue(end_val63, step_val63); hv_IgnoreIndex += step_val63)
      {
        Threshold(ho_Annot, &ho_ClsIgnoreTmp, HTuple(hv_IgnoreClassIDs[hv_IgnoreIndex]), 
            HTuple(hv_IgnoreClassIDs[hv_IgnoreIndex]));
        Union2(ho_ClsIgnore, ho_ClsIgnoreTmp, &ho_ClsIgnore);
      }
      }
      //
      //Go through model classes.
      {
      HTuple end_val69 = (hv_ClassIDs.TupleLength())-1;
      HTuple step_val69 = 1;
      for (hv_ClsIndex=0; hv_ClsIndex.Continue(end_val69, step_val69); hv_ClsIndex += step_val69)
      {
        hv_ClsId = HTuple(hv_ClassIDs[hv_ClsIndex]);
        //Get the annotated region for this class.
        Threshold(ho_Annot, &ho_ClsAnnot, hv_ClsId, hv_ClsId);
        //Get the result region for this class.
        Threshold(ho_Result, &ho_ClsResult, hv_ClsId, hv_ClsId);
        //The pixels in the ignore region should not be considered.
        Difference(ho_ClsResult, ho_ClsIgnore, &ho_ClsResult);
        //Get TP/FP/FN.
        Intersection(ho_ClsAnnot, ho_ClsResult, &ho_TPReg);
        Difference(ho_ClsResult, ho_ClsAnnot, &ho_FPReg);
        //We define false negatives as pixels that have been labeled as this class,
        //but not been correctly predicted.
        Difference(ho_ClsAnnot, ho_ClsResult, &ho_FNReg);
        //Get corresponding pixel numbers and update.
        RegionFeatures(ho_TPReg, "area", &hv_ClsTP);
        RegionFeatures(ho_FPReg, "area", &hv_ClsFP);
        RegionFeatures(ho_FNReg, "area", &hv_ClsFN);
        hv_TP[hv_ClsIndex] = HTuple(hv_TP[hv_ClsIndex])+hv_ClsTP;
        hv_FP[hv_ClsIndex] = HTuple(hv_FP[hv_ClsIndex])+hv_ClsFP;
        hv_FN[hv_ClsIndex] = HTuple(hv_FN[hv_ClsIndex])+hv_ClsFN;
      }
      }
    }
  }
  }
  //
  //Update running measures.
  if (0 != hv_CalcConfMatrix)
  {
    SetDictTuple(hv_RunningMeasures, "pixel_confusion_matrix", hv_ConfMatrix);
  }
  else
  {
    SetDictTuple(hv_RunningMeasures, "tp", hv_TP);
    SetDictTuple(hv_RunningMeasures, "fp", hv_FP);
    SetDictTuple(hv_RunningMeasures, "fn", hv_FN);
  }
  //
  return;
}

// Chapter: Deep Learning / Model
// Short Description: Update model parameters according to the change strategies. 
void update_train_dl_model_change_strategies (HTuple hv_DLModelHandle, HTuple hv_ChangeStrategyData, 
    HTuple hv_Epoch)
{

  // Local iconic variables

  // Local control variables
  HTuple  hv_Enabled, hv_ChangeStrategies, hv_SolverType;
  HTuple  hv_Index, hv_ChangeStrategy, hv_ModelParam, hv_Epochs;
  HTuple  hv_Values, hv_Initial, hv_Greater, hv_ValueIndex;
  HTuple  hv_Value, hv_InternalCurrentExists, hv_InternalCurrent;
  HTuple  hv_ScaleThreshold, hv_CurrentLR, hv_LRChangeRatio;
  HTuple  hv_PreviousMomentumExists, hv_CurrentMomentum, hv_AdaptedMomentum;
  HTuple  hv_AdaptedMomentumExists, hv_PreviousMomentum;

  //
  //This procedure updates all parameters according to the change strategies
  //with respect to the current iteration.
  //
  GetDictTuple(hv_ChangeStrategyData, "enabled", &hv_Enabled);
  if (0 != (hv_Enabled.TupleNot()))
  {
    return;
  }
  //
  GetDictTuple(hv_ChangeStrategyData, "strategies", &hv_ChangeStrategies);
  GetDlModelParam(hv_DLModelHandle, "solver_type", &hv_SolverType);
  //
  //Update the parameter of each strategy.
  {
  HTuple end_val13 = (hv_ChangeStrategies.TupleLength())-1;
  HTuple step_val13 = 1;
  for (hv_Index=0; hv_Index.Continue(end_val13, step_val13); hv_Index += step_val13)
  {
    hv_ChangeStrategy = HTuple(hv_ChangeStrategies[hv_Index]);
    GetDictTuple(hv_ChangeStrategy, "model_param", &hv_ModelParam);
    GetDictTuple(hv_ChangeStrategy, "epochs", &hv_Epochs);
    GetDictTuple(hv_ChangeStrategy, "values", &hv_Values);
    GetDictTuple(hv_ChangeStrategy, "initial_value", &hv_Initial);
    //Epochs defines at which epoch the change happens. Its sorting is enforced during initialization.
    hv_Greater = hv_Epoch.TupleGreaterEqualElem(hv_Epochs);
    TupleFindLast(hv_Greater, 1, &hv_ValueIndex);
    if (0 != (int(hv_ValueIndex==-1)))
    {
      hv_Value = hv_Initial;
    }
    else
    {
      hv_Value = HTuple(hv_Values[hv_ValueIndex]);
    }
    //Check current value and only make changes if the value changed.
    GetDictParam(hv_ChangeStrategy, "key_exists", "internal_current_value", &hv_InternalCurrentExists);
    if (0 != hv_InternalCurrentExists)
    {
      GetDictTuple(hv_ChangeStrategy, "internal_current_value", &hv_InternalCurrent);
    }
    else
    {
      GetDlModelParam(hv_DLModelHandle, hv_ModelParam, &hv_InternalCurrent);
    }
    //If the current value differs from the new value we change it.
    if (0 != (int(((hv_InternalCurrent.TupleNotEqualElem(hv_Value)).TupleSum())>0)))
    {
      //If the changed model parameter is the learning rate, we also change the momentum
      //to adapt the scale of the previous update.
      if (0 != (int(hv_ModelParam==HTuple("learning_rate"))))
      {
        //Get the threshold.
        GetDictTuple(hv_ChangeStrategy, "scale_momentum_threshold", &hv_ScaleThreshold);
        if (0 != (HTuple(int((hv_ScaleThreshold.TupleLength())>0)).TupleAnd(int(hv_SolverType==HTuple("sgd")))))
        {
          GetDlModelParam(hv_DLModelHandle, hv_ModelParam, &hv_CurrentLR);
          //Check if the change is larger than the specified threshold.
          hv_LRChangeRatio = ((hv_Value.TupleReal())/(hv_CurrentLR.TupleMax2(1e-10))).TupleMax2((hv_CurrentLR.TupleReal())/(hv_Value.TupleMax2(1e-10)));
          if (0 != (HTuple(int(hv_LRChangeRatio>hv_ScaleThreshold)).TupleAnd(int(hv_CurrentLR>1e-7))))
          {
            GetDictParam(hv_ChangeStrategy, "key_exists", "previous_momentum", &hv_PreviousMomentumExists);
            if (0 != hv_PreviousMomentumExists)
            {
              GetDictTuple(hv_ChangeStrategy, "previous_momentum", &hv_CurrentMomentum);
            }
            else
            {
              GetDlModelParam(hv_DLModelHandle, "momentum", &hv_CurrentMomentum);
            }
            hv_AdaptedMomentum = (hv_Value/(hv_CurrentLR.TupleReal()))*hv_CurrentMomentum;
            SetDlModelParam(hv_DLModelHandle, "momentum", hv_AdaptedMomentum);
            //In the next iteration the momentum has to be set back.
            SetDictTuple(hv_ChangeStrategy, "adapted_momentum", 1);
            SetDictTuple(hv_ChangeStrategy, "previous_momentum", hv_CurrentMomentum);
          }
        }
      }
      SetDlModelParam(hv_DLModelHandle, hv_ModelParam, hv_Value);
    }
    else if (0 != (HTuple(int(hv_ModelParam==HTuple("learning_rate"))).TupleAnd(int(hv_SolverType==HTuple("sgd")))))
    {
      //Get the threshold.
      GetDictTuple(hv_ChangeStrategy, "scale_momentum_threshold", &hv_ScaleThreshold);
      if (0 != (int((hv_ScaleThreshold.TupleLength())>0)))
      {
        //Set the momentum back if it was adapted in the previous iteration.
        GetDictParam(hv_ChangeStrategy, "key_exists", "adapted_momentum", &hv_AdaptedMomentumExists);
        if (0 != hv_AdaptedMomentumExists)
        {
          GetDictTuple(hv_ChangeStrategy, "adapted_momentum", &hv_AdaptedMomentum);
          if (0 != hv_AdaptedMomentum)
          {
            GetDictTuple(hv_ChangeStrategy, "previous_momentum", &hv_PreviousMomentum);
            SetDlModelParam(hv_DLModelHandle, "momentum", hv_PreviousMomentum);
            SetDictTuple(hv_ChangeStrategy, "adapted_momentum", 0);
            RemoveDictKey(hv_ChangeStrategy, "previous_momentum");
          }
        }
      }
    }
    //Store the new internal current value.
    SetDictTuple(hv_ChangeStrategy, "internal_current_value", hv_Value);
  }
  }
  return;
}

// Chapter: Deep Learning / Model
// Short Description: Serialize the model if a strategy applies to the current training status. 
void update_train_dl_model_serialization (HTuple hv_TrainParam, HTuple hv_SerializationData, 
    HTuple hv_Iteration, HTuple hv_NumIterations, HTuple hv_Epoch, HTuple hv_EvaluationResult, 
    HTuple hv_DLModelHandle, HTuple hv_TrainInfos, HTuple hv_EvaluationInfos)
{

  // Local iconic variables

  // Local control variables
  HTuple  hv_RawData, hv_Types, hv_Strategies, hv_Index;
  HTuple  hv_Type, hv_Data, hv_Strategy, hv_EvaluationComparisonKeys;
  HTuple  hv_BestScoreHighest, hv_Exception, hv_CurrentBest;
  HTuple  hv_ModelType, hv_Value, hv_Epochs, hv_Indices, hv_LastIndex;
  HTuple  hv_ValidEvaluationKeys, hv_FilenameModel, hv_FilenameMetaData;

  //
  //Serialize the model if a strategy applies to the current training status.
  //
  GetDictTuple(hv_SerializationData, "raw_data", &hv_RawData);
  GetDictTuple(hv_SerializationData, "types", &hv_Types);
  GetDictTuple(hv_SerializationData, "strategies", &hv_Strategies);
  //
  {
  HTuple end_val7 = (hv_Types.TupleLength())-1;
  HTuple step_val7 = 1;
  for (hv_Index=0; hv_Index.Continue(end_val7, step_val7); hv_Index += step_val7)
  {
    //
    hv_Type = HTuple(hv_Types[hv_Index]);
    hv_Data = HTuple(hv_RawData[hv_Index]);
    hv_Strategy = HTuple(hv_Strategies[hv_Index]);
    //
    if (0 != (int(hv_Type==HTuple("best"))))
    {
      //If there is no new evaluation result, we will not serialize.
      if (0 != (int((hv_EvaluationResult.TupleLength())==0)))
      {
        continue;
      }
      //Get result and check if the new result is better than the serialized old result.
      GetDictTuple(hv_TrainParam, "evaluation_comparison_keys", &hv_EvaluationComparisonKeys);
      hv_BestScoreHighest = HTuple();
      try
      {
        GetDictTuple(hv_TrainParam, "evaluation_comparison_keys_best_highest", &hv_BestScoreHighest);
      }
      // catch (Exception) 
      catch (HException &HDevExpDefaultException)
      {
        HDevExpDefaultException.ToHTuple(&hv_Exception);
      }
      //
      reduce_dl_evaluation_result(hv_EvaluationResult, hv_EvaluationComparisonKeys, 
          &hv_Value, &hv_ValidEvaluationKeys);
      GetDictTuple(hv_Data, "best_value", &hv_CurrentBest);
      GetDlModelParam(hv_DLModelHandle, "type", &hv_ModelType);
      if (0 != (int(hv_BestScoreHighest==HTuple())))
      {
        hv_BestScoreHighest = 1;
        if (0 != (int(hv_ModelType==HTuple("classification"))))
        {
          if (0 != (int((HTuple(hv_EvaluationComparisonKeys[0]).TupleRegexpTest("top[0-9]*_error"))==1)))
          {
            hv_BestScoreHighest = 0;
          }
        }
      }
      //
      if (0 != (HTuple(HTuple(int(hv_CurrentBest==-1)).TupleOr(HTuple(int(hv_Value<hv_CurrentBest)).TupleAnd(hv_BestScoreHighest.TupleNot()))).TupleOr(HTuple(int(hv_Value>hv_CurrentBest)).TupleAnd(hv_BestScoreHighest))))
      {
        SetDictTuple(hv_Data, "best_value", hv_Value);
        serialize_train_dl_model_intermediate(hv_DLModelHandle, hv_Epoch, hv_Strategy, 
            hv_TrainInfos, hv_EvaluationInfos, &hv_FilenameModel, &hv_FilenameMetaData);
      }
    }
    else if (0 != (int(hv_Type==HTuple("final"))))
    {
      if (0 != (int(hv_Iteration==(hv_NumIterations-1))))
      {
        //Serialize final model.
        serialize_train_dl_model_intermediate(hv_DLModelHandle, hv_Epoch, hv_Strategy, 
            hv_TrainInfos, hv_EvaluationInfos, &hv_FilenameModel, &hv_FilenameMetaData);
      }
    }
    else if (0 != (HTuple(int(hv_Type==HTuple("epochs"))).TupleOr(int(hv_Type==HTuple("current")))))
    {
      //Check if the specified epoch is reached.
      GetDictTuple(hv_Data, "epochs", &hv_Epochs);
      TupleFindLast(hv_Epoch.TupleLessElem(hv_Epochs), 0, &hv_Indices);
      //Also check that the last saved epoch is not the same.
      GetDictTuple(hv_Data, "last_epoch_index", &hv_LastIndex);
      if (0 != (HTuple(int(hv_Type==HTuple("current"))).TupleAnd(int((hv_EvaluationResult.TupleLength())>0))))
      {
        //For type current we also write every EvaluationIntervalEpochs epochs.
        serialize_train_dl_model_intermediate(hv_DLModelHandle, hv_Epoch, hv_Strategy, 
            hv_TrainInfos, hv_EvaluationInfos, &hv_FilenameModel, &hv_FilenameMetaData);
      }
      else if (0 != (int((hv_Indices.TupleLength())>0)))
      {
        //
        if (0 != (HTuple(int(HTuple(hv_Indices[0])>-1)).TupleAnd(int(HTuple(hv_Indices[0])!=hv_LastIndex))))
        {
          SetDictTuple(hv_Data, "last_epoch_index", HTuple(hv_Indices[0]));
          //Serialize final model.
          serialize_train_dl_model_intermediate(hv_DLModelHandle, hv_Epoch, hv_Strategy, 
              hv_TrainInfos, hv_EvaluationInfos, &hv_FilenameModel, &hv_FilenameMetaData);
        }
      }
    }
  }
  }
  //
  return;
}

// Chapter: Graphics / Window
// Short Description: This procedure sets and returns meta information to display images correctly. 
void update_window_meta_information (HTuple hv_WindowHandle, HTuple hv_WidthImage, 
    HTuple hv_HeightImage, HTuple hv_WindowRow1, HTuple hv_WindowColumn1, HTuple hv_MapColorBarWidth, 
    HTuple hv_MarginBottom, HTuple *hv_WindowImageRatioHeight, HTuple *hv_WindowImageRatioWidth, 
    HTuple *hv_SetPartRow2, HTuple *hv_SetPartColumn2, HTuple *hv_PrevWindowCoordinatesOut)
{

  // Local iconic variables

  // Local control variables
  HTuple  hv__, hv_WindowWidth, hv_WindowHeight;
  HTuple  hv_WindowRow2, hv_WindowColumn2, hv_WindowRatio;
  HTuple  hv_ImageRow2, hv_ImageColumn2, hv_ImageRatio, hv_ImageWindowRatioHeight;
  HTuple  hv_ImageRow2InWindow, hv_ImageCol2InWindow;

  //
  //This procedure sets and returns meta information to display images correctly.
  //
  //Set part for the image to be displayed later and adapt window size (+ MarginBottom + MapColorBarWidth).
  GetWindowExtents(hv_WindowHandle, &hv__, &hv__, &hv_WindowWidth, &hv_WindowHeight);
  (*hv_WindowImageRatioHeight) = hv_WindowHeight/(hv_HeightImage*1.0);
  (*hv_WindowImageRatioWidth) = hv_WindowWidth/(hv_WidthImage*1.0);
  //
  //Set window part such that image is displayed undistorted.
  hv_WindowRow2 = hv_WindowHeight;
  hv_WindowColumn2 = hv_WindowWidth;
  hv_WindowRatio = hv_WindowColumn2/(hv_WindowRow2*1.0);
  //
  hv_ImageRow2 = hv_HeightImage+(hv_MarginBottom/(*hv_WindowImageRatioHeight));
  hv_ImageColumn2 = hv_WidthImage+(hv_MapColorBarWidth/(*hv_WindowImageRatioWidth));
  hv_ImageRatio = hv_ImageColumn2/(hv_ImageRow2*1.0);
  if (0 != (int(hv_ImageRatio>hv_WindowRatio)))
  {
    //
    //Extend image until right window border.
    (*hv_SetPartColumn2) = hv_ImageColumn2;
    hv_ImageWindowRatioHeight = hv_ImageColumn2/(hv_WindowColumn2*1.0);
    hv_ImageRow2InWindow = hv_ImageRow2/hv_ImageWindowRatioHeight;
    (*hv_SetPartRow2) = hv_ImageRow2+((hv_WindowRow2-hv_ImageRow2InWindow)/(*hv_WindowImageRatioWidth));
  }
  else
  {
    //
    //Extend image until bottom of window.
    (*hv_SetPartRow2) = hv_ImageRow2;
    hv_ImageWindowRatioHeight = hv_ImageRow2/(hv_WindowRow2*1.0);
    hv_ImageCol2InWindow = hv_ImageColumn2/hv_ImageWindowRatioHeight;
    (*hv_SetPartColumn2) = hv_ImageColumn2+((hv_WindowColumn2-hv_ImageCol2InWindow)/(*hv_WindowImageRatioHeight));
  }
  if (HDevWindowStack::IsOpen())
    SetPart(HDevWindowStack::GetActive(),0, 0, (*hv_SetPartRow2)-1, (*hv_SetPartColumn2)-1);
  //
  //Return the coordinates of the new window.
  (*hv_PrevWindowCoordinatesOut).Clear();
  (*hv_PrevWindowCoordinatesOut).Append(hv_WindowRow1);
  (*hv_PrevWindowCoordinatesOut).Append(hv_WindowColumn1);
  (*hv_PrevWindowCoordinatesOut).Append(hv_WindowWidth);
  (*hv_PrevWindowCoordinatesOut).Append(hv_WindowHeight);
  //
  return;
}

// Chapter: Deep Learning / Evaluation
// Short Description: Check that all given entries in EvalParams are valid. 
void validate_evaluation_param (HTuple hv_EvalParams, HTuple *hv_Valid, HTuple *hv_Exception)
{

  // Local iconic variables

  // Local control variables
  HTuple  hv_ClassIDsExist, hv_ClassIDs, hv_NumClassesExist;
  HTuple  hv_NumClasses, hv_EvalInstancesExists, hv_EvaluationTypeExists;
  HTuple  hv_EvaluationType, hv_Indices, hv_MeasuresExists;
  HTuple  hv_Measures, hv_ValidMeasures, hv_Ks, hv_KeysExist;
  HTuple  hv_ClassNames, hv_ClassesToEvaluate, hv_ClassIDsToEvaluate;
  HTuple  hv_IouThreshExists, hv_IouThresholds, hv_MaxNumDetectionsExists;
  HTuple  hv_MaxNumDetections, hv_AreaRangesExist, hv_AreaRanges;
  HTuple  hv_AreaKeysExist, hv_AreaNames, hv_MinAreas, hv_MaxAreas;
  HTuple  hv_InstanceTypeExists, hv_InstanceType, hv_ValidInstanceTypes;
  HTuple  hv_AllocationBlockLengthExists, hv_AllocationBlockLength;
  HTuple  hv_DetailedEvaluationExists, hv_DetailedEvaluation;
  HTuple  hv_KeyExists, hv_InterpolatePRCurves, hv_IgnoreClassIDsExist;
  HTuple  hv_ValidMeasuresString, hv_Idx, hv_ValidMeasure;

  //
  //This procedure checks if the dictionary EvalParams
  //contains all necessary parameters and if they are valid (type, range, ...).
  //
  (*hv_Valid) = 0;
  (*hv_Exception) = "";
  //Check class IDs.
  GetDictParam(hv_EvalParams, "key_exists", "class_ids", &hv_ClassIDsExist);
  if (0 != (hv_ClassIDsExist.TupleNot()))
  {
    (*hv_Exception) = "The evaluation parameters need a key-value pair for 'class_ids'";
    return;
  }
  else
  {
    GetDictTuple(hv_EvalParams, "class_ids", &hv_ClassIDs);
    if (0 != (int((hv_ClassIDs.TupleLength())<1)))
    {
      (*hv_Exception) = "'class_ids' should have at least length 1";
      return;
    }
    if (0 != (int((hv_ClassIDs.TupleIsIntElem())!=HTuple(hv_ClassIDs.TupleLength(),1))))
    {
      (*hv_Exception) = "'class_ids' should be of type int";
      return;
    }
    if (0 != (int(((hv_ClassIDs.TupleLessElem(0)).TupleFind(1))>-1)))
    {
      (*hv_Exception) = "'class_ids' should be positive or zero";
      return;
    }
  }
  //Check the entry num_classes.
  GetDictParam(hv_EvalParams, "key_exists", "num_classes", &hv_NumClassesExist);
  if (0 != (hv_NumClassesExist.TupleNot()))
  {
    (*hv_Exception) = "The evaluation parameters need a key-value pair for 'num_classes'";
    return;
  }
  else
  {
    GetDictTuple(hv_EvalParams, "num_classes", &hv_NumClasses);
    if (0 != (int((hv_NumClasses.TupleLength())!=1)))
    {
      (*hv_Exception) = "'num_classes' should have length 1";
      return;
    }
    if (0 != (int((hv_NumClasses.TupleType())!=(HTuple(HTuple(1).TupleInt()).TupleType()))))
    {
      (*hv_Exception) = "'num_classes' should be of type int";
      return;
    }
    if (0 != (int(hv_NumClasses<1)))
    {
      (*hv_Exception) = "'num_classes' should be at least 1";
      return;
    }
  }
  //Check that num_classes is equal to |class_ids|.
  if (0 != (int(hv_NumClasses!=(hv_ClassIDs.TupleLength()))))
  {
    (*hv_Exception) = "'num_classes' has to be set to the number of 'class_ids'";
    return;
  }
  //Check the entry 'evaluate_instances'.
  GetDictParam(hv_EvalParams, "key_exists", "evaluate_instances", &hv_EvalInstancesExists);
  if (0 != (hv_EvalInstancesExists.TupleNot()))
  {
    (*hv_Exception) = "The evaluation parameters need a key-value pair for 'evaluate_instances'";
    return;
  }
  //Check the entry 'evaluation_type'.
  GetDictParam(hv_EvalParams, "key_exists", "evaluation_type", &hv_EvaluationTypeExists);
  if (0 != (hv_EvaluationTypeExists.TupleNot()))
  {
    (*hv_Exception) = "The evaluation parameters need a key-value pair for 'evaluation_type'";
    return;
  }
  else
  {
    GetDictTuple(hv_EvalParams, "evaluation_type", &hv_EvaluationType);
    TupleFind((((HTuple("anomaly_detection").Append("classification")).Append("segmentation")).Append("detection")), 
        hv_EvaluationType, &hv_Indices);
    if (0 != (HTuple(int(hv_Indices==-1)).TupleOr(int(hv_Indices==HTuple()))))
    {
      (*hv_Exception) = "Invalid entry for 'evaluation_type': "+hv_EvaluationType;
      return;
    }
  }
  //Check the entry 'measures'.
  GetDictParam(hv_EvalParams, "key_exists", "measures", &hv_MeasuresExists);
  if (0 != (hv_MeasuresExists.TupleNot()))
  {
    (*hv_Exception) = "The evaluation parameters need a key-value pair for 'measures'";
    return;
  }
  else
  {
    GetDictTuple(hv_EvalParams, "measures", &hv_Measures);
    if (0 != (int((hv_Measures.TupleLength())==0)))
    {
      (*hv_Exception) = "'measures' should contain at least one entry";
      return;
    }
  }
  //Check evaluation type specific entries of EvalParams.
  GetDictTuple(hv_EvalParams, "evaluation_type", &hv_EvaluationType);
  if (0 != (int(hv_EvaluationType==HTuple("anomaly_detection"))))
  {
    //
    //Check for correct number of classes.
    if (0 != (int((hv_ClassIDs.TupleLength())!=2)))
    {
      throw HException("The number of classes must be 2 for model type anomaly detection.");
    }
    //
    hv_ValidMeasures.Clear();
    hv_ValidMeasures[0] = "anomaly_score_histogram";
    hv_ValidMeasures[1] = "precision";
    hv_ValidMeasures[2] = "recall";
    hv_ValidMeasures[3] = "absolute_confusion_matrix";
    hv_ValidMeasures[4] = "relative_confusion_matrix";
    hv_ValidMeasures[5] = "all";
  }
  else if (0 != (int(hv_EvaluationType==HTuple("classification"))))
  {
    TupleGenSequence(1, hv_ClassIDs.TupleLength(), 1, &hv_Ks);
    hv_ValidMeasures = ("top"+hv_Ks)+"_error";
    hv_ValidMeasures = hv_ValidMeasures.TupleConcat(((((HTuple("precision").Append("recall")).Append("f_score")).Append("absolute_confusion_matrix")).Append("relative_confusion_matrix")));
    //
    //Check if not both of the two options to specify the evaluated classes are chosen.
    GetDictParam(hv_EvalParams, "key_exists", (HTuple("class_names_to_evaluate").Append("class_ids_to_evaluate")), 
        &hv_KeysExist);
    if (0 != (int((hv_KeysExist.TupleSum())==2)))
    {
      (*hv_Exception) = "No more than one option of 'class_names_to_evaluate' and 'class_ids_to_evaluate' is allowed";
      return;
    }
    if (0 != (HTuple(hv_KeysExist[0])))
    {
      GetDictTuple(hv_EvalParams, "class_names", &hv_ClassNames);
      GetDictTuple(hv_EvalParams, "class_names_to_evaluate", &hv_ClassesToEvaluate);
      if (0 != (int((hv_ClassesToEvaluate.TupleDifference(hv_ClassNames.TupleConcat("global")))!=HTuple())))
      {
        (*hv_Exception) = "Invalid entry in 'class_names_to_evaluate'";
        return;
      }
    }
    if (0 != (HTuple(hv_KeysExist[1])))
    {
      GetDictTuple(hv_EvalParams, "class_ids", &hv_ClassIDs);
      GetDictTuple(hv_EvalParams, "class_ids_to_evaluate", &hv_ClassIDsToEvaluate);
      if (0 != (int((hv_ClassIDsToEvaluate.TupleDifference(hv_ClassIDs.TupleConcat("global")))!=HTuple())))
      {
        (*hv_Exception) = "Invalid entry in 'class_ids_to_evaluate'";
        return;
      }
    }
  }
  else if (0 != (int(hv_EvaluationType==HTuple("detection"))))
  {
    //Instance-based evaluation.
    //Add instance measures.
    hv_ValidMeasures.Clear();
    hv_ValidMeasures[0] = "all";
    hv_ValidMeasures[1] = "mean_ap";
    //
    //Check if the entry 'iou_threshold' is present.
    GetDictParam(hv_EvalParams, "key_exists", "iou_threshold", &hv_IouThreshExists);
    if (0 != (hv_IouThreshExists.TupleNot()))
    {
      (*hv_Exception) = "The evaluation parameters need a key-value pair for 'iou_threshold'";
      return;
    }
    else
    {
      GetDictTuple(hv_EvalParams, "iou_threshold", &hv_IouThresholds);
      //Check the length of 'iou_threshold'.
      if (0 != (int((hv_IouThresholds.TupleLength())<1)))
      {
        (*hv_Exception) = "'iou_threshold' is empty";
        return;
      }
      if (0 != (int((hv_IouThresholds.TupleIsRealElem())!=HTuple(hv_IouThresholds.TupleLength(),1))))
      {
        (*hv_Exception) = "'iou_threshold' should be of type real";
        return;
      }
      //Check if the IoU thresholds are within (0.0, 1.0).
      if (0 != (HTuple(int((hv_IouThresholds.TupleMin())<=0.0)).TupleOr(int((hv_IouThresholds.TupleMax())>=1.0))))
      {
        (*hv_Exception) = HTuple("Invalid 'iou_threshold', not in range (0.0, 1.0)");
        return;
      }
    }
    //
    //Check if the entry 'max_num_detections' is present.
    GetDictParam(hv_EvalParams, "key_exists", "max_num_detections", &hv_MaxNumDetectionsExists);
    if (0 != (hv_MaxNumDetectionsExists.TupleNot()))
    {
      (*hv_Exception) = "The evaluation parameters need a key-value pair for 'max_num_detections'";
      return;
    }
    else
    {
      GetDictTuple(hv_EvalParams, "max_num_detections", &hv_MaxNumDetections);
      //Check the length of 'max_num_detections'.
      if (0 != (int((hv_MaxNumDetections.TupleLength())<1)))
      {
        (*hv_Exception) = "'max_num_detections' is empty";
        return;
      }
      if (0 != (int((hv_MaxNumDetections.TupleIsIntElem())!=HTuple(hv_MaxNumDetections.TupleLength(),1))))
      {
        (*hv_Exception) = "'max_num_detections' should be of type int";
        return;
      }
      //Check if 'max_num_detections' is -1 (to use all detections) or positive.
      if (0 != (HTuple(int((hv_MaxNumDetections.TupleMin())<-1)).TupleOr(int((hv_MaxNumDetections.TupleFind(0))>-1))))
      {
        (*hv_Exception) = "'max_num_detections' should be -1 or positive";
        return;
      }
    }
    //
    //Check if the entry 'area_ranges' is present.
    GetDictParam(hv_EvalParams, "key_exists", "area_ranges", &hv_AreaRangesExist);
    if (0 != (hv_AreaRangesExist.TupleNot()))
    {
      (*hv_Exception) = "The evaluation parameters need a key-value pair for 'area_ranges'";
      return;
    }
    else
    {
      //Check if the entry 'area_ranges' is a dict.
      GetDictTuple(hv_EvalParams, "area_ranges", &hv_AreaRanges);
      if (0 != (int((hv_AreaRanges.TupleSemType())!=HTuple("dict"))))
      {
        (*hv_Exception) = "'area_ranges' must be a dict";
        return;
      }
      //Check that the necessary keys exist.
      GetDictParam(hv_AreaRanges, "key_exists", ((HTuple("name").Append("min")).Append("max")), 
          &hv_AreaKeysExist);
      if (0 != (HTuple(hv_AreaKeysExist[0]).TupleNot()))
      {
        (*hv_Exception) = "'area_ranges' need a key-value pair for 'name'";
        return;
      }
      if (0 != (HTuple(hv_AreaKeysExist[1]).TupleNot()))
      {
        (*hv_Exception) = "'area_ranges' need a key-value pair for 'min'";
        return;
      }
      if (0 != (HTuple(hv_AreaKeysExist[2]).TupleNot()))
      {
        (*hv_Exception) = "'area_ranges' need a key-value pair for 'max'";
        return;
      }
      //Check the lengths of the area keys.
      GetDictTuple(hv_AreaRanges, "name", &hv_AreaNames);
      if (0 != (int((hv_AreaNames.TupleLength())<1)))
      {
        (*hv_Exception) = "'area_ranges': 'name' is empty";
        return;
      }
      GetDictTuple(hv_AreaRanges, "min", &hv_MinAreas);
      if (0 != (int((hv_MinAreas.TupleLength())<1)))
      {
        (*hv_Exception) = "'area_ranges': 'min' is empty";
        return;
      }
      GetDictTuple(hv_AreaRanges, "max", &hv_MaxAreas);
      if (0 != (int((hv_MaxAreas.TupleLength())<1)))
      {
        (*hv_Exception) = "'area_ranges': 'max' is empty";
        return;
      }
      if (0 != (HTuple(int((hv_AreaNames.TupleLength())!=(hv_MinAreas.TupleLength()))).TupleOr(int((hv_AreaNames.TupleLength())!=(hv_MaxAreas.TupleLength())))))
      {
        (*hv_Exception) = HTuple("'area_ranges': 'name', 'min' and 'max' must have the same length");
        return;
      }
      //Check values of min, max.
      if (0 != (int(((hv_MinAreas.TupleGreaterEqualElem(hv_MaxAreas)).TupleFind(1))>-1)))
      {
        (*hv_Exception) = "'area_ranges': 'min' must be elementwise smaller than 'max'";
        return;
      }
    }
    //
    //Check if instance-type is valid.
    GetDictParam(hv_EvalParams, "key_exists", "instance_type", &hv_InstanceTypeExists);
    if (0 != (hv_InstanceTypeExists.TupleNot()))
    {
      (*hv_Exception) = "The evaluation parameters need a key-value pair for 'instance_type'";
      return;
    }
    else
    {
      GetDictTuple(hv_EvalParams, "instance_type", &hv_InstanceType);
      hv_ValidInstanceTypes.Clear();
      hv_ValidInstanceTypes[0] = "rectangle1";
      hv_ValidInstanceTypes[1] = "rectangle2";
      if (0 != (int((hv_ValidInstanceTypes.TupleFind(hv_InstanceType))==-1)))
      {
        (*hv_Exception) = ("Invalid instance type '"+hv_InstanceType)+"'";
        return;
      }
    }
    //
    //Check if the entry 'allocation_block_length' is present and valid.
    GetDictParam(hv_EvalParams, "key_exists", "allocation_block_length", &hv_AllocationBlockLengthExists);
    if (0 != (hv_AllocationBlockLengthExists.TupleNot()))
    {
      (*hv_Exception) = "The evaluation parameters need a key-value pair for 'allocation_block_length'";
      return;
    }
    else
    {
      GetDictTuple(hv_EvalParams, "allocation_block_length", &hv_AllocationBlockLength);
      //Check the length of 'allocation_block_length'.
      if (0 != (int((hv_AllocationBlockLength.TupleLength())!=1)))
      {
        (*hv_Exception) = "'allocation_block_length' should have length 1";
        return;
      }
      //Check the type of 'allocation_block_length'.
      if (0 != (int((hv_AllocationBlockLength.TupleType())!=(HTuple(HTuple(0).TupleInt()).TupleType()))))
      {
        (*hv_Exception) = "'allocation_block_length' should be of type int";
        return;
      }
      //Check if 'allocation_block_length' is larger than zero.
      if (0 != (int(hv_AllocationBlockLength<1)))
      {
        (*hv_Exception) = "'allocation_block_length' should be positive";
        return;
      }
    }
    //
    //Check if the entry 'detailed_evaluation' is valid if present.
    GetDictParam(hv_EvalParams, "key_exists", "detailed_evaluation", &hv_DetailedEvaluationExists);
    if (0 != (hv_DetailedEvaluationExists.TupleNot()))
    {
      (*hv_Exception) = "The evaluation parameters need a key-value pair for 'detailed_evaluation'";
      return;
    }
    else
    {
      GetDictTuple(hv_EvalParams, "detailed_evaluation", &hv_DetailedEvaluation);
      //Check the length of 'detailed_evaluation'.
      if (0 != (int((hv_DetailedEvaluation.TupleLength())!=1)))
      {
        (*hv_Exception) = "'detailed_evaluation' should have length 1";
        return;
      }
      //Check the type of 'detailed_evaluation'.
      if (0 != (int((hv_DetailedEvaluation.TupleType())!=(HTuple(1).TupleType()))))
      {
        (*hv_Exception) = "'detailed_evaluation' should be of type int";
        return;
      }
      //Check if 'detailed_evaluation' is true or false.
      if (0 != (HTuple(int(hv_DetailedEvaluation!=0)).TupleAnd(int(hv_DetailedEvaluation!=1))))
      {
        (*hv_Exception) = "'detailed_evaluation' should be zero or one";
        return;
      }
    }
    //
    //Check if the entry 'interpolate_pr_curves' is valid if present.
    GetDictParam(hv_EvalParams, "key_exists", "interpolate_pr_curves", &hv_KeyExists);
    if (0 != (hv_KeyExists.TupleNot()))
    {
      (*hv_Exception) = "The evaluation parameters need a key-value pair for 'interpolate_pr_curves'";
      return;
    }
    else
    {
      GetDictTuple(hv_EvalParams, "interpolate_pr_curves", &hv_InterpolatePRCurves);
      //Check the length of 'interpolate_pr_curves'.
      if (0 != (int((hv_InterpolatePRCurves.TupleLength())!=1)))
      {
        (*hv_Exception) = "'interpolate_pr_curves' should have length 1";
        return;
      }
      //Check the type of 'interpolate_pr_curves'.
      if (0 != (int((hv_InterpolatePRCurves.TupleType())!=(HTuple(1).TupleType()))))
      {
        (*hv_Exception) = "'interpolate_pr_curves' should be of type int";
        return;
      }
      //Check if 'interpolate_pr_curves' is true or false.
      if (0 != (HTuple(int(hv_InterpolatePRCurves!=0)).TupleAnd(int(hv_InterpolatePRCurves!=1))))
      {
        (*hv_Exception) = "'interpolate_pr_curves' should be zero or one";
        return;
      }
    }
    //
    //Add valid measure 'soap' if instance_type is 'rectangle2'.
    if (0 != (int(hv_InstanceType==HTuple("rectangle2"))))
    {
      hv_ValidMeasures = hv_ValidMeasures.TupleConcat("soap");
    }
  }
  else if (0 != (int(hv_EvaluationType==HTuple("segmentation"))))
  {
    //Pixel-based evaluation.
    hv_ValidMeasures.Clear();
    hv_ValidMeasures[0] = "all";
    hv_ValidMeasures[1] = "pixel_accuracy";
    hv_ValidMeasures[2] = "mean_accuracy";
    hv_ValidMeasures[3] = "mean_iou";
    hv_ValidMeasures[4] = "class_iou";
    hv_ValidMeasures[5] = "class_pixel_accuracy";
    hv_ValidMeasures[6] = "pixel_confusion_matrix";
    hv_ValidMeasures[7] = "frequency_weighted_iou";
    //
    //Check if the entry 'ignore_class_ids' exists.
    GetDictParam(hv_EvalParams, "key_exists", "ignore_class_ids", &hv_IgnoreClassIDsExist);
    if (0 != (hv_IgnoreClassIDsExist.TupleNot()))
    {
      (*hv_Exception) = "The evaluation parameters need a key-value pair for 'ignore_class_ids'";
      return;
    }
  }
  else
  {
    (*hv_Exception) = "Unknown evaluation_type: "+hv_EvaluationType;
    return;
  }
  //Check measures.
  hv_ValidMeasuresString = HTuple(((hv_ValidMeasures.TupleLength())*2)-1,HTuple("','"));
  hv_ValidMeasuresString[HTuple::TupleGenSequence(0,(hv_ValidMeasuresString.TupleLength())-1,2)] = hv_ValidMeasures;
  hv_ValidMeasuresString = hv_ValidMeasuresString.TupleSum();
  {
  HTuple end_val331 = (hv_Measures.TupleLength())-1;
  HTuple step_val331 = 1;
  for (hv_Idx=0; hv_Idx.Continue(end_val331, step_val331); hv_Idx += step_val331)
  {
    hv_ValidMeasure = (hv_ValidMeasures.TupleFind(HTuple(hv_Measures[hv_Idx]))).TupleGreaterElem(-1);
    if (0 != (hv_ValidMeasure.TupleNot()))
    {
      (*hv_Exception) = ((("Invalid measure '"+HTuple(hv_Measures[hv_Idx]))+HTuple("', choose one of ['"))+hv_ValidMeasuresString)+"']";
      return;
    }
  }
  }
  //
  //Done with checks.
  (*hv_Valid) = 1;
  return;
}

// Chapter: Deep Learning / Model
// Short Description: Write the dictionaries of the samples in DLSampleBatch to hdict files and store the paths in DLDataset. 
void write_dl_samples (HTuple hv_DLDataset, HTuple hv_SampleIndices, HTuple hv_DLSampleBatch, 
    HTuple hv_GenParamName, HTuple hv_GenParamValue)
{

  // Local iconic variables

  // Local control variables
  HTuple  hv_RaiseErrorWriteDict, hv_IndexGenParam;
  HTuple  hv_DLSampleDirExists, hv_OutDir, hv_DatasetSamples;
  HTuple  hv_I, hv_DLDatasetIndex, hv_DatasetSample, hv_DLSample;
  HTuple  hv_DatasetImageID, hv_SampleImageID, hv_FileNameOut;

  //
  //This procedure writes all given DLSamples in DLSampleBatch to hdict files
  //and stores the file paths in the respective samples of the DLDataset.
  //The directory needs to be given in dlsample_dir, before calling this procedure.
  //
  //The output filename is created in the following way: image_id + '_dlsample.hdict'
  //
  //Set the default values.
  //Raise error when writing dictionary.
  hv_RaiseErrorWriteDict = "true";
  //
  //Transfer generic parameters.
  if (0 != (int((hv_GenParamName.TupleLength())!=(hv_GenParamValue.TupleLength()))))
  {
    throw HException("GenParamName and GenParamValue have to be of equal length.");
  }
  //
  if (0 != (int(hv_GenParamName!=HTuple())))
  {
    {
    HTuple end_val17 = (hv_GenParamName.TupleLength())-1;
    HTuple step_val17 = 1;
    for (hv_IndexGenParam=0; hv_IndexGenParam.Continue(end_val17, step_val17); hv_IndexGenParam += step_val17)
    {
      if (0 != (int(HTuple(hv_GenParamName[hv_IndexGenParam])==HTuple("raise_error_if_content_not_serializable"))))
      {
        //Set 'raise_error_if_content_not_serializable' for writing write_dict.
        hv_RaiseErrorWriteDict = HTuple(hv_GenParamValue[hv_IndexGenParam]);
      }
      else
      {
        throw HException("Unknown GenParam key : "+HTuple(hv_GenParamName[hv_IndexGenParam]));
      }
    }
    }
  }
  //
  //Check the parameters.
  //Check that the base path is available in the DLDataset.
  GetDictParam(hv_DLDataset, "key_exists", "dlsample_dir", &hv_DLSampleDirExists);
  if (0 != (hv_DLSampleDirExists.TupleNot()))
  {
    throw HException("The dataset needs to include the key 'dlsample_dir'.");
    return;
  }

  if (0 != (int((hv_DLSampleBatch.TupleLength())!=(hv_SampleIndices.TupleLength()))))
  {
    throw HException("The input tuples DLSampleBatch and SampleIndices need to match in length.");
    return;
  }
  //
  //Write preprocessed data.
  //
  //Get the base path for the outputs.
  GetDictTuple(hv_DLDataset, "dlsample_dir", &hv_OutDir);
  //
  //Get the samples.
  GetDictTuple(hv_DLDataset, "samples", &hv_DatasetSamples);
  //
  //Loop over all samples in the batch.
  {
  HTuple end_val49 = (hv_DLSampleBatch.TupleLength())-1;
  HTuple step_val49 = 1;
  for (hv_I=0; hv_I.Continue(end_val49, step_val49); hv_I += step_val49)
  {
    //Get the sample dictionaries.
    hv_DLDatasetIndex = HTuple(hv_SampleIndices[hv_I]);
    hv_DatasetSample = HTuple(hv_DatasetSamples[hv_DLDatasetIndex]);
    hv_DLSample = HTuple(hv_DLSampleBatch[hv_I]);
    //
    //Check that image IDs match.
    GetDictTuple(hv_DatasetSample, "image_id", &hv_DatasetImageID);
    GetDictTuple(HTuple(hv_DLSampleBatch[hv_I]), "image_id", &hv_SampleImageID);
    if (0 != (int(hv_DatasetImageID!=hv_SampleImageID)))
    {
      throw HException("Image IDs do not match. Please use correct indexing in input argument SampleIndices.");
      return;
    }
    //
    //Generate the output file name.
    hv_FileNameOut = hv_SampleImageID+"_dlsample.hdict";
    //
    //Write output dictionary.
    WriteDict(hv_DLSample, (hv_OutDir+"/")+hv_FileNameOut, "raise_error_if_content_not_serializable", 
        hv_RaiseErrorWriteDict);
    //Add output path to DLDataset sample dictionary.
    SetDictTuple(HTuple(hv_DatasetSamples[hv_DLDatasetIndex]), "dlsample_file_name", 
        hv_FileNameOut);
    //
  }
  }

  return;
}

// Local procedures 
void get_image_dir (HTuple *hv_ImageDir)
{

  // Local iconic variables

  // Local control variables
  HTuple  hv_OperatingSystem, hv_IsWindows, hv_Separator;

  GetSystem("image_dir", &(*hv_ImageDir));
  GetSystem("operating_system", &hv_OperatingSystem);
  hv_IsWindows = int((hv_OperatingSystem.TupleSubstr(0,2))==HTuple("Win"));
  if (0 != hv_IsWindows)
  {
    hv_Separator = ";";
  }
  else
  {
    hv_Separator = ":";
  }
  TupleSplit((*hv_ImageDir), hv_Separator, &(*hv_ImageDir));
  (*hv_ImageDir) = ((const HTuple&)(*hv_ImageDir))[0];
  (*hv_ImageDir) = (*hv_ImageDir).TupleRegexpReplace((HTuple("\\\\+").Append("replace_all")),"/");
  return;
}

void preprocess_dl_samples_bottle (HTuple hv_DLSamples)
{

  // Local iconic variables
  HObject  ho_SampleImage, ho_GrayImage, ho_CenterRegion;
  HObject  ho_CenterRegionFillUp, ho_SampleImageReduced;

  // Local control variables
  HTuple  hv_SampleIndex;

  //
  //This procedure performs a threshold segmentation to remove the
  //white background. This improves the performance of the anomaly
  //detection model.
  //
  //Loop over all samples in the dataset.
  {
  HTuple end_val6 = (hv_DLSamples.TupleLength())-1;
  HTuple step_val6 = 1;
  for (hv_SampleIndex=0; hv_SampleIndex.Continue(end_val6, step_val6); hv_SampleIndex += step_val6)
  {
    GetDictObject(&ho_SampleImage, HTuple(hv_DLSamples[hv_SampleIndex]), "image");
    //
    //Since anomalies are only located on the bottles we exclude
    //the white areas surrounding the bottles.
    Rgb1ToGray(ho_SampleImage, &ho_GrayImage);
    Threshold(ho_GrayImage, &ho_CenterRegion, "min", 1.5);
    FillUp(ho_CenterRegion, &ho_CenterRegionFillUp);
    ReduceDomain(ho_SampleImage, ho_CenterRegionFillUp, &ho_SampleImageReduced);
    //
    //Update the sample.
    SetDictObject(ho_SampleImageReduced, HTuple(hv_DLSamples[hv_SampleIndex]), "image");
  }
  }
  //
  return;
}

// Generated stubs for parallel procedure calls. Wrapped in name
// space to avoid name conflicts with actual procedure names
namespace HDevExportCpp
{
// Parallel execution wrapper for create_dl_dataset_anomaly_samples(...) 
static void* _hcppthread_create_dl_dataset_anomaly_samples(void *hcthread)
{
  // +++ define thread context for this procedure
  HDevThread*  hcppthread = (HDevThread*) hcthread;
  try
  {
    // Input parameters
    const HTuple        &cbhv_ImageList = hcppthread->GetInputCtrlParamTuple(0);
    const HTuple        &cbhv_LabelList = hcppthread->GetInputCtrlParamTuple(1);
    const HTuple        &cbhv_AnomalyList = hcppthread->GetInputCtrlParamTuple(2);
    const HTuple        &cbhv_AnomalyDirGiven = hcppthread->GetInputCtrlParamTuple(3);
    const HTuple        &cbhv_NumSamplesPerThread = hcppthread->GetInputCtrlParamTuple(4);
    const HTuple        &cbhv_UniqueIndex = hcppthread->GetInputCtrlParamTuple(5);

    // Output parameters
    HTuple        cbhv_SamplesOutput;
    HTuple        cbhv_Exception;

    // Call create_dl_dataset_anomaly_samples
    create_dl_dataset_anomaly_samples(cbhv_ImageList, cbhv_LabelList, cbhv_AnomalyList, 
        cbhv_AnomalyDirGiven, cbhv_NumSamplesPerThread, cbhv_UniqueIndex, &cbhv_SamplesOutput, 
        &cbhv_Exception);

    // Store output parameters in thread object
    hcppthread->StoreOutputCtrlParamTuple(0,cbhv_SamplesOutput);
    hcppthread->StoreOutputCtrlParamTuple(1,cbhv_Exception);

    // Reduce reference counter of thread object
    hcppthread->Exit();
    delete hcppthread;

  }
  catch (HException& exc)
  {
    // No exceptions may be raised from stub in parallel case,
    // so we need to store this information prior to cleanup
    bool is_direct_call = hcppthread->IsDirectCall();
    // Attempt to clean up in error case, too
    hcppthread->Exit();
    delete hcppthread;
    // Propagate exception if called directly
    if (is_direct_call)
      throw exc;
  }
  return NULL;
}

}

// Main procedure 
void action()
{

  // Local iconic variables
  HObject  ho_Image;

  // Local control variables
  HTuple  hv_HalconImages, hv_ImageDir, hv_ImageSubDirs;
  HTuple  hv_AnomalyDir, hv_OutputDir, hv_ExampleSpecificPreprocessing;
  HTuple  hv_ImageWidth, hv_ImageHeight, hv_Complexity, hv_GenParamDataset;
  HTuple  hv_DLDataset, hv_DLModelHandle, hv_DLDevice, hv_PreprocessSettings;
  HTuple  hv_DLPreprocessParam, hv_DLDatasetFileName, hv_DatasetSamples;
  HTuple  hv_DLSampleBatch, hv_WindowDict, hv_Index, hv_SampleIndex;
  HTuple  hv_DLSample, hv_WindowHandles, hv_EnableDisplay;
  HTuple  hv_ErrorThreshold, hv_MaxNumEpochs, hv_DomainRatio;
  HTuple  hv_RegularizationNoise, hv_TrainParamAnomaly, hv_TrainParam;
  HTuple  hv_TrainResults, hv_TrainInfos, hv_EvaluationInfos;
  HTuple  hv_StandardDeviationFactor, hv_GenParamThreshold;
  HTuple  hv_AnomalySegmentationThreshold, hv_AnomalyClassificationThresholds;
  HTuple  hv_GenParamEvaluation, hv_EvaluationResult, hv_EvalParams;
  HTuple  hv_GenParamDisplay, hv_ClassificationThresholdIndex;
  HTuple  hv_ImageFiles, hv_ImageFilesShuffled, hv_InferenceClassificationThreshold;
  HTuple  hv_InferenceSegmentationThreshold, hv_DLDatasetInfo;
  HTuple  hv_IndexInference, hv_DLResult;

  //
  //Deep learning anomaly detection example.
  //
  //This example demonstrates the general workflow for anomaly detection
  //based on deep learning.
  //
  //The workflow typically consists of four steps which are described here:
  //1. Dataset preprocessing.
  //2. Training of the model and threshold estimation on 'ok' images.
  //3. Qualitative evaluation of the trained model.
  //4. Inference on new images.
  //
  dev_update_off();
  if (HDevWindowStack::IsOpen())
    CloseWindow(HDevWindowStack::Pop());
  SetSystem("seed_rand", 73);
  //
  //
  //***   0.) SET INPUT/OUTPUT PATHS   ***
  //
  //Set path to data.
  get_image_dir(&hv_HalconImages);
  //
  //Base directory containing the folders with the used images.
  //There must be a 'good' or 'ok' subfolder in ImageDir.
  //For training, only images in the 'good' or 'ok' folders are used.
  //Images in subfolders named differently will be treated as containing anomalies.
  //They can be used for qualitative evaluation later.
  //╤╡┴╖╓╗╨шok╬─╝■╝╨гм╞ф╦√╬─╝■╝╨╙├╙┌╓о║є╡─╞└╣└
  //Note: You can immediately train an anomaly detection model on your own data as long
  //as your images not showing any anomaly are located in a subfolder called 'good' or 'ok'.┬╖╛╢╝░╫╙╬─╝■╝╨├√
  hv_ImageDir = hv_HalconImages+"/bottles";
  hv_ImageSubDirs.Clear();
  hv_ImageSubDirs[0] = "good";
  hv_ImageSubDirs[1] = "broken_large";
  hv_ImageSubDirs[2] = "broken_small";
  hv_ImageSubDirs[3] = "contamination";
  //
  //Folder where the ground truth anomaly regions are stored.
  //This folder is not required for training.
  //If there is no ground truth data available, set AnomalyDir to [].╚▒╧▌╟°╙Є╡─╢■╓╡═╝┬╖╛╢ги╬▐гй
  hv_AnomalyDir = hv_HalconImages+"/labels/bottles";
  //
  //Folder where the preprocessed samples will be stored.╦∙╙╨╤∙▒╛╘д┤ж└э║є╡─▒г┤ц┬╖╛╢
  hv_OutputDir = "./anomaly_bottle_data";
  //
  //Dataset specific preprocessing.╩¤╛▌╝п╠╪╢и╡─╘д┤ж└э
  hv_ExampleSpecificPreprocessing = 1;
  //
  //Set image size. It should be large enough if the defects are small.
  //Please refer to the documentation of read_dl_model for possible restrictions
  //depending on the network.╦ї╖┼║є╡─┤є╨бги32╡─▒╢╩¤гй
  hv_ImageWidth = 320;
  hv_ImageHeight = 320;
  //
  //Set the complexity of the model which roughly describes the capability of the model
  //to handle complex application images. A higher value can improve the performance but
  //increases the time needed to train the model.╕┤╘╙╢╚гм╘╜┤є╫╝╚╖┬╩╘╜╕▀гм╤╡┴╖╘╜║─╩▒
  hv_Complexity = 15;
  //
  //
  //***   1.) PREPARE   ***
  //
  //Read and preprocess an anomaly detection dataset.
  //For optimal performance it is beneficial to do a custom
  //preprocessing step that defines input domains.
  //
  //Load and split the dataset.
  CreateDict(&hv_GenParamDataset);
  SetDictTuple(hv_GenParamDataset, "image_sub_dirs", hv_ImageSubDirs);
  read_dl_dataset_anomaly(hv_ImageDir, hv_AnomalyDir, HTuple(), HTuple(), hv_GenParamDataset, 
      &hv_DLDataset);
  //
  //Note: make sure that every split contains at least one 'ok' image.
  //For a small number of images in the dataset you might need to increase
  //the validation fraction. ▓Ё╖╓╤∙▒╛╝п╬к╤╡┴╖╝п(50%)бв╤щ╓д╝п(10%)бв▓т╩╘╝п(╩г╙р╡─40%)
  split_dl_dataset(hv_DLDataset, 50, 10, HTuple());
  //
  //Load the anomaly detection model and set parameters.
  //For documentation see set_dl_model_param () and get_dl_model_param ().
  ReadDlModel("initial_dl_anomaly_medium.hdl", &hv_DLModelHandle);
  SetDlModelParam(hv_DLModelHandle, "image_width", hv_ImageWidth);
  SetDlModelParam(hv_DLModelHandle, "image_height", hv_ImageHeight);
  SetDlModelParam(hv_DLModelHandle, "complexity", hv_Complexity);
  //In this example, the training of the anomaly detection model is done on the CPU.
  QueryAvailableDlDevices((HTuple("runtime").Append("id")), (HTuple("cpu").Append(0)), 
      &hv_DLDevice);
  SetDlModelParam(hv_DLModelHandle, "device", hv_DLDevice);
  //
  //Set preprocessing parameters and preprocess.
  CreateDict(&hv_PreprocessSettings);
  SetDictTuple(hv_PreprocessSettings, "overwrite_files", 1);
  create_dl_preprocess_param("anomaly_detection", hv_ImageWidth, hv_ImageHeight, 
      3, HTuple(), HTuple(), "constant_values", "full_domain", HTuple(), HTuple(), 
      HTuple(), HTuple(), &hv_DLPreprocessParam);
  preprocess_dl_dataset(hv_DLDataset, hv_OutputDir, hv_DLPreprocessParam, hv_PreprocessSettings, 
      &hv_DLDatasetFileName);
  //
  //Run a specific preprocessing for this example.
  //Note: For your own application, this step needs to be adapted.
  //In case the possible areas for defects (ROI) on the images can be restricted,
  //we recommend to define these areas as domains. For this example, this is
  //done in the exemplary procedure preprocess_dl_sample_bottle.╗ё╚б╤∙▒╛╝пDLDataset╓╨╡─╤∙▒╛
  GetDictTuple(hv_DLDataset, "samples", &hv_DatasetSamples);
  if (0 != hv_ExampleSpecificPreprocessing)
  {
    read_dl_samples(hv_DLDataset, HTuple::TupleGenSequence(0,(hv_DatasetSamples.TupleLength())-1,1), 
        &hv_DLSampleBatch);
    preprocess_dl_samples_bottle(hv_DLSampleBatch);
    write_dl_samples(hv_DLDataset, HTuple::TupleGenSequence(0,(hv_DatasetSamples.TupleLength())-1,1), 
        hv_DLSampleBatch, HTuple(), HTuple());
  }
  //
  //Visually inspect ten randomly selected preprocessed DLSamples.╒╣╩╛10╕Ў╦ц╗·╘д┤ж└э║є╡─ DLSamples
  CreateDict(&hv_WindowDict);
  for (hv_Index=0; hv_Index<=9; hv_Index+=1)
  {
    hv_SampleIndex = HTuple(HTuple::TupleRand(1)*(hv_DatasetSamples.TupleLength())).TupleInt();
    read_dl_samples(hv_DLDataset, hv_SampleIndex, &hv_DLSample);
    dev_display_dl_data(hv_DLSample, HTuple(), hv_DLDataset, "anomaly_ground_truth", 
        HTuple(), hv_WindowDict);
    if (HDevWindowStack::IsOpen())
      DispText(HDevWindowStack::GetActive(),"Press Run (F5) to continue", "window", 
          "bottom", "right", "black", HTuple(), HTuple());
    //
    GetDictTuple(hv_WindowDict, "anomaly_ground_truth", &hv_WindowHandles);
    HDevWindowStack::SetActive(HTuple(hv_WindowHandles[0]));
    if (HDevWindowStack::IsOpen())
      DispText(HDevWindowStack::GetActive(),"Preprocessed image", "window", "top", 
          "left", "black", HTuple(), HTuple());
    //
    // stop(...); only in hdevelop
  }
  dev_close_window_dict(hv_WindowDict);
  //
  // stop(...); only in hdevelop
  //
  //
  //***   2.) TRAIN   ***
  //
  //Create training parameters.
  //╩╟╖ё╒╣╩╛╤╡┴╖╣¤│╠
  //Control whether the training progress is displayed (true/false).
  hv_EnableDisplay = 1;
  //╔ш╓├╤╡┴╖╓╒╓╣╠ї╝■гм┤э╬є┬╩бв┤╬╩¤гм┬·╫у╞ф╥╗╘Є╓╒╓╣
  //Set a threshold for the training error and a maximum number of training epochs.
  //If the training error falls below this threshold, the training is finished.
  //Otherwise the training continues until the maximum number of epochs is reached.
  hv_ErrorThreshold = 0.001;
  hv_MaxNumEpochs = 15;
  //
  //Set the domain ratio which controls the fraction of each image used for training.
  //The training result might be improved by setting a greater value, but this also
  //increases the training time.╤╡┴╖╝п╓╨╙├╙┌╤╡┴╖╡─╤∙▒╛▒╚
  hv_DomainRatio = 0.25;
  //
  //Regularization noise can make the training more robust. In case the training fails,
  //setting a higher value might help.╒¤╘Є╗п╘ы╔∙гм╩╣╡├╤╡┴╖╕№╜б╫│бг╬к╖└╓╣╤╡┴╖╩з░▄гм┐╔╥╘╔ш╓├┤є╨й
  hv_RegularizationNoise = 0.01;
  //┤┤╜и╫╓╡фгм▓в┤ц┤в╝№-╓╡╢╘
  CreateDict(&hv_TrainParamAnomaly);
  SetDictTuple(hv_TrainParamAnomaly, "regularization_noise", hv_RegularizationNoise);
  SetDictTuple(hv_TrainParamAnomaly, "error_threshold", hv_ErrorThreshold);
  SetDictTuple(hv_TrainParamAnomaly, "domain_ratio", hv_DomainRatio);
  create_dl_train_param(hv_DLModelHandle, hv_MaxNumEpochs, HTuple(), hv_EnableDisplay, 
      73, "anomaly", hv_TrainParamAnomaly, &hv_TrainParam);//┤┤╜и╤╡┴╖▓╬╩¤
  //
  //The training and thus the call of train_dl_model_anomaly_dataset ()
  //is done using the following procedure. This may take some time.
  train_dl_model(hv_DLDataset, hv_DLModelHandle, hv_TrainParam, 0, &hv_TrainResults, 
      &hv_TrainInfos, &hv_EvaluationInfos);
  if (HDevWindowStack::IsOpen())
    DispText(HDevWindowStack::GetActive(),"Press Run (F5) to continue", "window", 
        "bottom", "right", "black", HTuple(), HTuple());
  // stop(...); only in hdevelop
  //
  if (HDevWindowStack::IsOpen())
    CloseWindow(HDevWindowStack::Pop());
  //
  //
  //***   3.) EVALUATE   ***
  //
  //Calculate classification and segmentation thresholds and perform evaluation.
  //╞└╣└─г╨═(╝╞╦у╡├╡╜╖╓└рбв╖╓╕ю╡─у╨╓╡)
  //Set the factor used to calculate the anomaly score. See the documentation of
  //get_dl_model_param for details.
  //This parameter can help to improve the anomaly score for a better classification
  //between 'ok' and 'nok' images. For example, in case the defects are small, a larger
  //value might be suitable.▒ъ╫╝▓ю╥Є╫╙ги╚ч╣√╚▒╧▌║▄╨бгм═╞╝Ў╜╧┤є╓╡гй
  hv_StandardDeviationFactor = 1.0;
  SetDlModelParam(hv_DLModelHandle, "standard_deviation_factor", hv_StandardDeviationFactor);
  //
  //Estimate threshold values. They are used to determine whether a pixel or image
  //is regarded as anomalous. The procedure compute_dl_anomaly_thresholds returns
  //possible suggestions based on the dataset used. Depending on the application, manual
  //fine-tuning may be beneficial.╝╞╦уу╨╓╡
  CreateDict(&hv_GenParamThreshold);
  SetDictTuple(hv_GenParamThreshold, "enable_display", "true");
  compute_dl_anomaly_thresholds(hv_DLModelHandle, hv_DLDataset, hv_GenParamThreshold, 
      &hv_AnomalySegmentationThreshold, &hv_AnomalyClassificationThresholds);
  if (HDevWindowStack::IsOpen())
    DispText(HDevWindowStack::GetActive(),"Press Run (F5) to continue", "window", 
        "bottom", "right", "black", HTuple(), HTuple());
  // stop(...); only in hdevelop
  //
  if (HDevWindowStack::IsOpen())
    CloseWindow(HDevWindowStack::Pop());
  // ╔ш╓├╞└╣└▓╬╩¤гм╘┌test╝п╔╧╜°╨╨╞└╣└
  //Set generic evaluation parameters and evaluate the model on the test split.
  CreateDict(&hv_GenParamEvaluation);
  SetDictTuple(hv_GenParamEvaluation, "measures", "all");
  SetDictTuple(hv_GenParamEvaluation, "anomaly_classification_thresholds", hv_AnomalyClassificationThresholds);
  evaluate_dl_model(hv_DLDataset, hv_DLModelHandle, "split", "test", hv_GenParamEvaluation, 
      &hv_EvaluationResult, &hv_EvalParams);
  //
  //Set display parameters.╥к╒╣╩╛╡─▓╬╩¤
  CreateDict(&hv_GenParamDisplay);
  //
  //Visualize the histogram over the anomaly scores and with it the classification
  //thresholds used for the evaluation.╓▒╖╜═╝бв═╝└¤
  SetDictTuple(hv_GenParamDisplay, "display_mode", (HTuple("score_histogram").Append("score_legend")));
  CreateDict(&hv_WindowDict);
  dev_display_anomaly_detection_evaluation(hv_EvaluationResult, hv_EvalParams, hv_GenParamDisplay, 
      hv_WindowDict);
  if (HDevWindowStack::IsOpen())
    DispText(HDevWindowStack::GetActive(),"Press Run (F5) to continue", "window", 
        "bottom", "right", "black", "box", "true");
  // stop(...); only in hdevelop
  //
  dev_close_window_dict(hv_WindowDict);
  //┐╔╩╙╗пprecision╛л╚╖┬╩, recall╒┘╗╪┬╩, and confusion matrix
  //Visualize evaluation results such as precision, recall, and confusion matrix
  //for a given classification threshold.
  SetDictTuple(hv_GenParamDisplay, "display_mode", ((HTuple("pie_charts_precision").Append("pie_charts_recall")).Append("absolute_confusion_matrix")));
  //Select evaluation results for one threshold by its index in
  //AnomalyClassificationThresholds for display. We use the last
  //one of the computed AnomalyClassificationThresholds. Note that
  //you have to use the AnomalyClassificationThreshold which suits
  //your application best.╒╣╩╛ AnomalyClassificationThresholds ╓╨╡─╥╗╕Ўу╨╓╡ги╡┌╚¤╕Ўгй
  hv_ClassificationThresholdIndex = (hv_AnomalyClassificationThresholds.TupleLength())-1;
  SetDictTuple(hv_GenParamDisplay, "classification_threshold_index", hv_ClassificationThresholdIndex);
  CreateDict(&hv_WindowDict);
  dev_display_anomaly_detection_evaluation(hv_EvaluationResult, hv_EvalParams, hv_GenParamDisplay, 
      hv_WindowDict);
  if (HDevWindowStack::IsOpen())
    DispText(HDevWindowStack::GetActive(),"Press Run (F5) to continue", "window", 
        "bottom", "right", "black", HTuple(), HTuple());
  // stop(...); only in hdevelop
  //
  dev_close_window_dict(hv_WindowDict);
  //
  //Store the trained model.▒г┤ц─г╨═
  WriteDlModel(hv_DLModelHandle, "model_final.hdl");
  //
  //
  //***   4.) INFER   ***
  //
  //To demonstrate the inference steps, we apply the
  //trained model to some randomly chosen example images. ▓т╩╘╡─╤∙▒╛гм╦ц╗·╡─10╕Ўng═╝(╡═╙┌10╥╘╩╡╝╩╬к╫╝)
  list_image_files((hv_ImageDir+"/")+hv_ImageSubDirs, "default", "recursive", &hv_ImageFiles);
  tuple_shuffle(hv_ImageFiles, &hv_ImageFilesShuffled);//┤Є┬╥╩¤╛▌╝п
  //╔ш╓├у╨╓╡ги─г╨═╤╡┴╖║є╡├╡╜гй
  //Choose thresholds for inference.
  hv_InferenceClassificationThreshold = HTuple(hv_AnomalyClassificationThresholds[hv_ClassificationThresholdIndex]);
  hv_InferenceSegmentationThreshold = hv_AnomalySegmentationThreshold;
  //┤┤╜и└р▒Ё▒ъ╟й╫╓╡ф(▓╗╞Ё╫ў╙├гм╡л╩╟▒╪╨ы╙╨)
  //Create dictionary with dataset parameters used for display.
  CreateDict(&hv_DLDatasetInfo);
  SetDictTuple(hv_DLDatasetInfo, "class_names", (HTuple("ok").Append("nok")));
  SetDictTuple(hv_DLDatasetInfo, "class_ids", (HTuple(0).Append(1)));
  //┤┤╜и╫╓╡фгм│╨╘╪┤░╠х╨┼╧в
  CreateDict(&hv_WindowDict);
  {
  HTuple end_val237 = (HTuple(hv_ImageFilesShuffled.TupleLength()).TupleMin2(10))-1;
  HTuple step_val237 = 1;
  for (hv_IndexInference=0; hv_IndexInference.Continue(end_val237, step_val237); hv_IndexInference += step_val237)
  {
    ReadImage(&ho_Image, HTuple(hv_ImageFilesShuffled[hv_IndexInference]));
    gen_dl_samples_from_images(ho_Image, &hv_DLSample);
    preprocess_dl_samples(hv_DLSample, hv_DLPreprocessParam);
    //╙ы╤╡┴╖╩▒╧р═м╡─╠╪╢и┤ж└э
    //Use the same dataset specific preprocessing as for training.
    if (0 != hv_ExampleSpecificPreprocessing)
    {
      preprocess_dl_samples_bottle(hv_DLSample);
    }
    //
    ApplyDlModel(hv_DLModelHandle, hv_DLSample, HTuple(), &hv_DLResult);
    //
    //Apply thresholds to classify regions and the entire image.
    threshold_dl_anomaly_results(hv_InferenceSegmentationThreshold, hv_InferenceClassificationThreshold, 
        hv_DLResult);
    //
    //Display the inference result.
    dev_display_dl_data(hv_DLSample, hv_DLResult, hv_DLDatasetInfo, (HTuple("anomaly_result").Append("anomaly_image")), 
        HTuple(), hv_WindowDict);
    if (HDevWindowStack::IsOpen())
      DispText(HDevWindowStack::GetActive(),"Press F5 (continue)", "window", "bottom", 
          "center", "black", HTuple(), HTuple());
    // stop(...); only in hdevelop
  }
  }
  //
  dev_close_window_dict(hv_WindowDict);
}


