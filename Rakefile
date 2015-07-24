CLANG = "clang-602.0.53"
CLANG_TARBALL = "#{CLANG}.tar.gz"
CLANG_ROOT_DIR = "CLANG_ROOT"

namespace :build do
  desc "Build clang"
  task :clang do
    cpu = `sysctl -n hw.ncpu`.strip
    sh "tar -xjof #{CLANG_TARBALL}"
    mkdir_p "CLANG_BUILD_DIR"
    mkdir_p CLANG_ROOT_DIR

    Dir.chdir("CLANG_BUILD_DIR") do
      sh "../clang-602.0.53/src/configure --enable-libcpp --enable-debug-runtime --enable-debug-symbols --disable-timestamps --disable-assertions--without-llvmgcc --without-llvmgxx --disable-bindings --disable-doxygen"
      sh "make -j#{cpu}"
      sh "make install DESTDIR=" + File.expand_path("../#{CLANG_ROOT_DIR}")
    end
  end
end
