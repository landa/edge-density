#include <stdlib.h>
#include <stdio.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <string.h>
#include <dirent.h>

using namespace cv;
using namespace std;

vector<string> getFileNamesInDirectory(char* directory) {
  vector<string> filenames;
  DIR *dir;
  struct dirent *ent;
  dir = opendir (directory);
  if (dir != NULL) {
    while ((ent = readdir (dir)) != NULL) {
      if (strcmp(ent->d_name, ".") && strcmp(ent->d_name, "..")) {
        stringstream image_location;
        image_location << directory << "/" << ent->d_name;
        filenames.push_back(image_location.str());
      }
    }
    closedir(dir);
  } else {
    perror("");
  }
  sort(filenames.begin(), filenames.end());
  return filenames;
}

double getEdgeDensity(const cv::Mat& input) {
  int low_threshold = 30;
  int ratio = 3;
  int kernel_size = 3;
  Mat canny;
  Canny(input, canny, low_threshold, ratio*low_threshold, kernel_size);
  int nonzero = countNonZero(canny);
  double density = nonzero/((double)canny.rows*canny.cols);
  return density;
}

double getEdgeDensityScore(double edge_density) {
  // edge_density is in [0, 1]
  // score rises steadily until it's clamped at 1 at 0.02
  return MAX(0.0, MIN(1.0, 50*edge_density));
}

int main(int argc, char** argv) {


  if (argc != 2) { fprintf(stderr, "no input file\n"); return 1; }

  vector<string> filenames = getFileNamesInDirectory(argv[1]);

  for (int ii = 0; ii < filenames.size(); ++ii) {
    Mat input = imread(filenames[ii], 0);

    double density = getEdgeDensity(input);
    double score = getEdgeDensityScore(density);

    fprintf(stderr, "%s edge density = %.3f, score = %.3f\n",
                    filenames[ii].c_str(), density, score);
  }

}
