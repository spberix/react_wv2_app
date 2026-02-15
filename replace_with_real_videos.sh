#!/bin/bash

# Script to help replace test videos with real footage
#
# Usage:
# 1. Download 9 videos of people at desks (MP4 format)
# 2. Place them in a folder (e.g., ~/Downloads/desk_videos/)
# 3. Run: ./replace_with_real_videos.sh ~/Downloads/desk_videos/

VIDEO_DIR="/tmp/video_grid_test"
SOURCE_DIR="${1}"

if [ -z "$SOURCE_DIR" ]; then
    echo "Usage: $0 <source_directory>"
    echo "Example: $0 ~/Downloads/desk_videos/"
    exit 1
fi

if [ ! -d "$SOURCE_DIR" ]; then
    echo "Error: Directory '$SOURCE_DIR' does not exist"
    exit 1
fi

# Check if FFmpeg is installed
if ! command -v ffmpeg &> /dev/null; then
    echo "Error: FFmpeg is not installed"
    echo "Install with: brew install ffmpeg"
    exit 1
fi

echo "Converting videos from: $SOURCE_DIR"
echo "Output directory: $VIDEO_DIR"
echo ""

# Find all video files in source directory
video_files=($(find "$SOURCE_DIR" -type f \( -iname "*.mp4" -o -iname "*.mov" -o -iname "*.avi" -o -iname "*.mkv" \) | head -9))

if [ ${#video_files[@]} -eq 0 ]; then
    echo "Error: No video files found in $SOURCE_DIR"
    exit 1
fi

echo "Found ${#video_files[@]} video file(s)"
echo ""

# Create output directory
mkdir -p "$VIDEO_DIR"

# Convert each video
for i in "${!video_files[@]}"; do
    input="${video_files[$i]}"
    output="$VIDEO_DIR/test_video_$i.mp4"

    echo "[$((i+1))/${#video_files[@]}] Processing: $(basename "$input")"

    # Convert to 1280x720, 10 seconds, 30fps
    ffmpeg -i "$input" \
        -vf "scale=1280:720:force_original_aspect_ratio=decrease,pad=1280:720:(ow-iw)/2:(oh-ih)/2,fps=30" \
        -t 10 \
        -c:v libx264 -preset fast -crf 23 \
        -c:a aac -b:a 128k \
        -y "$output" > /dev/null 2>&1

    if [ $? -eq 0 ]; then
        echo "   ✓ Saved to: $output"
    else
        echo "   ✗ Failed to convert"
    fi
done

echo ""
echo "Done! Converted ${#video_files[@]} videos"
echo "Videos are now ready to use in the application"
