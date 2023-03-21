#include <glm/gtx/euler_angles.hpp>

// 计算切线方向向量 tangent_vector

// 将切线方向向量转化为欧拉角
glm::vec3 euler_angles = glm::eulerAngles(glm::quat_cast(glm::mat3(glm::normalize(tangent_vector), binormal_vector, normal_vector)));

// 将弧度制转化为角度制
float roll = glm::degrees(euler_angles.x);
float yaw = glm::degrees(euler_angles.y);
float pitch = glm::degrees(euler_angles.z);


#include <cmath>
#include <iostream>

const double a = 6378137.0;  // WGS84椭球体半长轴
const double b = 6356752.3142;  // WGS84椭球体半短轴
const double e2 = 1.0 - b*b/(a*a);  // 第一偏心率的平方

struct GeodeticCoord {
  double lat;  // 纬度（单位：弧度）
  double lon;  // 经度（单位：弧度）
  double alt;  // 高度（单位：米）
};

struct ECEFCoord {
  double x;  // X 坐标（单位：米）
  double y;  // Y 坐标（单位：米）
  double z;  // Z 坐标（单位：米）
};

// 将大地坐标系中的点转换为空间直角坐标系中的点
ECEFCoord GeodeticToECEF(const GeodeticCoord& g) {
  double sinlat = sin(g.lat);
  double coslat = cos(g.lat);
  double sinlon = sin(g.lon);
  double coslon = cos(g.lon);
  double N = a / sqrt(1.0 - e2 * sinlat*sinlat);

  ECEFCoord ecef;
  ecef.x = (N + g.alt) * coslat * coslon;
  ecef.y = (N + g.alt) * coslat * sinlon;
  ecef.z = (b*b/(a*a)*N + g.alt) * sinlat;
  return ecef;
}

int main() {
  GeodeticCoord g = {0.7854, 1.5708, 100.0};  // 纬度、经度、高度
  ECEFCoord e = GeodeticToECEF(g);
  std::cout << "ECEF coordinates: (" << e.x << ", " << e.y << ", " << e.z << ")\n";
  return 0;
}

#import <Foundation/Foundation.h>

int main(int argc, const char * argv[]) {
    @autoreleasepool {
        NSFileManager *fileManager = [NSFileManager defaultManager];
        
        // 获取目录路径
        NSString *directoryPath = @"/path/to/your/directory";
        
        NSError *error;
        NSArray *files = [fileManager contentsOfDirectoryAtPath:directoryPath error:&error];
        
        if (files == nil) {
            NSLog(@"Error: %@", error.localizedDescription);
        } else {
            for (NSString *file in files) {
                NSLog(@"%@", file);
            }
        }
    }
    return 0;
}

#import <UIKit/UIKit.h>
#import <Photos/Photos.h>
#import <AVFoundation/AVFoundation.h>
#import <opencv2/opencv.hpp>

@interface ViewController : UIViewController

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    [self fetchAndCompareVideosFromAlbum1:@"相册1" album2:@"相册2"];
}

- (void)fetchAndCompareVideosFromAlbum1:(NSString *)album1 album2:(NSString *)album2 {
    NSMutableArray *videos1 = [self fetchVideosFromAlbum:album1];
    NSMutableArray *videos2 = [self fetchVideosFromAlbum:album2];

    for (NSInteger i = 0; i < videos1.count; i++) {
        PHAsset *videoAsset1 = videos1[i];
        PHAsset *videoAsset2 = videos2[i];

        __block NSURL *videoUrl1;
        __block NSURL *videoUrl2;

        dispatch_group_t group = dispatch_group_create();

        dispatch_group_enter(group);
        [[PHImageManager defaultManager] requestAVAssetForVideo:videoAsset1 options:nil resultHandler:^(AVAsset *avAsset, AVAudioMix *audioMix, NSDictionary *info) {
            videoUrl1 = [(AVURLAsset *)avAsset URL];
            dispatch_group_leave(group);
        }];

        dispatch_group_enter(group);
        [[PHImageManager defaultManager] requestAVAssetForVideo:videoAsset2 options:nil resultHandler:^(AVAsset *avAsset, AVAudioMix *audioMix, NSDictionary *info) {
            videoUrl2 = [(AVURLAsset *)avAsset URL];
            dispatch_group_leave(group);
        }];

        dispatch_group_wait(group, DISPATCH_TIME_FOREVER);

        [self compareVideos:videoUrl1 videoUrl2:videoUrl2];
    }
}

- (NSMutableArray *)fetchVideosFromAlbum:(NSString *)albumName {
    NSMutableArray *videos = [NSMutableArray array];
    PHFetchOptions *fetchOptions = [[PHFetchOptions alloc] init];
    fetchOptions.predicate = [NSPredicate predicateWithFormat:@"mediaType = %d", PHAssetMediaTypeVideo];

    PHAssetCollection *targetCollection;
    PHFetchResult *collections = [PHAssetCollection fetchAssetCollectionsWithType:PHAssetCollectionTypeAlbum subtype:PHAssetCollectionSubtypeAny options:nil];

    for (PHAssetCollection *collection in collections) {
        if ([collection.localizedTitle isEqualToString:albumName]) {
            targetCollection = collection;
            break;
        }
    }

    if (targetCollection) {
        PHFetchResult *assets = [PHAsset fetchAssetsInAssetCollection:targetCollection options:fetchOptions];

        for (PHAsset *asset in assets) {
            [videos addObject:asset];
        }
    } else {
        NSLog(@"相册未找到");
    }

    return videos;
}

- (void)compareVideos:(NSURL *)videoUrl1 videoUrl2:(NSURL *)videoUrl2 {
    // TODO: 使用 OpenCV 逐帧计算亮度差异并输出结果
}

@end
// Load the image
NSString *imagePath = [[NSBundle mainBundle] pathForResource:@"example" ofType:@"jpg"];
cv::Mat inputImage = cv::imread([imagePath UTF8String]);

// Check if the image was loaded
if (inputImage.empty()) {
    NSLog(@"Failed to load the image.");
    return;
}

// Convert the image to grayscale
cv::Mat grayImage;
cv::cvtColor(inputImage, grayImage, cv::COLOR_BGR2GRAY);

// Save the grayscale image to the app's Documents folder
NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
NSString *documentsDirectory = [paths objectAtIndex:0];
NSString *grayImagePath = [documentsDirectory stringByAppendingPathComponent:@"gray_image.png"];

if (cv::imwrite([grayImagePath UTF8String], grayImage)) {
    NSLog(@"Grayscale image saved at: %@", grayImagePath);
} else {
    NSLog(@"Failed to save the grayscale image.");
}


