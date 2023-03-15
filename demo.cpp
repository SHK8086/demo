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

