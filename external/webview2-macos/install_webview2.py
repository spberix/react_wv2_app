# Copyright (C) Microsoft Corporation. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

import argparse
import os
import plistlib
import sys
import shutil

all_helper_suffix = [
  " Helper",
  " Helper (GPU)",
  " Helper (Plugin)",
  " Helper (Renderer)",
]

# Change 'Microsoft WebView' in Contents/Resources/xxx.lproj/InfoPlist.string to new_webview2_name.
def ModifyLocalizedInfoPlist(output_path, new_webview2_name):
  plist_string_folder = os.path.join(output_path, 'Contents', 'Resources')
  if not os.path.exists(plist_string_folder):
    raise Exception('%s doesn\'t exsit.' % plist_string_folder)

  dirs = os.listdir(plist_string_folder)
  for dir in dirs:
    if not dir.endswith('.lproj'):
      continue
    
    plist_string_file = os.path.join(plist_string_folder, dir, 'InfoPlist.strings')
    if not os.path.isfile(plist_string_file):
      print('%s doesn\'t exsit.' % plist_string_file)
      continue

    with open(plist_string_file, 'r') as file:
      string_data = file.read()

    string_data = string_data.replace('Microsoft WebView', new_webview2_name)
    with open(plist_string_file, 'w') as file:
        file.write(string_data)

def ModifyWebview(original_webview2, output_dir, new_webview2_name, bundle_id, stamp_file):
  if not os.path.exists(original_webview2):
    raise Exception('%s doesn\'t exsit.' % original_webview2)

  output_path = os.path.join(output_dir, new_webview2_name + '.app')
  if os.path.exists(output_path):
    shutil.rmtree(output_path)

  shutil.copytree(original_webview2, output_path, True)

  try:
    original_name = os.path.basename(original_webview2).split('.')[0]
    original_executable_file_path = os.path.join(output_path, 'Contents', 'MacOS', original_name)
    if not os.path.exists(original_executable_file_path):
      raise Exception('%s doesn\'t exsit.' % original_executable_file_path)

    plist_path = os.path.join(output_path, 'Contents', 'Info.plist')
    if not os.path.exists(plist_path):
      raise Exception('%s doesn\'t exsit.' % plist_path)

    new_executable_file_path = os.path.join(output_path, 'Contents', 'MacOS', new_webview2_name)
    helpers_group_path = os.path.join(output_path, 'Contents', 'Frameworks', 'Microsoft Edge Framework.framework', 'Helpers')

    # Rename the executable
    os.rename(original_executable_file_path, new_executable_file_path)

    # Modify the info.plist
    with open(plist_path, 'rb') as file:
      plist_data = plistlib.load(file)
      
    plist_data['CFBundleDisplayName'] = new_webview2_name
    plist_data['CFBundleExecutable'] = new_webview2_name
    plist_data['CFBundleName'] = new_webview2_name
    plist_data['CFBundleIdentifier'] = bundle_id
    with open(plist_path, 'wb') as file: 
      plistlib.dump(plist_data, file)

    # Modify the InfoPlist.strings file in Resource folder
    ModifyLocalizedInfoPlist(output_path, new_webview2_name)

    # Rename all helper bundle
    for suffix in all_helper_suffix:
      original_helper_name = original_name + suffix
      new_helper_name = new_webview2_name + suffix
      original_helper_path = os.path.join(helpers_group_path, original_helper_name  + ".app")
      new_helper_path = os.path.join(helpers_group_path, new_helper_name + ".app")
      original_exe_path = os.path.join(new_helper_path, 'Contents', 'MacOS', original_helper_name)
      new_exe_path = os.path.join(new_helper_path, 'Contents', 'MacOS', new_helper_name)
      helper_plist_path = os.path.join(new_helper_path, 'Contents', 'Info.plist')

      # Rename the bundle
      os.rename(original_helper_path, new_helper_path)

      # Rename the executable
      os.rename(original_exe_path, new_exe_path)

      # Modify info.plist
      with open(helper_plist_path, 'rb') as file:
        plist_data = plistlib.load(file)
      plist_data['CFBundleDisplayName'] = new_webview2_name
      plist_data['CFBundleExecutable'] = new_helper_name
      plist_data['CFBundleIdentifier'] = bundle_id
      with open(helper_plist_path, 'wb') as file: 
        plistlib.dump(plist_data, file)
      
  except Exception as e:
    # Clean the new bundle
    shutil.rmtree(output_path)
    raise

  if stamp_file:
    # Touch a dummy file to keep a timestamp
    with open(stamp_file, "w") as f:
      f.write("")
    
def Main():
  parser = argparse.ArgumentParser()
  parser.add_argument(
      '--original-webview2',
      required=True,
      help='The path to the original WebView2 bundle')
  parser.add_argument(
      '--output-dir',
      required=True,
      help=('The path where the Webview2 will be put'))
  parser.add_argument(
      '--new-webview2-name',
      required=True,
      help=('The host application name'))
  parser.add_argument(
      '--bundle-id',
      required=True,
      help=('The bundle id of the webview app'))
  parser.add_argument(
      '--stamp',
      help='The stampfile of current action')
      
  args = parser.parse_args()

  ModifyWebview(args.original_webview2, args.output_dir, args.new_webview2_name, args.bundle_id, args.stamp)
  return 0

if __name__ == '__main__':
  sys.exit(Main())
