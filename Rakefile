CLANG = "clang-602.0.53"
CLANG_TARBALL = "#{CLANG}.tar.gz"
CLANG_ROOT_DIR = "CLANG_ROOT"
LLVM_CONFIG = "#{CLANG_ROOT_DIR}/usr/local/bin/llvm-config"

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

  desc "Build sample1"
  task :sample1 do
    cxxflags = `#{LLVM_CONFIG} --cxxflags`.strip
    ldflags = `#{LLVM_CONFIG} --ldflags`.strip
    system_libs = `#{LLVM_CONFIG} --system-libs`.strip
    llvm_libs = `#{LLVM_CONFIG} --libs`.strip
    clang_libs = %w(
      -lclangTooling
      -lclangFrontendTool
      -lclangFrontend
      -lclangDriver
      -lclangSerialization
      -lclangCodeGen
      -lclangParse
      -lclangSema
      -lclangAPINotes
      -lclangStaticAnalyzerFrontend
      -lclangStaticAnalyzerCheckers
      -lclangStaticAnalyzerCore
      -lclangAnalysis
      -lclangARCMigrate
      -lclangRewrite
      -lclangRewriteFrontend
      -lclangEdit
      -lclangAST
      -lclangLex
      -lclangBasic
    ).join(" ")
    sh "clang++ #{cxxflags} #{ldflags} #{system_libs} #{llvm_libs} #{clang_libs} -o sample1 sample1.cpp"
  end
end

task :default => :"build:sample1"
