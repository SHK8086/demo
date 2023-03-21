// 导入必要的头文件
#import <UIKit/UIKit.h>
#import <Photos/Photos.h>
#import <AVFoundation/AVFoundation.h>
#import <opencv2/opencv.hpp>

// 定义一个名为 ViewController 的类，继承自 UIViewController
@interface ViewController : UIViewController

@end

@implementation ViewController

// 当 ViewController 加载视图时，调用这个方法
- (void)viewDidLoad {
    [super viewDidLoad];
    
    // 获取并比较相册1和相册2中的视频
    [self fetchAndCompareVideosFromAlbum1:@"相册1" album2:@"相册2"];
}

// 获取并比较两个相册中的视频
- (void)fetchAndCompareVideosFromAlbum1:(NSString *)album1 album2:(NSString *)album2 {
    // 获取两个相册中的视频
    NSMutableArray *videos1 = [self fetchVideosFromAlbum:album1];
    NSMutableArray *videos2 = [self fetchVideosFromAlbum:album2];

    // 遍历视频数组
    for (NSInteger i = 0; i < videos1.count; i++) {
        PHAsset *videoAsset1 = videos1[i];
        PHAsset *videoAsset2 = videos2[i];

        // 用于存储视频 URL 的变量
        __block NSURL *videoUrl1;
        __block NSURL *videoUrl2;

        // 创建一个 dispatch group 以异步获取视频 URL
        dispatch_group_t group = dispatch_group_create();

        // 获取第一个视频的 URL
        dispatch_group_enter(group);
        [[PHImageManager defaultManager] requestAVAssetForVideo:videoAsset1 options:nil resultHandler:^(AVAsset *avAsset, AVAudioMix *audioMix, NSDictionary *info) {
            videoUrl1 = [(AVURLAsset *)avAsset URL];
            dispatch_group_leave(group);
        }];

        // 获取第二个视频的 URL
        dispatch_group_enter(group);
        [[PHImageManager defaultManager] requestAVAssetForVideo:videoAsset2 options:nil resultHandler:^(AVAsset *avAsset, AVAudioMix *audioMix, NSDictionary *info) {
            videoUrl2 = [(AVURLAsset *)avAsset URL];
            dispatch_group_leave(group);
        }];

        // 等待异步任务完成
        dispatch_group_wait(group, DISPATCH_TIME_FOREVER);

        // 比较两个视频
        [self compareVideos:videoUrl1 videoUrl2:videoUrl2];
    }
}

// 从指定相册中获取视频
- (NSMutableArray *)fetchVideosFromAlbum:(NSString *)albumName {
    // 创建一个可变数组存储视频
    NSMutableArray *videos = [NSMutableArray array];
    // 设置获取选项，仅获取视频
    PHFetchOptions *fetchOptions = [[PHFetchOptions alloc] init];
    fetchOptions.predicate = [NSPredicate predicateWithFormat:@"mediaType = %d", PHAssetMediaTypeVideo];

    // 查找目标相册
    PHAssetCollection *targetCollection;
    PHFetchResult *collections = [PHAssetCollection fetchAssetCollectionsWithType:PHAssetCollectionTypeAlbum subtype:PHAssetCollectionSubtypeAny options:nil];

    for (PHAssetCollection *collection in collections) {
        if ([collection.localizedTitle isEqualToString:albumName]) {
            targetCollection = collection;
            break;
        }
    }

    // 如果找到相册
// 如果找到相册，则获取其中的视频
if (targetCollection) {
    // 获取相册中的所有视频资源
    PHFetchResult *assets = [PHAsset fetchAssetsInAssetCollection:targetCollection options:fetchOptions];

    // 将视频资源添加到数组中
    for (PHAsset *asset in assets) {
        [videos addObject:asset];
    }
} else {
    // 如果未找到相册，输出提示信息
    NSLog(@"相册未找到");
}

// 返回包含视频的数组
return videos;
